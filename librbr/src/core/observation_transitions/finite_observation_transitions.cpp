/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "../../../include/core/observation_transitions/finite_observation_transitions.h"
#include "../../../include/core/observation_transitions/observation_transition_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"
#include "../../../include/core/observations/named_observation.h"

FiniteObservationTransitions::FiniteObservationTransitions()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");
	observationWildcard = new NamedObservation("*");
}

FiniteObservationTransitions::~FiniteObservationTransitions()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
	delete observationWildcard;
}

void FiniteObservationTransitions::set(const Action *previousAction, const State *state,
		const Observation *observation, double probability)
{
	if (previousAction == nullptr) {
		previousAction = actionWildcard;
	}
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (observation == nullptr) {
		observation = observationWildcard;
	}

	observationTransitions[previousAction][state][observation] = std::max(0.0, std::min(1.0, probability));
}

double FiniteObservationTransitions::get(const Action *previousAction, const State *state,
		const Observation *observation) const
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		const Action *alpha = actionWildcard;
		if (i & (1 << 1)) {
			alpha = previousAction;
		}

		const State *beta = stateWildcard;
		if (i & (1 << 2)) {
			beta = state;
		}

		const Observation *gamma = observationWildcard;
		if (i & (1 << 0)) {
			gamma = observation;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const ObservationTransitionException &err) { }
	}

	return 0.0;
}

void FiniteObservationTransitions::available(const FiniteObservations *Z, const Action *action,
		const State *nextState, std::vector<const Observation *> &result) const
{
	result.clear();
	for (auto z : *Z) {
		const Observation *observation = resolve(z);
		if (get(action, nextState, observation) > 0.0) {
			result.push_back(observation);
		}
	}
}

void FiniteObservationTransitions::reset()
{
	observationTransitions.clear();
}

double FiniteObservationTransitions::get_value(const Action *previousAction, const State *state,
		const Observation *observation) const
{
	std::unordered_map<const Action *,
		std::unordered_map<const State *,
		std::unordered_map<const Observation *, double> > >::const_iterator alpha = observationTransitions.find(previousAction);
	if (alpha == observationTransitions.end()) {
		throw ObservationTransitionException();
	}

	std::unordered_map<const State *,
		std::unordered_map<const Observation *, double> >::const_iterator beta = alpha->second.find(state);
	if (beta == alpha->second.end()) {
		throw ObservationTransitionException();
	}

	std::unordered_map<const Observation *, double>::const_iterator gamma = beta->second.find(observation);
	if (gamma == beta->second.end()) {
		throw ObservationTransitionException();
	}

	return gamma->second;
}
