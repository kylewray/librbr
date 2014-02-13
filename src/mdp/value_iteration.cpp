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


#include "../../include/mdp/value_iteration.h"

#include "../../include/core/policy/policy_exception.h"
#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"

#include <math.h>
#include <limits>

/**
 * The default constructor for the ValueIteration class.
 */
ValueIteration::ValueIteration()
{
	epsilon = 0.001;
}

/**
 * A constructor for the ValueIteration class which allows for the specification
 * of the convergence criterion.
 * @param tolerance The tolerance which determines convergence of value iteration.
 */
ValueIteration::ValueIteration(double tolerance)
{
	epsilon = tolerance;
}

/**
 * The deconstructor for the ValueIteration class.
 */
ValueIteration::~ValueIteration()
{ }

/**
 * Solve the MDP provided using value iteration.
 * @param mdp The Markov decision process to solve.
 * @return Return the optimal policy.
 * @throws StateException				The MDP did not have a FiniteStates states object.
 * @throws ActionException				The MDP did not have a FiniteActions actions object.
 * @throws StateTransitionsException	The MDP did not have a FiniteStateTransitions state transitions object.
 * @throws RewardException				The MDP did not have a SASRewards rewards object.
 * @throws PolicyException				An error occurred computing the policy.
 */
MapPolicy *ValueIteration::solve(const MDP *mdp)
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

	// Obtain the horizon and return the correct value iteration.
	const Horizon *h = mdp->get_horizon();
	if (h->is_finite()) {
		return solve_finite_horizon(S, A, T, R, h);
	} else {
		return solve_infinite_horizon(S, A, T, R, h);
	}
}

/**
 * Solve an finite horizon MDP using value iteration.
 * @param S The finite states.
 * @param A The finite actions.
 * @param T The finite state transition function.
 * @param R The state-action-state rewards.
 * @param h The horizon.
 * @return Return the optimal policy.
 * @throws PolicyException An error occurred computing the policy.
 */
MapPolicy *ValueIteration::solve_finite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	MapPolicy *policy = new MapPolicy(h);

	// The value of a states and state's actions.
	std::map<State *, double> V;
	std::map<State *, std::map<Action *, double> > Q;

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	for (int t = 0; t < h->get_horizon(); t++){
		// For all the states, compute V[s].
		for (State *s : S->all()) {
			Action *aBest = nullptr;
			double Vs = std::numeric_limits<double>::lowest();

			// For all the actions, compute Q[s][a].
			for (Action *a : A->available(s)) {
				// Compute the Q(s, a) estimate.
				Q[s][a] = 0.0;
				for (State *sPrime : S->all()) {
					Q[s][a] += T->get(s, a, sPrime) * (R->get(s, a, sPrime) + V[sPrime]);
				}
				Q[s][a] = Q[s][a] * h->get_discount_factor();

				// While we are looping over actions, find the maximum.
				if (Q[s][a] > Vs) {
					Vs = Q[s][a];
					aBest = a;
				}
			}

			// Set the value of the state.
			V[s] = Vs;

			// Set the policy's action, which will yield the optimal policy at the end.
			policy->set(t, s, aBest);
		}
	}

	return policy;
}

#include <iostream>
/**
 * Solve an infinite horizon MDP using value iteration.
 * @param S The finite states.
 * @param A The finite actions.
 * @param T The finite state transition function.
 * @param R The state-action-state rewards.
 * @param h The horizon.
 * @return Return the optimal policy.
 * @throws PolicyException An error occurred computing the policy.
 */
MapPolicy *ValueIteration::solve_infinite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
		const SASRewards *R, const Horizon *h)
{
	// Create the policy based on the horizon.
	MapPolicy *policy = new MapPolicy(h);

	// The value of a states and state's actions.
	std::map<State *, double> V;
	std::map<State *, std::map<Action *, double> > Q;

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	double difference = epsilon + 1.0;
	while (difference > epsilon) {
		difference = 0.0;

		// For all the states, compute V[s].
		for (State *s : S->all()) {
			Action *aBest = nullptr;
			double Vs = std::numeric_limits<double>::lowest();

			// For all the actions, compute Q[s][a].
			for (Action *a : A->available(s)) {
				// Compute the Q(s, a) estimate.
				Q[s][a] = 0.0;
				for (State *sPrime : S->all()) {
					Q[s][a] += T->get(s, a, sPrime) * (R->get(s, a, sPrime) + V[sPrime]);
				}
				Q[s][a] = Q[s][a] * h->get_discount_factor();

				// While we are looping over actions, find the maximum.
				if (Q[s][a] > Vs) {
					Vs = Q[s][a];
					aBest = a;
				}
			}

			// Find the maximum difference, as part of our convergence criterion check.
			if (fabs(Vs - V[s]) > difference) {
				difference = fabs(Vs - V[s]);
			}

			// Set the value of the state.
			V[s] = Vs;

			// Set the policy's action, which will yield the optimal policy at the end.
			policy->set(s, aBest);
		}
	}

	return policy;
}
