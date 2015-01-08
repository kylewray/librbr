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


#ifndef POMDP_UTILITIES_H
#define POMDP_UTILITIES_H


#include <vector>

#include "../core/states/state.h"
#include "../core/states/belief_state.h"
#include "../core/actions/action.h"
#include "../core/observations/observation.h"

#include "../core/states/states_map.h"
#include "../core/actions/actions_map.h"
#include "../core/observations/observations_map.h"
#include "../core/state_transitions/state_transitions_map.h"
#include "../core/observation_transitions/observation_transitions_map.h"
#include "../core/rewards/rewards.h"
#include "../core/horizon.h"

#include "../core/policy/policy_alpha_vector.h"

/**
 * Create the commonly used Gamma_{a,*}.
 * @param	S					The set of finite states.
 * @param	Z					The set of finite observations.
 * @param	T					The finite state transition function.
 * @param	O					The finite observation transition function.
 * @param	R					The state-action rewards function.
 * @param	action				The action taken at this time step.
 * @return	The alpha vector which is the single element in Gamma_{a,*}.
 * @throw	RewardException		The rewards argument was invalid.
 */
PolicyAlphaVector *create_gamma_a_star(StatesMap *S,
		ObservationsMap *Z, StateTransitions *T, ObservationTransitions *O,
		Rewards *R, Action *action);

/**
 * Perform the belief state update equation on the current belief. This creates a new belief state in memory.
 * @param	S				The finite states object.
 * @param	T				The finite state transition function.
 * @param	O				The finite observation transition function.
 * @param	belief			The current belief state.
 * @param	action			The action taken in the current belief state.
 * @param	observation		The observation observed after taking the action in the current belief state.
 * @return	The resultant new belief state.
 */
BeliefState *belief_state_update(StatesMap *S, StateTransitions *T,
		ObservationTransitions *O, BeliefState *belief, Action *action,
		Observation *observation);

/**
 * Compute the Bellman update/backup using the cross sum operation, fully expanding all possible alpha vectors. Since the value
 * function V' = HV, is over the belief state space, and we can represent the value function approximately as a PWLC set of alpha
 * vectors, it returns this set of alpha vectors.
 * @param	S 		 	The finite states.
 * @param	Z			The finite observations.
 * @param	T 		 	The finite state transition function.
 * @param	O			The finite observation transition function.
 * @param	h 			The horizon.
 * @param	gammaAStar	The initial gamma which is always used in the cross sum: Gamma_{a,*}.
 * @param	gamma		The current Bellman backup, represented as the set Gamma storing alpha-vectors.
 * @param	action	 	The action taken.
 * @return	The Gamma_{a} which contains the new set of optimal alpha-vectors, given a particular action.
 */
std::vector<PolicyAlphaVector *> bellman_update_cross_sum(StatesMap *S, ObservationsMap *Z,
		StateTransitions *T, ObservationTransitions *O,
		Horizon *h, std::vector<PolicyAlphaVector *> &gammaAStar, std::vector<PolicyAlphaVector *> &gamma,
		Action *action);

/**
 * Compute the Bellman update/backup for one specific belief state, returning the new optimal alpha-vector for this belief state. In
 * this case, we assume an action has been taken to construct the new alpha vector.
 * @param	S 		 	The finite states.
 * @param	Z			The finite observations.
 * @param	T 		 	The finite state transition function.
 * @param	O			The finite observation transition function.
 * @param	h 		 	The horizon.
 * @param	gammaAStar	The initial gamma which is always used in the cross sum: Gamma_{a,*}.
 * @param	gamma		The current Bellman backup, represented as the set Gamma storing alpha-vectors.
 * @param	action	 	The action taken.
 * @param	b			The belief state for which to compute the updated alpha vector.
 * @return	The optimal alpha-vector at this belief state, given a particular action.
 */
PolicyAlphaVector *bellman_update_belief_state(StatesMap *S, ObservationsMap *Z,
		StateTransitions *T, ObservationTransitions *O,
		Horizon *h, std::vector<PolicyAlphaVector *> &gammaAStar, std::vector<PolicyAlphaVector *> &gamma,
		Action *action, BeliefState *b);


#endif // POMDP_UTILITIES_H
