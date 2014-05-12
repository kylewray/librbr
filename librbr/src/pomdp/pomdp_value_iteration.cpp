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


#include "../../include/pomdp/pomdp_value_iteration.h"
#include "../../include/pomdp/pomdp_utilities.h"

#include "../../include/core/policy/policy_alpha_vectors.h"
#include "../../include/core/policy/policy_alpha_vector.h"

#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/observations/observation_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/observation_transitions/observation_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"
#include "../../include/core/policy/policy_exception.h"

#include <vector>
#include <math.h>

/**
 * The default constructor for the POMDPValueIteration class.
 */
POMDPValueIteration::POMDPValueIteration()
{
	epsilon = 0.001;
}

/**
 * A constructor for the POMDPValueIteration class which allows for the specification
 * of the convergence criterion.
 * @param tolerance The tolerance which determines convergence of value iteration.
 */
POMDPValueIteration::POMDPValueIteration(double tolerance)
{
	epsilon = tolerance;
}

/**
 * The deconstructor for the POMDPValueIteration class.
 */
POMDPValueIteration::~POMDPValueIteration()
{ }

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
PolicyAlphaVectors *POMDPValueIteration::solve(const POMDP *pomdp)
{
	// Handle the trivial case.
	if (pomdp == nullptr) {
		return nullptr;
	}

	// Attempt to convert the states object into FiniteStates.
	const FiniteStates *S = dynamic_cast<const FiniteStates *>(pomdp->get_states());
	if (S == nullptr) {
		throw StateException();
	}

	// Attempt to convert the actions object into FiniteActions.
	const FiniteActions *A = dynamic_cast<const FiniteActions *>(pomdp->get_actions());
	if (A == nullptr) {
		throw ActionException();
	}

	// Attempt to convert the observations object into FiniteObservations.
	const FiniteObservations *Z = dynamic_cast<const FiniteObservations *>(pomdp->get_observations());
	if (Z == nullptr) {
		throw ObservationException();
	}

	// Attempt to convert the state transitions object into FiniteStateTransitions.
	const FiniteStateTransitions *T =
			dynamic_cast<const FiniteStateTransitions *>(pomdp->get_state_transitions());
	if (T == nullptr) {
		throw StateTransitionException();
	}

	// Attempt to convert the observation transitions object into FiniteObservationTransitions.
	const FiniteObservationTransitions *O =
			dynamic_cast<const FiniteObservationTransitions *>(pomdp->get_observation_transitions());
	if (O == nullptr) {
		throw ObservationTransitionException();
	}

	// Attempt to convert the rewards object into SASORewards.
	const SASORewards *R = dynamic_cast<const SASORewards *>(pomdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	// Obtain the horizon and return the correct value iteration.
	const Horizon *h = pomdp->get_horizon();
	if (h->is_finite()) {
		return solve_finite_horizon(S, A, Z, T, O, R, h);
	} else {
		return solve_infinite_horizon(S, A, Z, T, O, R, h);
	}
}

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
PolicyAlphaVectors *POMDPValueIteration::solve_finite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
		const Horizon *h)
{
	// Create the policy of alpha vectors variable. Set the horizon, to make the object's policy differ over time.
	PolicyAlphaVectors *policy = new PolicyAlphaVectors(h->get_horizon());

	// Before anything, cache Gamma_{a, *} for all actions. This is used in every cross-sum computation.
	std::map<const Action *, std::vector<PolicyAlphaVector *> > gammaAStar;
	for (const Action *action : *A) {
		gammaAStar[action].push_back(create_gamma_a_star(S, A, Z, T, O, R, action));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<PolicyAlphaVector *> gamma[2];
	bool current = false;

	// Continue to iterate until the horizon has been reached.
	for (int t = 0; t < h->get_horizon(); t++){
		// Compute the new set of alpha vectors, gamma.
		for (const Action *action : *A) {
			std::vector<PolicyAlphaVector *> alphaVector = bellman_update(S, A, Z, T, O, R, h,
					action, gammaAStar[action], gamma[!current]);
			gamma[current].insert(gamma[current].end(), alphaVector.begin(), alphaVector.end());
		}

		// Prune the dominated alpha vectors from the set.
		PolicyAlphaVectors::prune_dominated(S, gamma[current]);

		// Add the current gamma to the policy object, then call the prune_linear_program static method.
		// Note: This transfers the responsibility of memory management to the PolicyAlphaVectors object.
		policy->set(t, gamma[current]);

		// Prepare the next time step's gamma by clearing it. Remember again, we don't free the memory
		// because policy manages the previous time step's gamma (above).
		current = !current;
		gamma[current].clear();
	}

	// Free the memory of Gamma_{a, *}.
	for (const Action *action : *A) {
		for (PolicyAlphaVector *alphaVector : gammaAStar[action]) {
			delete alphaVector;
		}
		gammaAStar[action].clear();
	}
	gammaAStar.clear();

	return policy;
}

/**
 * Solve an infinite horizon POMDP using value iteration.
 * @param S The finite states.
 * @param A The finite actions.
 * @param Z The finite observations.
 * @param T The finite state transition function.
 * @param O The finite observation transition function.
 * @param R The state-action-state-observation rewards.
 * @param h The horizon.
 * @return Return the optimal policy as a finite state controller.
 * @throws PolicyException An error occurred computing the policy.
 */
PolicyAlphaVectors *POMDPValueIteration::solve_infinite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
		const Horizon *h)
{
	return nullptr;

	/*
	// Before anything, cache Gamma_{a, *} for all actions. This is used in every cross-sum computation.
	std::map<const Action *, std::vector<POMDPAlphaVector *> > gammaAStar;
	for (const Action *action : *A) {
		gammaAStar[action].push_back(create_gamma_a_star(S, A, Z, T, O, R, action));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<POMDPAlphaVector *> gamma[2];
	bool current = false;

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	double convergenceCriterion = epsilon * (1.0 - h->get_discount_factor()) / h->get_discount_factor();
	double delta = convergenceCriterion + 1.0;

	while (delta > convergenceCriterion) {
		delta = 0.0;

		// Compute the new set of alpha vectors, gamma.
		for (const Action *action : *A) {
			std::vector<POMDPAlphaVector *> alphaVector = bellman_update(S, A, Z, T, O, R, h,
					action, gammaAStar[action], gamma[current]);
			gamma[current].insert(gamma[current].end(), alphaVector.begin(), alphaVector.end());
		}

		// Prune alpha vectors by solving a linear program.
//		OsiSolverInterface *si = new OsiClpSolverInterface();
//
//		si
//
//		delete si;

		// NOTE: Don't forget to swap "current" variable!
		// NOTE: Don't forget to free memory of the NEW current after you swap the variable!

		// TODO: Compute delta by looking at the max_{s in S} |max_{alpha in gamma} alpha[s] - max_{alpha in gammaPrev} gammaPrev[s]|.
		// HACK: TERMINATE IMMEDIATELY!
//		delta = ???;
	}

	// NOTE: Don't forget to free the memory of the PREVIOUS gamma (i.e., gamma[!current]), and then return/use gamma[current].

	// Free the memory of Gamma_{a, *}.
	for (const Action *action : *A) {
		for (POMDPAlphaVector *alphaVector : gammaAStar[action]) {
			delete alphaVector;
		}
		gammaAStar[action].clear();
	}
	gammaAStar.clear();

	return nullptr;
	*/






	/*
	// Create the policy based on the horizon.
	PolicyFSC *policy = new PolicyFSC(h);

	// The value of the states.
	std::map<State *, double> V;

	// Continue to iterate until the maximum difference between two V[s]'s is less than the tolerance.
	double convergenceCriterion = epsilon * (1.0 - h->get_discount_factor()) / h->get_discount_factor();
	double delta = convergenceCriterion + 1.0;

	while (delta > convergenceCriterion) {
		delta = 0.0;

		// For all the states, compute V(s).
		for (State *s : *S) {
			Action *aBest = nullptr;
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
	*/
}
