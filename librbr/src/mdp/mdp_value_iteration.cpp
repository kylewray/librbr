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


#include "../../include/mdp/mdp_value_iteration.h"
#include "../../include/mdp/mdp_utilities.h"

#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"
#include "../../include/core/policy/policy_exception.h"

#include <math.h>

MDPValueIteration::MDPValueIteration()
{
	epsilon = 0.001;
}

MDPValueIteration::MDPValueIteration(double tolerance)
{
	epsilon = tolerance;
}

MDPValueIteration::~MDPValueIteration()
{ }

PolicyMap *MDPValueIteration::solve(const MDP *mdp)
{
	// Handle the trivial case.
	if (mdp == nullptr) {
		return nullptr;
	}

	// Attempt to convert the states object into StatesMap.
	const StatesMap *S = dynamic_cast<const StatesMap *>(mdp->get_states());
	if (S == nullptr) {
		throw StateException();
	}

	// Attempt to convert the actions object into ActionsMap.
	const ActionsMap *A = dynamic_cast<const ActionsMap *>(mdp->get_actions());
	if (A == nullptr) {
		throw ActionException();
	}

	// Attempt to convert the state transitions object into StateTransitions.
	const StateTransitions *T =
			dynamic_cast<const StateTransitions *>(mdp->get_state_transitions());
	if (T == nullptr) {
		throw StateTransitionException();
	}

	// Attempt to convert the rewards object into SASRewards.
	const SASRewards *R = dynamic_cast<const SASRewards *>(mdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	// Obtain the horizon and return the correct value iteration.
	const Horizon *h = mdp->get_horizon();
	if (h->is_finite()) {
		return solve_finite_horizon(S, A, T, R, h);
	} else {
		return solve_infinite_horizon(S, A, T, R, h);
	}
}

const std::unordered_map<const State *, double> &MDPValueIteration::get_V() const
{
	return V;
}

PolicyMap *MDPValueIteration::solve_finite_horizon(const StatesMap *S, const ActionsMap *A, const StateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	PolicyMap *policy = new PolicyMap(h);

	V.clear();

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	for (int t = h->get_horizon() - 1; t >= 0; t--){
		// For all the states, compute V(s).
		for (auto state : *S) {
			const State *s = resolve(state);
			const Action *aBest = nullptr;

			bellman_update(S, A, T, R, h, s, V, aBest);

			// Set the policy's action, which will yield the optimal policy at the end.
			policy->set(t, s, aBest);
		}
	}

	return policy;
}

PolicyMap *MDPValueIteration::solve_infinite_horizon(const StatesMap *S, const ActionsMap *A, const StateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	PolicyMap *policy = new PolicyMap(h);

	V.clear();

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	double convergenceCriterion = epsilon * (1.0 - h->get_discount_factor()) / h->get_discount_factor();
	double delta = convergenceCriterion + 1.0;

	while (delta > convergenceCriterion) {
		delta = 0.0;

		// For all the states, compute V(s).
		for (auto state : *S) {
			const State *s = resolve(state);
			const Action *aBest = nullptr;
			double Vs = V[s];

			// Perform the Bellman update, which modifies V and aBest such that V(s) = max Q(s, a)
			// and aBest = argmax Q(s, a).
			bellman_update(S, A, T, R, h, s, V, aBest);

			// Find the maximum difference, as part of our convergence criterion check.
			if (fabs(V[s] - Vs) > delta) {
				delta = fabs(V[s] - Vs);
			}

			// Set the policy's action, which will yield the optimal policy at the end.
			policy->set(s, aBest);
		}
	}

	return policy;
}
