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


#ifndef MDP_UTILITIES_H
#define MDP_UTILITIES_H


#include <unordered_map>
#include <vector>

#include "../core/states/state.h"
#include "../core/states/states_map.h"
#include "../core/actions/actions_map.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/rewards/sas_rewards.h"
#include "../core/rewards/factored_rewards.h"
#include "../core/horizon.h"
#include "../core/policy/policy_map.h"

/**
 * Compute the Bellman update/backup for a given state and compute the action which achieves the
 * highest Q(s, a), and store the value and action in the variables provided.
 * @param	S 		The finite states.
 * @param	A 		The finite actions.
 * @param	T 		The finite state transition function.
 * @param	R 		The state-action-state rewards.
 * @param	h 		The horizon.
 * @param	s 		The current state being examined, i.e., V(s).
 * @param	V		The current Bellman backup, mapping states to expected rewards. This will be updated.
 * @param	aBest	The action which produced the maximum V(s) value. This will be updated.
 */
void bellman_update(StatesMap *S, ActionsMap *A, StateTransitions *T,
		SASRewards *R, Horizon *h, State *s,
		std::unordered_map<State *, double> &V, Action *&aBest);

/**
 * Compute the value of states (V^\pi), following the Bellman's equation, given a policy. This assumes
 * a single reward function.
 * @param	S 			The finite states.
 * @param	A 			The finite actions.
 * @param	T 			The finite state transition function.
 * @param	R 			The state-action-state rewards.
 * @param	h 			The horizon.
 * @param	epsilon		The tolerance for convergence.
 * @param	pi			The policy mapping states to actions.
 * @param	V			The resultant values, mapping states to expected rewards. This will be updated.
 */
void compute_V_pi(StatesMap *S, ActionsMap *A, StateTransitions *T, SASRewards *R, Horizon *h,
		double epsilon, PolicyMap *pi, std::unordered_map<State *, double> &V);

/**
 * Compute the value of states (V^\pi), following the Bellman's equation, given a policy. This assumes
 * a factored reward function, eaching having a SASReward.
 * @param	S 			The finite states.
 * @param	A 			The finite actions.
 * @param	T 			The finite state transition function.
 * @param	R 			The factored rewards, each being a state-action-state reward.
 * @param	h 			The horizon.
 * @param	epsilon		The tolerance for convergence.
 * @param	pi			The policy mapping states to actions.
 * @param	V			The resultant values, mapping states to expected rewards. This will be updated.
 */
void compute_V_pi(StatesMap *S, ActionsMap *A, StateTransitions *T, FactoredRewards *R, Horizon *h,
		double epsilon, PolicyMap *pi, std::vector<std::unordered_map<State *, double> > &V);


#endif // MDP_UTILITIES_H
