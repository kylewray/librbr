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


#include "../../include/mdp/mdp_policy_iteration.h"
#include "../../include/mdp/mdp_utilities.h"

#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"
#include "../../include/core/policy/policy_exception.h"

#include <unordered_map>
#include <math.h>
#include <eigen3/Eigen/Dense>

MDPPolicyIteration::MDPPolicyIteration()
{
	modifiedK = 0;
}

MDPPolicyIteration::MDPPolicyIteration(unsigned int k)
{
	modifiedK = k;
}

MDPPolicyIteration::~MDPPolicyIteration()
{ }

PolicyMap *MDPPolicyIteration::solve(const MDP *mdp)
{
	// Handle the trivial case.
	if (mdp == nullptr) {
		return nullptr;
	}

	// Attempt to convert the states object into FiniteStates.
	const StatesMap *S = dynamic_cast<const StatesMap *>(mdp->get_states());
	if (S == nullptr) {
		throw StateException();
	}

	// Attempt to convert the actions object into FiniteActions.
	const ActionsMap *A = dynamic_cast<const ActionsMap *>(mdp->get_actions());
	if (A == nullptr) {
		throw ActionException();
	}

	// Attempt to convert the state transitions object into FiniteStateTransitions.
	const StateTransitionsMap *T =
			dynamic_cast<const StateTransitionsMap *>(mdp->get_state_transitions());
	if (T == nullptr) {
		throw StateTransitionException();
	}

	// Attempt to convert the rewards object into SASRewards.
	const SASRewards *R = dynamic_cast<const SASRewards *>(mdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	// Obtain the horizon and throw an error if it is not infinite.
	const Horizon *h = mdp->get_horizon();
	if (h->is_finite()) {
		throw PolicyException();
	}

	// Compute the optimal policy based on the desired version.
	if (modifiedK == 0) {
		return solve_exact(S, A, T, R, h);
	} else {
		return solve_modified(S, A, T, R, h);
	}
}

PolicyMap *MDPPolicyIteration::solve_exact(const StatesMap *S, const ActionsMap *A, const StateTransitionsMap *T,
		const SASRewards *R, const Horizon *h)
{
	PolicyMap *policy = new PolicyMap(h);
	for (auto s : *S) {
		policy->set(resolve(s), A->begin()->second);
	}

	// Create the M matrix with each cell M(i, j) denoting the i-th starting state s_i and
	// the j-th successor state s_j.
	Eigen::MatrixXd M(S->get_num_states(), S->get_num_states());

	// Create the b vector.
	Eigen::VectorXd b(S->get_num_states());

	// Continue to iterate until the policy is unchanged in between two iterations.
	bool unchanged = false;

	while (!unchanged) {
		unchanged = true;

		// Update the matrix M with the new policy changes.
		unsigned int i = 0;
		for (auto stateI : *S) {
			const State *si = resolve(stateI);

			unsigned int j = 0;
			for (auto stateJ : *S) {
				const State *sj = resolve(stateJ);

				M(i, j) = h->get_discount_factor() * T->get(si, policy->get(si), sj);

				// Handle the special case for the diagonal, which will be one less since we subtracted
				// the b vector (which contains V(s_i) itself), leaving b = 0 for our solver.
				if (i == j) {
					M(i, j) -= 1.0;
				}
				j++;
			}

			i++;
		}

		// Update the vector b with the new policy changes.
		i = 0;
		for (auto stateI : *S) {
			const State *si = resolve(stateI);

			b(i) = 0.0;
			for (auto stateJ : *S) {
				const State *sj = resolve(stateJ);
				b(i) -= T->get(si, policy->get(si), sj) * R->get(si, policy->get(si), sj);
			}

			i++;
		}

		// Solve the system of linear equations to find V(s) for all states s in S.
		Eigen::VectorXd x(S->get_num_states());
		x = M.colPivHouseholderQr().solve(b);

		// Store updates in V.
		std::unordered_map<const State *, double> V;
		i = 0;
		for (auto s : *S) {
			V[resolve(s)] = x(i);
			i++;
		}

		// Compute the action which maximizes the expected reward. During the computation, check if any
		// policy changes occur.
		i = 0;
		for (auto state : *S) {
			const State *s = resolve(state);

			// Perform the Bellman update, but we only care about aBest = argmax Q(s, a).
			const Action *aBest = nullptr;
			bellman_update(S, A, T, R, h, s, V, aBest);

			// Check if a policy change was required. Also, handle the initial case with an undefined mapping.
			if (policy->get(s) != aBest) {
				policy->set(s, aBest);
				unchanged = false;
			}
			i++;
		}
	}

	return policy;
}

PolicyMap *MDPPolicyIteration::solve_modified(const StatesMap *S, const ActionsMap *A, const StateTransitionsMap *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	PolicyMap *policy = new PolicyMap(h);

	// The value of the states, which will be constantly improved over iterations.
	std::unordered_map<const State *, double> V;

	// Continue to iterate until the policy is unchanged in between two iterations.
	bool unchanged = false;

	while (!unchanged) {
		unchanged = true;

		// Continue to iterate a number of times equal to the constant k specified at initialization.
		for (int k = 0; k < modifiedK; k++) {
			// For all the states, compute V(s).
			for (auto state : *S) {
				const State *s = resolve(state);

				const Action *aBest = nullptr;

				// Perform the Bellman update, which modifies V and aBest such that V(s) = max Q(s, a)
				// and aBest = argmax Q(s, a).
				bellman_update(S, A, T, R, h, s, V, aBest);

				// On the last iteration, check if we need to update any of the policy's actions. If we
				// do, then we are not done iterating. Otherwise, all actions remain constant and we
				// are done.
				if (k == modifiedK - 1) {
					// Also, handle the initial case with an undefined mapping.
					try {
						if (policy->get(s) != aBest) {
							policy->set(s, aBest);
							unchanged = false;
						}
					} catch (const PolicyException &err) {
						policy->set(s, aBest);
						unchanged = false;
					}
				}
			}
		}
	}

	return policy;
}
