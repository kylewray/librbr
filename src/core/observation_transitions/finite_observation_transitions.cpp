/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2013 Kyle Wray and Luis Pineda
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

/**
 * The default constructor for the FiniteObservationTransitions class.
 */
FiniteObservationTransitions::FiniteObservationTransitions()
{
	stateWildcard = new State("*");
	actionWildcard = new Action("*");
	observationWildcard = new Observation("*");
}

/**
 * The default deconstructor for the FiniteObservationTransitions class.
 */
FiniteObservationTransitions::~FiniteObservationTransitions()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
	delete observationWildcard;
}

/**
 * Set a observation transition from a particular observation-action-state triple to a probability.
 * @param observation	The next observation to which we assign a probability.
 * @param action		The action taken at the current state.
 * @param state			The current state.
 * @param probability	The probability of the observation given we were in the state and took the action.
 */
void FiniteObservationTransitions::set(Observation *observation, Action *action, State *nextState, double probability)
{
	if (observation == nullptr) {
		observation = observationWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}
	if (nextState == nullptr) {
		nextState = stateWildcard;
	}

	observationTransitions[observation][action][nextState] = std::max(0.0, std::min(1.0, probability));
}

/**
 * The probability of a transition following the observation-action-state triple provided.
 * @param observation	The next observation to which we assign a probability.
 * @param action		The action taken at the current state.
 * @param state			The current state.
 * @return The probability of the observation given we were in the state and took the action.
 */
double FiniteObservationTransitions::get(Observation *observation, Action *action, State *state) const
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		Observation *alpha = observationWildcard;
		if (i & (1 << 0)) {
			alpha = observation;
		}

		Action *beta = actionWildcard;
		if (i & (1 << 1)) {
			beta = action;
		}

		State *gamma = stateWildcard;
		if (i & (1 << 2)) {
			gamma = state;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const ObservationTransitionException &err) { }
	}

	return 0.0;
}

/**
 * Reset the observation transitions, clearing the internal mapping.
 */
void FiniteObservationTransitions::reset()
{
	observationTransitions.clear();
}

/**
 * The actual get function which returns a value. This will throw an error if the value is undefined.
 * It is used as a helper function for the public get function.
 * @param observation	The next observation to which we assign a probability.
 * @param action		The action taken at the current state.
 * @param state			The current state.
 * @return The reward from taking the given action in the given state.
 * @throws ObservationTransitionException The observation transition was not defined.
 */
double FiniteObservationTransitions::get_value(Observation *observation, Action *action, State *state) const
{
	std::map<Observation *, std::map<Action *, std::map<State *, double> > >::const_iterator alpha =
			observationTransitions.find(observation);
	if (alpha == observationTransitions.end()) {
		throw ObservationTransitionException();
	}

	std::map<Action *, std::map<State *, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw ObservationTransitionException();
	}

	std::map<State *, double>::const_iterator gamma = beta->second.find(state);
	if (gamma == beta->second.end()) {
		throw ObservationTransitionException();
	}

	return gamma->second;
}
