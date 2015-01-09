/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Hollins Wray, University of Massachusetts
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


#include "../../../include/core/observation_transitions/observation_transitions_map.h"
#include "../../../include/core/observation_transitions/observation_transition_exception.h"

#include "../../../include/core/observations/observations_map.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"
#include "../../../include/core/observations/named_observation.h"

#include <algorithm>

ObservationTransitionsMap::ObservationTransitionsMap()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");
	observationWildcard = new NamedObservation("*");
}

ObservationTransitionsMap::~ObservationTransitionsMap()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
	delete observationWildcard;
}

void ObservationTransitionsMap::set(Action *previousAction, State *state,
		Observation *observation, double probability)
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

double ObservationTransitionsMap::get(Action *previousAction, State *state,
		Observation *observation)
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		Action *alpha = actionWildcard;
		if (i & (1 << 1)) {
			alpha = previousAction;
		}

		State *beta = stateWildcard;
		if (i & (1 << 2)) {
			beta = state;
		}

		Observation *gamma = observationWildcard;
		if (i & (1 << 0)) {
			gamma = observation;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const ObservationTransitionException &err) { }
	}

	return 0.0;
}

void ObservationTransitionsMap::available(Observations *Z, Action *previousAction, State *state,
		std::vector<Observation *> &result)
{
	ObservationsMap *ZMap = static_cast<ObservationsMap *>(Z);
	if (ZMap == nullptr) {
		throw ObservationTransitionException();
	}

	result.clear();
	for (auto z : *ZMap) {
		Observation *observation = resolve(z);
		if (get(previousAction, state, observation) > 0.0) {
			result.push_back(observation);
		}
	}
}

void ObservationTransitionsMap::reset()
{
	observationTransitions.clear();
}

double ObservationTransitionsMap::get_value(Action *previousAction, State *state,
		Observation *observation)
{
	std::unordered_map<Action *,
		std::unordered_map<State *,
		std::unordered_map<Observation *, double> > >::const_iterator alpha = observationTransitions.find(previousAction);
	if (alpha == observationTransitions.end()) {
		throw ObservationTransitionException();
	}

	std::unordered_map<State *,
		std::unordered_map<Observation *, double> >::const_iterator beta = alpha->second.find(state);
	if (beta == alpha->second.end()) {
		throw ObservationTransitionException();
	}

	std::unordered_map<Observation *, double>::const_iterator gamma = beta->second.find(observation);
	if (gamma == beta->second.end()) {
		throw ObservationTransitionException();
	}

	return gamma->second;
}
