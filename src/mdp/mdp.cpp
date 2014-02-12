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


#include "../../include/mdp/mdp.h"

/**
 * The default constructor for the MDP class.
 */
MDP::MDP()
{
	states = nullptr;
	actions = nullptr;
	stateTransitions = nullptr;
	rewards = nullptr;
	initialState = nullptr;
	horizon = nullptr;
}

/**
 * A constructor for the MDP class.
 * @param s		The states.
 * @param a		The actions.
 * @param st	The state transitions, which uses the states and actions parameters.
 * @param r		The rewards, which uses the states and actions parameters.
 * @param is	The initial state, which uses the states parameter.
 * @param h		The horizon.
 */
MDP::MDP(States *s, Actions *a, StateTransitions *st, Rewards *r, InitialState *is, Horizon *h)
{
	states = s;
	actions = a;
	stateTransitions = st;
	rewards = r;
	initialState = is;
	horizon = h;
}

/**
 * A deconstructor for the MDP class. This frees the memory of all MDP variables.
 */
MDP::~MDP()
{
	if (states != nullptr) {
		delete states;
	}
	if (actions != nullptr) {
		delete actions;
	}
	if (stateTransitions != nullptr) {
		delete stateTransitions;
	}
	if (rewards != nullptr) {
		delete rewards;
	}
	if (initialState != nullptr) {
		delete initialState;
	}
	if (horizon != nullptr) {
		delete horizon;
	}
}

/**
 * Get the states object.
 * @return The states object.
 */
const States *MDP::get_states() const
{
	return states;
}

/**
 * Get the actions object.
 * @return The actions object.
 */
const Actions *MDP::get_actions() const
{
	return actions;
}

/**
 * Get the state transitions object.
 * @return The state transitions object.
 */
const StateTransitions *MDP::get_state_transitions() const
{
	return stateTransitions;
}

/**
 * Get the rewards object.
 * @return The rewards object.
 */
const Rewards *MDP::get_rewards() const
{
	return rewards;
}

/**
 * Get the initial state object.
 * @return The initial state object.
 */
const InitialState *MDP::get_initial_state() const
{
	return initialState;
}

/**
 * Get the horizon object.
 * @return The horizon object.
 */
const Horizon *MDP::get_horizon() const
{
	return horizon;
}
