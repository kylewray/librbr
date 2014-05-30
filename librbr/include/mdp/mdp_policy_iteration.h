/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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


#ifndef POLICY_ITERATION_H
#define POLICY_ITERATION_H


#include "mdp.h"

#include "../core/policy/policy_map.h"

#include "../core/states/finite_states.h"
#include "../core/actions/finite_actions.h"
#include "../core/state_transitions/finite_state_transitions.h"
#include "../core/rewards/sas_rewards.h"
#include "../core/horizon.h"

/**
 * Solve an MDP via policy iteration (infinite horizon) with either the exact or modified
 * version. This solver has the following requirements:
 * - MDP states must be of type FiniteStates.
 * - MDP actions must be of type FiniteActions.
 * - MDP state transitions must be of type FiniteStateTransitions.
 * - MDP rewards must be of type SASRewards.
 */
class MDPPolicyIteration {
public:
	/**
	 * The default constructor for the PolicyIteration class. It uses the exact
	 * version of policy iteration by default.
	 */
	MDPPolicyIteration();

	/**
	 * A constructor for the PolicyIteration class. It uses the modified version of
	 * policy iteration with a given k value.
	 * @param	k	The number of iterations to compute an approximate set of value functions.
	 */
	MDPPolicyIteration(unsigned int k);

	/**
	 * The deconstructor for the PolicyIteration class.
	 */
	virtual ~MDPPolicyIteration();

	/**
	 * Solve the MDP provided using policy iteration.
	 * @param	mdp							The Markov decision process to solve.
	 * @throw	StateException				The MDP did not have a FiniteStates states object.
	 * @throw	ActionException				The MDP did not have a FiniteActions actions object.
	 * @throw	StateTransitionsException	The MDP did not have a FiniteStateTransitions state transitions object.
	 * @throw	RewardException				The MDP did not have a SASRewards rewards object.
	 * @throw	PolicyException				An error occurred computing the policy.
	 * @return	Return the optimal policy.
	 */
	PolicyMap *solve(const MDP *mdp);

private:
	/**
	 * Solve an infinite horizon MDP using exact policy iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	T					The finite state transition function.
	 * @param	R					The state-action-state rewards.
	 * @param	h					The horizon.
	 * @throw	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy.
	 */
	PolicyMap *solve_exact(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
			const SASRewards *R, const Horizon *h);

	/**
	 * Solve an infinite horizon MDP using modified policy iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	T					The finite state transition function.
	 * @param	R					The state-action-state rewards.
	 * @param	h					The horizon.
	 * @throws	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy.
	 */
	PolicyMap *solve_modified(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
			const SASRewards *R, const Horizon *h);

	/**
	 * The number of times to iterate in the modified version. Disable the modified version of
	 * policy iteration by setting k = 0.
	 */
	unsigned int modifiedK;

};


#endif // POLICY_ITERATION_H
