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


#ifndef POMDP_VALUE_ITERATION_H
#define POMDP_VALUE_ITERATION_H


#include "pomdp.h"

#include "../core/policy/policy_alpha_vectors.h"
#include "../core/policy/policy_alpha_vector.h"

#include "../core/states/finite_states.h"
#include "../core/actions/finite_actions.h"
#include "../core/observations/finite_observations.h"
#include "../core/state_transitions/finite_state_transitions.h"
#include "../core/observation_transitions/finite_observation_transitions.h"
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
	 * The default constructor for the POMDPValueIteration class. The default tolerance is 0.001.
	 */
	POMDPValueIteration();

	/**
	 * A constructor for the POMDPValueIteration class which allows for the specification
	 * of the convergence criterion (tolerance).
	 * @param tolerance The tolerance which determines convergence of value iteration.
	 */
	POMDPValueIteration(double tolerance);

	/**
	 * The deconstructor for the POMDPValueIteration class.
	 */
	virtual ~POMDPValueIteration();

	/**
	 * Solve the POMDP provided using value iteration.
	 * @param pomdp The partially observable Markov decision process to solve.
	 * @return Return the optimal policy as a finite state controller (infinite horizon) or tree (finite horizon).
	 * @throws StateException					The POMDP did not have a FiniteStates states object.
	 * @throws ActionException					The POMDP did not have a FiniteActions actions object.
	 * @throws ObservationException				The POMDP did not have a FiniteObservations observations object.
	 * @throws StateTransitionsException		The POMDP did not have a FiniteStateTransitions state transitions object.
	 * @throws ObservationTransitionsException	The POMDP did not have a FiniteObservationTransitions observation transitions object.
	 * @throws RewardException					The POMDP did not have a SASRewards rewards object.
	 * @throws PolicyException					An error occurred computing the policy.
	 */
	PolicyAlphaVectors *solve(const POMDP *pomdp);

private:
	/**
	 * Solve a finite horizon POMDP using value iteration.
	 * @param S The finite states.
	 * @param A The finite actions.
	 * @param Z The finite observations.
	 * @param T The finite state transition function.
	 * @param O The finite observation transition function.
	 * @param R The state-action-state-observation rewards.
	 * @param h The horizon.
	 * @return Return the optimal policy as a collection of alpha vectors.
	 * @throws PolicyException An error occurred computing the policy.
	 */
	PolicyAlphaVectors *solve_finite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
			const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
			const Horizon *h);

	/**
	 * Solve an infinite horizon POMDP using value iteration.
	 * @param S The finite states.
	 * @param A The finite actions.
	 * @param Z The finite observations.
	 * @param T The finite state transition function.
	 * @param O The finite observation transition function.
	 * @param R The state-action-state-observation rewards.
	 * @param h The horizon.
	 * @return Return the optimal policy as a collection of alpha vectors.
	 * @throws PolicyException An error occurred computing the policy.
	 */
	PolicyAlphaVectors *solve_infinite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
			const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
			const Horizon *h);

	/**
	 * The tolerance convergence criterion.
	 */
	double epsilon;

};


#endif // POMDP_VALUE_ITERATION_H
