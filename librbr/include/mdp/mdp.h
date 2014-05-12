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


#ifndef MDP_H
#define MDP_H


#include "../core/states/states.h"
#include "../core/actions/actions.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/rewards/rewards.h"
#include "../core/initial.h"
#include "../core/horizon.h"

/**
 * A Markov Decision Process (MDP).
 */
class MDP {
public:
	/**
	 * The default constructor for the MDP class.
	 */
	MDP();

	/**
	 * A constructor for the MDP class.
	 * @param s		The states.
	 * @param a		The actions.
	 * @param st	The state transitions, which uses the states and actions parameters.
	 * @param r		The rewards, which uses the states and actions parameters.
	 * @param is	The initial state, which uses the states parameter.
	 * @param h		The horizon.
	 */
	MDP(States *s, Actions *a, StateTransitions *st, Rewards *r, Initial *is, Horizon *h);

	/**
	 * A deconstructor for the MDP class.
	 */
	virtual ~MDP();

	/**
	 * Get the states object.
	 * @return The states object.
	 */
	const States *get_states() const;

	/**
	 * Get the actions object.
	 * @return The actions object.
	 */
	const Actions *get_actions() const;

	/**
	 * Get the state transitions object.
	 * @return The state transitions object.
	 */
	const StateTransitions *get_state_transitions() const;

	/**
	 * Get the rewards object.
	 * @return The rewards object.
	 */
	const Rewards *get_rewards() const;

	/**
	 * Get the initial state object.
	 * @return The initial state object.
	 */
	const Initial *get_initial_state() const;

	/**
	 * Get the horizon object.
	 * @return The horizon object.
	 */
	const Horizon *get_horizon() const;

protected:
	/**
	 * The states in the MDP; e.g., an array of strings.
	 */
	States *states;

	/**
	 * The actions in the MDP; e.g., an array of strings.
	 */
	Actions *actions;

	/**
	 * The state transition function in the MDP; e.g., a three-dimensional array mapping to a double.
	 */
	StateTransitions *stateTransitions;

	/**
	 * The reward function in the MDP; e.g., a two-dimensional array mapping to a double.
	 */
	Rewards *rewards;

	/**
	 * The initial state or initial belief state; e.g., factored initial state.
	 */
	Initial *initialState;

	/**
	 * The horizon, either a finite time or a discount factor.
	 */
	Horizon *horizon;

};


#endif // MDP_H
