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
 * @param previousAction	The action taken at the previous state which resulted in the current state.
 * @param state				The current state.
 * @param observation		The next observation to which we assign a probability.
 * @param probability		The probability of the observation given we took the action and landed in the state given.
 */
void FiniteObservationTransitions::set(Action *previousAction, State *state, Observation *observation, double probability)
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

/**
 * The probability of a transition following the observation-action-state triple provided.
 * @param observation		The next observation to which we assign a probability.
 * @param previousAction	The action taken at the previous state which resulted in the current state.
 * @param state				The current state.
 * @return The probability of the observation given we took the action and landed in the state given.
 */
double FiniteObservationTransitions::get(Action *previousAction, State *state, Observation *observation) const
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
 * @param previousAction	The action taken at the previous state which resulted in the current state.
 * @param state				The current state.
 * @param observation		The next observation to which we assign a probability.
 * @return The probability of the observation given we took the action and landed in the state given.
 * @throws ObservationTransitionException The observation transition was not defined.
 */
double FiniteObservationTransitions::get_value(Action *previousAction, State *state, Observation *observation) const
{
	std::map<Action *, std::map<State *, std::map<Observation *, double> > >::const_iterator alpha =
			observationTransitions.find(previousAction);
	if (alpha == observationTransitions.end()) {
		throw ObservationTransitionException();
	}

	std::map<State *, std::map<Observation *, double> >::const_iterator beta = alpha->second.find(state);
	if (beta == alpha->second.end()) {
		throw ObservationTransitionException();
	}

	std::map<Observation *, double>::const_iterator gamma = beta->second.find(observation);
	if (gamma == beta->second.end()) {
		throw ObservationTransitionException();
	}

	return gamma->second;
}
