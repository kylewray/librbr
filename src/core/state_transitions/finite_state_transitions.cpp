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


#include "../../../include/core/state_transitions/finite_state_transitions.h"


/**
 * The default constructor for the FiniteStateTransitions class.
 */
FiniteStateTransitions::FiniteStateTransitions()
{ }

/**
 * The default deconstructor for the FiniteStateTransitions class.
 */
FiniteStateTransitions::~FiniteStateTransitions()
{
	stateTransitions.clear();
}

/**
 * Set a state transition from a particular state-action-state triple to a probability.
 * @param state			The current state of the system.
 * @param action		The action taken at the current state.
 * @param nextState		The next state with which we assign the probability.
 * @param probability	The probability of going from the state, taking the action, then
 * 						moving to the nextState.
 */
void FiniteStateTransitions::set(State *state, Action *action, State *nextState, double probability)
{
	stateTransitions[state][action][nextState] = std::max(0.0, std::min(1.0, probability));
}

/**
 * The probability of a transition following the state-action-state triple provided.
 * @param state		The current state of the system.
 * @param action	The action taken at the current state.
 * @param nextState	The next state with which we assign the probability.
 * @return The probability of going from the state, taking the action, then moving to the nextState.
 */
double FiniteStateTransitions::get(State *state, Action *action, State *nextState) const
{
	std::map<State *, std::map<Action *, std::map<State *, double> > >::const_iterator alpha =
			stateTransitions.find(state);
	if (alpha == stateTransitions.end()) {
		alpha = stateTransitions.find(nullptr);
		if (alpha == stateTransitions.end()) {
			return 0.0;
		}
	}

	std::map<Action *, std::map<State *, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		beta = alpha->second.find(nullptr);
		if (beta == alpha->second.end()) {
			return 0.0;
		}
	}

	std::map<State *, double>::const_iterator gamma = beta->second.find(nextState);
	if (gamma == beta->second.end()) {
		gamma = beta->second.find(nullptr);
		if (gamma == beta->second.end()) {
			return 0.0;
		}
	}

	return gamma->second;
}

/**
 * Reset the state transitions, clearing the internal mapping.
 */
void FiniteStateTransitions::reset()
{
	stateTransitions.clear();
}
