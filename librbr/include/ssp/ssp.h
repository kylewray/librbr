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


#ifndef SSP_H
#define SSP_H


#include "../mdp/mdp.h"

#include "../core/states/states.h"
#include "../core/actions/actions.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/rewards/rewards.h"
#include "../core/initial.h"
#include "../core/horizon.h"

#include <vector>

/**
 * A Stochastic Control Process (SSP), which is a more general form of an MDP.
 */
class SSP : public MDP {
public:
	/**
	 * The default constructor for the SSP class.
	 */
	SSP();

	/**
	 * A constructor for the SSP class.
	 * @param	S		The states.
	 * @param	A		The actions.
	 * @param	T		The state transitions, which uses the states and actions parameters.
	 * @param	R		The rewards, which uses the states and actions parameters.
	 * @param	h		The horizon.
	 */
	SSP(States *S, Actions *A, StateTransitions *T, Rewards *R, Horizon *h, State *s0, std::vector<State *> G);

	/**
	 * A deconstructor for the SSP class.
	 */
	virtual ~SSP();

	/**
	 * Get the initial state object.
	 * @return	The initial state object.
	 */
	State *get_initial_state();

	/**
	 * Get the goal states.
	 * @return	The goal states as a vector.
	 */
	std::vector<State *> get_goal_states();

protected:
	/**
	 * The initial state of the SSP.
	 */
	State *initialState;

	/**
	 * The set of goal states of the SSP.
	 */
	std::vector<State *> goalStates;

};


#endif // SSP_H
