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


#include "../../include/pomdp/pomdp.h"

/**
 * The default constructor for the POMDP class.
 */
POMDP::POMDP()
{
	MDP::MDP();

	observations = nullptr;
	observationTransitions = nullptr;
}

/**
 * A constructor for the POMDP class.
 * @param states					The states.
 * @param actions					The actions.
 * @param observations				The observations.
 * @param stateTransitions			The state transitions, which uses the states and actions parameters.
 * @param observationTransitions	The observation transitions, which uses the states, actions, and observations parameters.
 * @param rewards					The rewards, which uses the states and actions parameters.
 * @param initialState				The initial state, which uses the states parameter.
 * @param horizon					The horizon.
 */
POMDP::POMDP(States *s, Actions *a, Observations *o, StateTransitions *st, ObservationTransitions *ot,
		Rewards *r, InitialState *is, Horizon *h)
{
	MDP::MDP(s, a, st, r, is, h);

	observations = o;
	observationTransitions = ot;
}

/**
 * A deconstructor for the POMDP class.
 */
POMDP::~POMDP()
{
	MDP::~MDP();

	if (observations != nullptr) {
		delete observations;
	}
	if (observationTransitions != nullptr) {
		delete observationTransitions;
	}
}

/**
 * Get the observations object.
 * @return The observations object.
 */
const Observations *POMDP::get_observations() const
{
	return observations;
}

/**
 * Get the observation transitions object.
 * @return The observation transitions object.
 */
const ObservationTransitions *POMDP::get_observation_transitions() const
{
	return observationTransitions;
}