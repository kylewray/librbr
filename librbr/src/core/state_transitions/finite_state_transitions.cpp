/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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
#include "../../../include/core/state_transitions/state_transition_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"

/**
 * The default constructor for the FiniteStateTransitions class.
 */
FiniteStateTransitions::FiniteStateTransitions()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");
}

/**
 * The default deconstructor for the FiniteStateTransitions class.
 */
FiniteStateTransitions::~FiniteStateTransitions()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
}

/**
 * Set a state transition from a particular state-action-state triple to a probability.
 * @param state			The current state of the system.
 * @param action		The action taken at the current state.
 * @param nextState		The next state with which we assign the probability.
 * @param probability	The probability of going from the state, taking the action, then
 * 						moving to the nextState.
 */
void FiniteStateTransitions::set(const State *state, const Action *action, const State *nextState, double probability)
{
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}
	if (nextState == nullptr) {
		nextState = stateWildcard;
	}

	stateTransitions[state][action][nextState] = std::max(0.0, std::min(1.0, probability));
}

/**
 * The probability of a transition following the state-action-state triple provided.
 * @param state		The current state of the system.
 * @param action	The action taken at the current state.
 * @param nextState	The next state with which we assign the probability.
 * @return The probability of going from the state, taking the action, then moving to the nextState.
 */
double FiniteStateTransitions::get(const State *state, const Action *action, const State *nextState) const
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		const State *alpha = stateWildcard;
		if (i & (1 << 0)) {
			alpha = state;
		}

		const Action *beta = actionWildcard;
		if (i & (1 << 1)) {
			beta = action;
		}

		const State *gamma = stateWildcard;
		if (i & (1 << 2)) {
			gamma = nextState;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const StateTransitionException &err) { }
	}

	return 0.0;
}

/**
 * Return a list of the states available given a previous state and the action taken there.
 * @param S			The finite set of states.
 * @param state		The previous state.
 * @param action	The action taken at the previous state.
 * @param result	The list to overwrite and set to be the list of successor states.
 */
void FiniteStateTransitions::successors(const FiniteStates *S, const State *state,
		const Action *action, std::vector<const State *> &result) const
{
	result.clear();
	for (const State *nextState : *S) {
		if (get(state, action, nextState) > 0.0) {
			result.push_back(nextState);
		}
	}
}

/**
 * Reset the state transitions, clearing the internal mapping.
 */
void FiniteStateTransitions::reset()
{
	stateTransitions.clear();
}

/**
 * The actual get function which returns a value. This will throw an error if the value is undefined.
 * It is used as a helper function for the public get function.
 * @param state		The current state of the system.
 * @param action	The action taken at the current state.
 * @param nextState	The next state with which we assign the reward.
 * @return The probability of going from the state, taking the action, then moving to the nextState.
 * @throws StateTransitionException The state transition was not defined.
 */
double FiniteStateTransitions::get_value(const State *state, const Action *action, const State *nextState) const
{
	std::unordered_map<const State *,
		std::unordered_map<const Action *,
		std::unordered_map<const State *, double> > >::const_iterator alpha = stateTransitions.find(state);
	if (alpha == stateTransitions.end()) {
		throw StateTransitionException();
	}

	std::unordered_map<const Action *,
		std::unordered_map<const State *, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw StateTransitionException();
	}

	std::unordered_map<const State *, double>::const_iterator gamma = beta->second.find(nextState);
	if (gamma == beta->second.end()) {
		throw StateTransitionException();
	}

	return gamma->second;
}
