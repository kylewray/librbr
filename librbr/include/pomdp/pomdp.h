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


#ifndef POMDP_H
#define POMDP_H


#include "../core/states/states.h"
#include "../core/actions/actions.h"
#include "../core/observations/observations.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/observation_transitions/observation_transitions.h"
#include "../core/rewards/rewards.h"
#include "../core/initial.h"
#include "../core/horizon.h"

#include "../mdp/mdp.h"

/**
 * A Partially Observable Markov Decision Process (POMDP).
 */
class POMDP : public MDP {
public:
	/**
	 * The default constructor for the POMDP class.
	 */
	POMDP();

	/**
	 * A constructor for the POMDP class.
	 * @param	S		The states.
	 * @param	A		The actions.
	 * @param	Z		The observations.
	 * @param	T		The state transitions, which uses the states and actions parameters.
	 * @param	O		The observation transitions, which uses the states, actions, and observations parameters.
	 * @param	R		The rewards, which uses the states and actions parameters.
	 * @param	s		The initial state, which uses the states parameter.
	 * @param	h		The horizon.
	 */
	POMDP(States *S, Actions *A, Observations *Z, StateTransitions *T, ObservationTransitions *O,
			Rewards *R, Initial *s, Horizon *h);

	/**
	 * A deconstructor for the POMDP class.
	 */
	virtual ~POMDP();

	/**
	 * Get the observations object.
	 * @return	The observations object.
	 */
	const Observations *get_observations() const;

	/**
	 * Get the observation transitions object.
	 * @return	The observation transitions object.
	 */
	const ObservationTransitions *get_observation_transitions() const;

protected:
	/**
	 * The observations in the POMDP; e.g., factored vectors of strings.
	 */
	Observations *observations;

	/**
	 * The observation transition function in the POMDP; e.g., a three-dimensional array mapping to a double.
	 */
	ObservationTransitions *observationTransitions;

};


#endif // POMDP_H
