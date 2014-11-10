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


#ifndef POMDP_VALUE_ITERATION_H
#define POMDP_VALUE_ITERATION_H


#include "pomdp.h"

#include "../core/policy/policy_alpha_vectors.h"
#include "../core/policy/policy_alpha_vector.h"

#include "../core/states/states_map.h"
#include "../core/actions/actions_map.h"
#include "../core/observations/observations_map.h"
#include "../core/state_transitions/state_transitions_map.h"
#include "../core/observation_transitions/observation_transitions_map.h"
#include "../core/rewards/saso_rewards.h"
#include "../core/horizon.h"

/**
 * Solve an POMDP via value iteration (finite or infinite horizon). This solver has the
 * following requirements:
 * - POMDP states must be of type FiniteStates.
 * - POMDP actions must be of type FiniteActions.
 * - POMDP observations must be of type FiniteObservations.
 * - POMDP state transitions must be of type FiniteStateTransitions.
 * - POMDP observation transitions must be of type FiniteObservationTransitions.
 * - POMDP rewards must be of type SASRewards.
 */
class POMDPValueIteration {
public:
	/**
	 * The default constructor for the POMDPValueIteration class. Default number of iterations
	 * for infinite horizon POMDPs is 1.
	 */
	POMDPValueIteration();

	/**
	 * A constructor for the POMDPValueIteration class which allows for the specification
	 * of the number of iterations to run for infinite horizon. Default is 1.
	 * @param	numIterations	The number of iterations to run for infinite horizon POMDPs.
	 */
	POMDPValueIteration(unsigned int numIterations);

	/**
	 * The deconstructor for the POMDPValueIteration class.
	 */
	virtual ~POMDPValueIteration();

	/**
	 * Set the number of iterations to run for infinite horizon POMDPs.
	 * @param	numIterations	The number of iterations to run for infinite horizon POMDPs.
	 */
	void set_num_iterations(unsigned int numIterations);

	/**
	 * Get the number of iterations to run for infinite horizon POMDPs.
	 * @return	The number of iterations to run for infinite horizon POMDPs.
	 */
	unsigned int get_num_iterations();

	/**
	 * Compute the optimal number of iterations to run for infinite horizon POMDPs, given
	 * the desired tolerance, requiring knowledge of the reward function.
	 * @param	pomdp 				The partially observable Markov decision process to use.
	 * @param	epsilon				The desired tolerance between value functions to check for convergence.
	 * @throw	RewardException		The POMDP did not have a SASORewards rewards object.
	 */
	void compute_num_iterations(POMDP *pomdp, double epsilon);

	/**
	 * Solve the POMDP provided using value iteration.
	 * @param	pomdp							The partially observable Markov decision process to solve.
	 * @throw	CoreException					The POMDP was null.
	 * @throw	StateException					The POMDP did not have a FiniteStates states object.
	 * @throw	ActionException					The POMDP did not have a FiniteActions actions object.
	 * @throw	ObservationException			The POMDP did not have a FiniteObservations observations object.
	 * @throw	StateTransitionsException		The POMDP did not have a FiniteStateTransitions state transitions object.
	 * @throw	ObservationTransitionsException	The POMDP did not have a FiniteObservationTransitions observation transitions object.
	 * @throw	RewardException					The POMDP did not have a SASORewards rewards object.
	 * @throw	PolicyException					An error occurred computing the policy.
	 * @return	Return the optimal policy as a finite state controller (infinite horizon) or tree (finite horizon).
	 */
	PolicyAlphaVectors *solve(POMDP *pomdp);

private:
	/**
	 * Solve a finite horizon POMDP using value iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	Z					The finite observations.
	 * @param	T					The finite state transition function.
	 * @param	O					The finite observation transition function.
	 * @param	R					The state-action-state-observation rewards.
	 * @param	h					The horizon.
	 * @throw	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy as a collection of alpha vectors.
	 */
	PolicyAlphaVectors *solve_finite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitionsMap *T, ObservationTransitionsMap *O, SASORewards *R,
			Horizon *h);

	/**
	 * Solve an infinite horizon POMDP using value iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	Z					The finite observations.
	 * @param	T					The finite state transition function.
	 * @param	O					The finite observation transition function.
	 * @param	R					The state-action-state-observation rewards.
	 * @param	h					The horizon.
	 * @throw	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy as a collection of alpha vectors.
	 */
	PolicyAlphaVectors *solve_infinite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitionsMap *T, ObservationTransitionsMap *O, SASORewards *R,
			Horizon *h);

	/**
	 * The number of iterations until the solver stops for infinite horizon POMDPs.
	 */
	unsigned int iterations;

};


#endif // POMDP_VALUE_ITERATION_H
