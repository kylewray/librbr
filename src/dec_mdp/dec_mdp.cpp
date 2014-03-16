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


#include "../../include/dec_mdp/dec_mdp.h"

/**
 * The default constructor for the DecMDP class.
 */
DecMDP::DecMDP() : MDP()
{
	agents = nullptr;
}

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
DecMDP::DecMDP(Agents *ag, States *s, Actions *a, StateTransitions *st, Rewards *r,
		Initial *is, Horizon *h) : MDP(s, a, st, r, is, h)
{
	agents = ag;
}

/**
 * A deconstructor for the DecMDP class.
 */
DecMDP::~DecMDP()
{
	if (agents != nullptr) {
		delete agents;
	}
}

/**
 * Get the agents object.
 * @return The agents object.
 */
const Agents *DecMDP::get_agents() const
{
	return agents;
}
