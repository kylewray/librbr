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


#include "../../include/mdp/policy_iteration.h"
#include "../../include/mdp/mdp_utilities.h"

#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"
#include "../../include/core/policy/policy_exception.h"

#include <math.h>
#include <Eigen/Dense>

/**
 * The default constructor for the PolicyIteration class. It uses the exact
 * version of policy iteration by default.
 */
PolicyIteration::PolicyIteration()
{
	modifiedK = 0;
}

/**
 * A constructor for the PolicyIteration class. It uses the modified version of
 * policy iteration with a given k value.
 * @param k The number of iterations to compute an approximate set of value functions.
 */
PolicyIteration::PolicyIteration(unsigned int k)
{
	modifiedK = k;
}

/**
 * The deconstructor for the PolicyIteration class.
 */
PolicyIteration::~PolicyIteration()
{ }

/**
 * Solve the MDP provided using policy iteration.
 * @param mdp The Markov decision process to solve.
 * @return Return the optimal policy.
 * @throws StateException				The MDP did not have a FiniteStates states object.
 * @throws ActionException				The MDP did not have a FiniteActions actions object.
 * @throws StateTransitionsException	The MDP did not have a FiniteStateTransitions state transitions object.
 * @throws RewardException				The MDP did not have a SASRewards rewards object.
 * @throws PolicyException				An error occurred computing the policy.
 */
MapPolicy *PolicyIteration::solve(const MDP *mdp)
{
	// Handle the trivial case.
	if (mdp == nullptr) {
		return nullptr;
	}

	// Attempt to convert the states object into FiniteStates.
	const FiniteStates *S = dynamic_cast<const FiniteStates *>(mdp->get_states());
	if (S == nullptr) {
		throw StateException();
	}

	// Attempt to convert the actions object into FiniteActions.
	const FiniteActions *A = dynamic_cast<const FiniteActions *>(mdp->get_actions());
	if (A == nullptr) {
		throw ActionException();
	}

	// Attempt to convert the state transitions object into FiniteStateTransitions.
	const FiniteStateTransitions *T =
			dynamic_cast<const FiniteStateTransitions *>(mdp->get_state_transitions());
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

/**
 * Solve an infinite horizon MDP using exact policy iteration.
 * @param S The finite states.
 * @param A The finite actions.
 * @param T The finite state transition function.
 * @param R The state-action-state rewards.
 * @param h The horizon.
 * @return Return the optimal policy.
 * @throws PolicyException An error occurred computing the policy.
 */
MapPolicy *PolicyIteration::solve_exact(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	MapPolicy *policy = new MapPolicy(h);
	for (State *s : *S) {
		policy->set(s, A->get(0));
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
		for (State *si : *S) {
			unsigned int j = 0;
			for (State *sj : *S) {
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
		for (State *si : *S) {
			b(i) = 0.0;
			for (State *sj : *S) {
				b(i) -= T->get(si, policy->get(si), sj) * R->get(si, policy->get(si), sj);
			}
			i++;
		}

		// Solve the system of linear equations to find V(s) for all states s in S.
		Eigen::VectorXd x(S->get_num_states());
		x = M.colPivHouseholderQr().solve(b);

		// Store updates in V.
		std::map<State *, double> V;
		i = 0;
		for (State *s : *S) {
			V[s] = x(i);
			i++;
		}

		// Compute the action which maximizes the expected reward. During the computation, check if any
		// policy changes occur.
		i = 0;
		for (State *s : *S) {
			// Perform the Bellman update, but we only care about aBest = argmax Q(s, a).
			Action *aBest = nullptr;
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

/**
 * Solve an infinite horizon MDP using modified policy iteration.
 * @param S The finite states.
 * @param A The finite actions.
 * @param T The finite state transition function.
 * @param R The state-action-state rewards.
 * @param h The horizon.
 * @return Return the optimal policy.
 * @throws PolicyException An error occurred computing the policy.
 */
MapPolicy *PolicyIteration::solve_modified(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	MapPolicy *policy = new MapPolicy(h);

	// The value of the states, which will be constantly improved over iterations.
	std::map<State *, double> V;

	// Continue to iterate until the policy is unchanged in between two iterations.
	bool unchanged = false;

	while (!unchanged) {
		unchanged = true;

		// Continue to iterate a number of times equal to the constant k specified at initialization.
		for (int k = 0; k < modifiedK; k++) {
			// For all the states, compute V(s).
			for (State *s : *S) {
				Action *aBest = nullptr;

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
