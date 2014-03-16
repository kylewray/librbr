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


#ifndef DEC_MDP_H
#define DEC_MDP_H


#include "../core/agents/agents.h"
#include "../core/states/states.h"
#include "../core/actions/actions.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/rewards/rewards.h"
#include "../core/initial.h"
#include "../core/horizon.h"

#include "../mdp/mdp.h"

/**
 * A Decentralized Markov Decision Process (MDP).
 */
class DecMDP : public MDP {
public:
	/**
	 * The default constructor for the DecMDP class.
	 */
	DecMDP();

	/**
	 * A constructor for the DecMDP class.
	 * @param ag	The agents.
	 * @param s		The states.
	 * @param a		The actions, which uses the agents parameter.
	 * @param st	The state transitions, which uses the states and actions parameters.
	 * @param r		The rewards, which uses the states and actions parameters.
	 * @param is	The initial state, which uses the states parameter.
	 * @param h		The horizon.
	 */
	DecMDP(Agents *ag, States *s, Actions *a, StateTransitions *st, Rewards *r, Initial *is, Horizon *h);

	/**
	 * A deconstructor for the DecMDP class.
	 */
	virtual ~DecMDP();

	/**
	 * Get the agents object.
	 * @return The agents object.
	 */
	const Agents *get_agents() const;

protected:
	/**
	 * The agents in the Dec-MDP; e.g., a vector of strings.
	 */
	Agents *agents;

};


#endif // DEC_MDP_H
