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


#include "../../include/pomdp/pomdp_value_iteration.h"
#include "../../include/pomdp/pomdp_utilities.h"

#include "../../include/core/policy/policy_alpha_vectors.h"
#include "../../include/core/policy/policy_alpha_vector.h"

#include "../../include/core/core_exception.h"
#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/observations/observation_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/observation_transitions/observation_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"
#include "../../include/core/policy/policy_exception.h"

#include <vector>
#include <math.h>

POMDPValueIteration::POMDPValueIteration()
{
	set_num_iterations(1);
}

POMDPValueIteration::POMDPValueIteration(unsigned int numIterations)
{
	set_num_iterations(numIterations);
}

POMDPValueIteration::~POMDPValueIteration()
{ }

void POMDPValueIteration::set_num_iterations(unsigned int numIterations)
{
	iterations = numIterations;
	if (iterations == 0) {
		iterations = 1;
	}
}

unsigned int POMDPValueIteration::get_num_iterations()
{
	return iterations;
}

void POMDPValueIteration::compute_num_iterations(POMDP *pomdp, double epsilon)
{
	// Handle the trivial case.
	if (pomdp == nullptr) {
		throw CoreException();
	}

	// Attempt to convert the rewards object into SASORewards.
	SASORewards *R = dynamic_cast<SASORewards *>(pomdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	Horizon *h = pomdp->get_horizon();

	// Make sure we do not take the log of 0.
	double Rmin = R->get_min();
	double Rmax = R->get_max();
	if (Rmax - Rmin < 0.000001) {
		Rmax = Rmin + 0.000001;
	}

	iterations = (int)((log(epsilon) - log(Rmax - Rmin)) / log(h->get_discount_factor()));
}

PolicyAlphaVectors *POMDPValueIteration::solve(POMDP *pomdp)
{
	// Handle the trivial case.
	if (pomdp == nullptr) {
		throw CoreException();
	}

	// Attempt to convert the states object into FiniteStates.
	StatesMap *S = dynamic_cast<StatesMap *>(pomdp->get_states());
	if (S == nullptr) {
		throw StateException();
	}

	// Attempt to convert the actions object into FiniteActions.
	ActionsMap *A = dynamic_cast<ActionsMap *>(pomdp->get_actions());
	if (A == nullptr) {
		throw ActionException();
	}

	// Attempt to convert the observations object into FiniteObservations.
	ObservationsMap *Z = dynamic_cast<ObservationsMap *>(pomdp->get_observations());
	if (Z == nullptr) {
		throw ObservationException();
	}

	// Attempt to convert the state transitions object into FiniteStateTransitions.
	StateTransitionsMap *T =
			dynamic_cast<StateTransitionsMap *>(pomdp->get_state_transitions());
	if (T == nullptr) {
		throw StateTransitionException();
	}

	// Attempt to convert the observation transitions object into FiniteObservationTransitions.
	ObservationTransitionsMap *O =
			dynamic_cast<ObservationTransitionsMap *>(pomdp->get_observation_transitions());
	if (O == nullptr) {
		throw ObservationTransitionException();
	}

	// Attempt to convert the rewards object into SASORewards.
	SASORewards *R = dynamic_cast<SASORewards *>(pomdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	// Obtain the horizon and return the correct value iteration.
	Horizon *h = pomdp->get_horizon();
	if (h->is_finite()) {
		return solve_finite_horizon(S, A, Z, T, O, R, h);
	} else {
		return solve_infinite_horizon(S, A, Z, T, O, R, h);
	}
}

PolicyAlphaVectors *POMDPValueIteration::solve_finite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
		StateTransitionsMap *T, ObservationTransitionsMap *O, SASORewards *R,
		Horizon *h)
{
	// Create the policy of alpha vectors variable. Set the horizon, to make the object's policy differ over time.
	PolicyAlphaVectors *policy = new PolicyAlphaVectors(h->get_horizon());

	// Before anything, cache Gamma_{a, *} for all actions. This is used in every cross-sum computation.
	std::map<Action *, std::vector<PolicyAlphaVector *> > gammaAStar;
	for (auto a : *A) {
		Action *action = resolve(a);
		gammaAStar[action].push_back(create_gamma_a_star(S, A, Z, T, O, R, action));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<PolicyAlphaVector *> gamma[2];
	bool current = false;

	// Continue to iterate until the horizon has been reached.
	for (unsigned int t = 0; t < h->get_horizon(); t++) {
		// Compute the new set of alpha vectors, gamma.
		for (auto a : *A) {
			Action *action = resolve(a);
			std::vector<PolicyAlphaVector *> alphaVector = bellman_update_cross_sum(S, A, Z, T, O, R, h,
					gammaAStar[action], gamma[!current], action);
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
	for (auto a : *A) {
		Action *action = resolve(a);
		for (PolicyAlphaVector *alphaVector : gammaAStar[action]) {
			delete alphaVector;
		}
		gammaAStar[action].clear();
	}
	gammaAStar.clear();

	return policy;
}

PolicyAlphaVectors *POMDPValueIteration::solve_infinite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
		StateTransitionsMap *T, ObservationTransitionsMap *O, SASORewards *R,
		Horizon *h)
{
	// Create the policy of alpha vectors variable. Set the horizon, to make the object's policy differ over time.
	PolicyAlphaVectors *policy = new PolicyAlphaVectors(h->get_horizon());

	// Before anything, cache Gamma_{a, *} for all actions. This is used in every cross-sum computation.
	std::map<Action *, std::vector<PolicyAlphaVector *> > gammaAStar;
	for (auto a : *A) {
		Action *action = resolve(a);
		gammaAStar[action].push_back(create_gamma_a_star(S, A, Z, T, O, R, action));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<PolicyAlphaVector *> gamma[2];
	bool current = false;

	// Continue to iterate until the number of iterations has been reached.
	for (unsigned int t = 0; t < iterations; t++) {
		// Compute the new set of alpha vectors, gamma.
		for (auto a : *A) {
			Action *action = resolve(a);
			std::vector<PolicyAlphaVector *> alphaVector = bellman_update_cross_sum(S, A, Z, T, O, R, h,
					gammaAStar[action], gamma[!current], action);
			gamma[current].insert(gamma[current].end(), alphaVector.begin(), alphaVector.end());
		}

		// Prune the dominated alpha vectors from the set.
		PolicyAlphaVectors::prune_dominated(S, gamma[current]);

		// Add the current gamma to the policy object, then call the prune_linear_program static method.
		// Note: This transfers the responsibility of memory management to the PolicyAlphaVectors object.
		policy->set(gamma[current]);

		// Prepare the next time step's gamma by clearing it. Remember again, we don't free the memory
		// because policy manages the previous time step's gamma (above).
		current = !current;
		gamma[current].clear();
	}

	// Free the memory of Gamma_{a, *}.
	for (auto a : *A) {
		Action *action = resolve(a);
		for (PolicyAlphaVector *alphaVector : gammaAStar[action]) {
			delete alphaVector;
		}
		gammaAStar[action].clear();
	}
	gammaAStar.clear();

	return policy;
}
