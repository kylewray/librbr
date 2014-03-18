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


#ifndef POMDP_UTILITIES_H
#define POMDP_UTILITIES_H


#include "../../include/core/states/state.h"
#include "../../include/core/states/belief_state.h"
#include "../../include/core/actions/action.h"
#include "../../include/core/observations/observation.h"

#include "../../include/core/states/finite_states.h"
#include "../../include/core/actions/finite_actions.h"
#include "../../include/core/observations/finite_observations.h"
#include "../../include/core/state_transitions/finite_state_transitions.h"
#include "../../include/core/observation_transitions/finite_observation_transitions.h"
#include "../../include/core/rewards/sas_rewards.h"
#include "../../include/core/horizon.h"

#include "../../include/pomdp/pomdp_alpha_vector.h"

/**
 * Create the commonly used Gamma_{a,*}.
 * @param S			The set of finite states.
 * @param A			The set of finite actions.
 * @param T			The finite state transition function.
 * @param R			The state-action-state rewards function.
 * @param action	The action taken at this time step.
 * @return The alpha vector which is the single element in Gamma_{a,*}.
 */
std::vector<POMDPAlphaVector *> create_gamma_a_star(const FiniteStates *S, const FiniteActions *A,
		const FiniteStateTransitions *T, const SASRewards *R, const Action *action);

/**
 * Perform the belief state update equation on the current belief.
 * @param S				The finite states object.
 * @param T				The finite state transition function.
 * @param O				The finite observation transition function.
 * @param belief		The current belief state.
 * @param action		The action taken in the current belief state.
 * @param observation	The observation observed after taking the action in the current belief state.
 * @return The resultant new belief state.
 */
BeliefState belief_state_update(const BeliefState &belief, const FiniteStates *S, const FiniteStateTransitions *T,
		const FiniteObservationTransitions *O, const Action *action, const Observation *observation);

/**
 * Compute the Bellman update/backup for a given belief state. Since the value function V' = HV, is over the belief state
 * space, and we can represent the value function approximately as a PWLC set of alpha vectors, it returns this set of
 * alpha vectors.
 * @param S 		 The finite states.
 * @param A 		 The finite actions.
 * @param Z			 The finite observations.
 * @param T 		 The finite state transition function.
 * @param O			 The finite observation transition function.
 * @param R 		 The state-action-state rewards.
 * @param h 		 The horizon.
 * @param action	 The action taken.
 * @param gammaAStar The initial gamma which is always used in the cross sum: Gamma_{a,*}.
 * @param gamma		 The current Bellman backup, represented as the set Gamma storing alpha-vectors.
 * @return The next Gamma which contains the new set of optimal alpha-vectors.
 */
std::vector<POMDPAlphaVector *> bellman_update(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASRewards *R,
		const Horizon *h, const Action *action, const std::vector<POMDPAlphaVector *> &gammaAStar,
		const std::vector<POMDPAlphaVector *> &gamma);


#endif // POMDP_UTILITIES_H
