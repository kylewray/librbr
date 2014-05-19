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


#include "../../include/dec_pomdp/dec_pomdp.h"

/**
 * The default constructor for the DecPOMDP class.
 */
DecPOMDP::DecPOMDP() : POMDP()
{
	agents = nullptr;
}

/**
 * A constructor for the DecPOMDP class.
 * @param ag	The agents.
 * @param s		The states.
 * @param a		The actions.
 * @param o		The observations.
 * @param st	The state transitions, which uses the states and actions parameters.
 * @param ot	The observation transitions, which uses the states, actions, and observations parameters.
 * @param r		The rewards, which uses the states and actions parameters.
 * @param is	The initial state, which uses the states parameter.
 * @param h		The horizon.
 */
DecPOMDP::DecPOMDP(Agents *ag, States *s, Actions *a, Observations *o, StateTransitions *st, ObservationTransitions *ot,
		Rewards *r, Initial *is, Horizon *h) : POMDP(s, a, o, st, ot, r, is, h)
{
	agents = ag;
}

/**
 * The deconstructor for the DecPOMDP class.
 */
DecPOMDP::~DecPOMDP()
{
	if (agents == nullptr) {
		delete agents;
	}
}

/**
 * Get the agents object.
 * @return The agents object.
 */
const Agents *DecPOMDP::get_agents() const
{
	return agents;
}
