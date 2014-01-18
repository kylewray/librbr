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


/**
 * The default constructor for the FiniteObservationTransitions class.
 */
FiniteObservationTransitions::FiniteObservationTransitions()
{ }

/**
 * The default deconstructor for the FiniteObservationTransitions class.
 */
FiniteObservationTransitions::~FiniteObservationTransitions()
{
	observationTransitions.clear();
}

/**
 * Set a observation transition from a particular observation-action-state triple to a probability.
 * @param observation	The next observation to which we assign a probability.
 * @param action		The action taken at the current state.
 * @param state			The current state.
 * @param probability	The probability of the observation given we were in the state and took the action.
 */
void FiniteObservationTransitions::set(Observation observation, Action action, State nextState, double probability)
{
	observationTransitions[observation][action][nextState] = std::max(0.0, std::min(1.0, probability));
}

/**
 * The probability of a transition following the observation-action-state triple provided.
 * @param observation	The next observation to which we assign a probability.
 * @param action		The action taken at the current state.
 * @param state			The current state.
 * @return The probability of the observation given we were in the state and took the action.
 */
double FiniteObservationTransitions::get(Observation observation, Action action, State state) const
{
	std::map<Observation, std::map<Action, std::map<State, double> > >::const_iterator alpha = observationTransitions.find(observation);
	if (alpha == observationTransitions.end()) {
		return 0.0;
	}

	std::map<Action, std::map<State, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		return 0.0;
	}

	std::map<State, double>::const_iterator gamma = beta->second.find(state);
	if (gamma == beta->second.end()) {
		return 0.0;
	}

	return gamma->second;
}
