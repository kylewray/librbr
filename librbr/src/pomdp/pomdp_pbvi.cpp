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


#include "../../include/pomdp/pomdp_pbvi.h"
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
#include <random>

/**
 * The default constructor for the POMDPPBVI class. Default number of iterations for infinite
 * horizon POMDPs is 1. The default expansion rule is Random Belief Selection.
 */
POMDPPBVI::POMDPPBVI()
{
	set_expansion_rule(POMDPPBVIExpansionRule::RANDOM_BELIEF_SELECTION);
	set_num_update_iterations(1);
	set_num_expansion_iterations(1);
}

/**
 * A constructor for the POMDPPBVI class which allows for the specification of the expansion rule,
 * and the number of iterations (both updates and expansions) to run for infinite horizon.
 * The default is 1 for both.
 * @param expansionRule			The expansion rule to use.
 * @param updateIterations 		The number of update iterations to run for infinite horizon POMDPs.
 * @param expansionIterations 	The number of expansion iterations to run for infinite horizon POMDPs.
 */
POMDPPBVI::POMDPPBVI(POMDPPBVIExpansionRule expansionRule, unsigned int updateIterations, unsigned int expansionIterations)
{
	set_expansion_rule(expansionRule);
	set_num_update_iterations(updateIterations);
	set_num_expansion_iterations(expansionIterations);
}

/**
 * The deconstructor for the POMDPPBVI class. This method frees the belief state memory.
 */
POMDPPBVI::~POMDPPBVI()
{
	// Free the memory of the initial belief states, as well as the belief states computed
	// as part of the solver.
	reset();
}

/**
 * Add an initial belief state which is used to seed the belief states before computing the optimal policy.
 * Note: This relinquishes control of the belief state's memory management to this class.
 * @param b The initial set of belief states before calling 'solve'.
 */
void POMDPPBVI::add_initial_belief_state(const BeliefState *b)
{
	if (b == nullptr) {
		return;
	}
	initialB.push_back(b);
}

/**
 * Set the initial set of belief states which are used to seed the belief states before computing
 * the optimal policy. Note: This relinquishes control of the belief states' memory management to this class.
 * @param initialB The initial set of belief states before calling 'solve'.
 */
void POMDPPBVI::set_initial_belief_states(const std::vector<const BeliefState *> &initialBeliefStates)
{
	initialB = initialBeliefStates;
}

/**
 * Set the expansion rule to add belief points.
 * @param expansionRule The expansion rule to use.
 */
void POMDPPBVI::set_expansion_rule(POMDPPBVIExpansionRule expansionRule)
{
	rule = expansionRule;
}

/**
 * Set the number of update iterations to run for infinite horizon POMDPs.
 * @param iterations The number of update iterations to run for infinite horizon POMDPs.
 */
void POMDPPBVI::set_num_update_iterations(unsigned int iterations)
{
	updates = iterations;
	if (updates == 0) {
		updates = 1;
	}
}

/**
 * Set the number of expansion iterations to run for infinite horizon POMDPs.
 * @param iterations The number of expansion iterations to run for infinite horizon POMDPs.
 */
void POMDPPBVI::set_num_expansion_iterations(unsigned int iterations)
{
	expansions = iterations;
	if (expansions == 0) {
		expansions = 1;
	}
}

/**
 * Get the initial set of belief states which are used to seed the belief states before computing
 * the optimal policy.
 * @return The initial set of belief states before calling 'solve'.
 */
const std::vector<const BeliefState *> &POMDPPBVI::get_initial_belief_states() const
{
	return initialB;
}

/**
 * Set the expansion rule to add belief points.
 * @param expansionRule The expansion rule to use.
 */
POMDPPBVIExpansionRule POMDPPBVI::get_expansion_rule() const
{
	return rule;
}

/**
 * Get the number of update iterations to run for infinite horizon POMDPs.
 * @return The number of update iterations to run for infinite horizon POMDPs.
 */
unsigned int POMDPPBVI::get_num_update_iterations() const
{
	return updates;
}

/**
 * Get the number of expansion iterations to run for infinite horizon POMDPs.
 * @return The number of expansion iterations to run for infinite horizon POMDPs.
 */
unsigned int POMDPPBVI::get_num_expansion_iterations() const
{
	return expansions;
}

/**
 * Get the set of belief states which were used to compute the optimal policy. This vector is only
 * populated after calling 'solve'.
 * @return The final set of belief states after calling 'solve'.
 */
const std::vector<const BeliefState *> &POMDPPBVI::get_belief_states() const
{
	return B;
}

/**
 * Compute the optimal number of update iterations to run for infinite horizon POMDPs, given
 * the desired tolerance, requiring knowledge of the reward function.
 * @param pomdp 			The partially observable Markov decision process to use.
 * @param epsilon			The desired tolerance between value functions to check for convergence.
 * @throws RewardException	The POMDP did not have a SASORewards rewards object.
 */
void POMDPPBVI::compute_num_update_iterations(const POMDP *pomdp, double epsilon)
{
	// Handle the trivial case.
	if (pomdp == nullptr) {
		throw CoreException();
	}

	// Attempt to convert the rewards object into SASORewards.
	const SASORewards *R = dynamic_cast<const SASORewards *>(pomdp->get_rewards());
	if (R == nullptr) {
		throw RewardException();
	}

	const Horizon *h = pomdp->get_horizon();

	// Make sure we do not take the log of 0.
	double Rmin = R->get_min();
	double Rmax = R->get_max();
	if (Rmax - Rmin < 0.000001) {
		Rmax = Rmin + 0.000001;
	}

	updates = (int)((log(epsilon) - log(Rmax - Rmin)) / log(h->get_discount_factor()));
}

/**
 * Solve the POMDP provided using point-based value iteration.
 * @param pomdp The partially observable Markov decision process to solve.
 * @return Return the optimal policy as a finite state controller (infinite horizon) or tree (finite horizon).
 * @throws CoreException					The POMDP was null.
 * @throws StateException					The POMDP did not have a FiniteStates states object.
 * @throws ActionException					The POMDP did not have a FiniteActions actions object.
 * @throws ObservationException				The POMDP did not have a FiniteObservations observations object.
 * @throws StateTransitionsException		The POMDP did not have a FiniteStateTransitions state transitions object.
 * @throws ObservationTransitionsException	The POMDP did not have a FiniteObservationTransitions observation transitions object.
 * @throws RewardException					The POMDP did not have a SASRewards rewards object.
 * @throws PolicyException					An error occurred computing the policy.
 */
PolicyAlphaVectors *POMDPPBVI::solve(const POMDP *pomdp)
{
	// Handle the trivial case.
	if (pomdp == nullptr) {
		throw CoreException();
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
 * Reset this POMDP PBVI solver. This method frees all the belief state memory.
 */
void POMDPPBVI::reset() {
	for (const BeliefState *b : initialB) {
		if (b != nullptr) {
			delete b;
		}
	}
	initialB.clear();

	for (const BeliefState *b : B) {
		if (b != nullptr) {
			delete b;
		}
	}
	B.clear();
}

/**
 * Solve a finite horizon POMDP using point-based value iteration.
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
PolicyAlphaVectors *POMDPPBVI::solve_finite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
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

	// Initialize the set of belief points to be the initial set. This must be a copy, since memory is managed
	// for both objects independently.
	for (const BeliefState *b : initialB) {
		B.push_back(new BeliefState(*b));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<PolicyAlphaVector *> gamma[2];
	bool current = false;

	// Initialize the first set Gamma to be a set of zero alpha vectors.
	for (int i = 0; i < B.size(); i++) {
		PolicyAlphaVector *zeroAlphaVector = new PolicyAlphaVector();
		for (const State *s : *S) {
			zeroAlphaVector->set(s, 0.0);
		}
		gamma[!current].push_back(zeroAlphaVector);
	}

	// Continue to iterate until the horizon has been reached.
	for (int t = 0; t < h->get_horizon(); t++){
		// For each of the belief points, we must compute the optimal alpha vector.
		for (const BeliefState *belief : B) {
			PolicyAlphaVector *maxAlphaB = nullptr;
			double maxAlphaDotBeta = 0.0;

			// Compute the optimal alpha vector for this belief state.
			for (const Action *action : *A) {
				PolicyAlphaVector *alphaBA = bellman_update_belief_state(S, A, Z, T, O, R, h,
						gammaAStar[action], gamma[!current], action, belief);

				double alphaDotBeta = alphaBA->compute_value(belief);
				if (maxAlphaB == nullptr || alphaDotBeta > maxAlphaDotBeta) {
					// This is the maximal alpha vector, so delete the old one.
					if (maxAlphaB != nullptr) {
						delete maxAlphaB;
					}
					maxAlphaB = alphaBA;
					maxAlphaDotBeta = alphaDotBeta;
				} else {
					// This was not the maximal alpha vector, so delete it.
					delete alphaBA;
				}
			}

			gamma[current].push_back(maxAlphaB);
		}

		// Add the current gamma to the policy object. Note: This transfers the responsibility of
		// memory management to the PolicyAlphaVectors object.
		policy->set(t, gamma[current]);

		// Prepare the next time step's gamma by clearing it. Remember again, we don't free the memory
		// because policy manages the previous time step's gamma (above). If this is the first horizon,
		// however, we actually do need to clear the set of zero alpha vectors.
		current = !current;
		if (h == 0) {
			for (PolicyAlphaVector *zeroAlphaVector : gamma[current]) {
				delete zeroAlphaVector;
			}
		}
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
 * Solve an infinite horizon POMDP using point-based value iteration.
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
PolicyAlphaVectors *POMDPPBVI::solve_infinite_horizon(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
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

	// Initialize the set of belief points to be the initial set. This must be a copy, since memory is managed
	// for both objects independently.
	for (const BeliefState *b : initialB) {
		B.push_back(new BeliefState(*b));
	}

	// Create the set of alpha vectors, which we call Gamma. As well as the previous Gamma set.
	std::vector<PolicyAlphaVector *> gamma[2];
	bool current = false;

	// Initialize the first set Gamma to be a set of zero alpha vectors.
	for (int i = 0; i < B.size(); i++) {
		PolicyAlphaVector *zeroAlphaVector = new PolicyAlphaVector();
		for (const State *s : *S) {
			zeroAlphaVector->set(s, 0.0);
		}
		gamma[!current].push_back(zeroAlphaVector);
	}

	// Perform a predefined number of expansions. Each update adds more belief points to the set B.
	for (int e = 0; e < expansions; e++) {
		// Perform a predefined number of updates. Each update improves the value function estimate.
		for (int u = 0; u < updates; u++){
			// For each of the belief points, we must compute the optimal alpha vector.
			for (const BeliefState *belief : B) {
				PolicyAlphaVector *maxAlphaB = nullptr;
				double maxAlphaDotBeta = 0.0;

				// Compute the optimal alpha vector for this belief state.
				for (const Action *action : *A) {
					PolicyAlphaVector *alphaBA = bellman_update_belief_state(S, A, Z, T, O, R, h,
							gammaAStar[action], gamma[!current], action, belief);

					double alphaDotBeta = alphaBA->compute_value(belief);
					if (maxAlphaB == nullptr || alphaDotBeta > maxAlphaDotBeta) {
						// This is the maximal alpha vector, so delete the old one.
						if (maxAlphaB != nullptr) {
							delete maxAlphaB;
						}
						maxAlphaB = alphaBA;
						maxAlphaDotBeta = alphaDotBeta;
					} else {
						// This was not the maximal alpha vector, so delete it.
						delete alphaBA;
					}
				}

				gamma[current].push_back(maxAlphaB);
			}

			// Prepare the next time step's gamma by clearing it. Remember again, we don't free the memory
			// because policy manages the previous time step's gamma (above). If this is the first horizon,
			// however, we actually do need to clear the set of zero alpha vectors.
			current = !current;
			for (PolicyAlphaVector *zeroAlphaVector : gamma[current]) {
				delete zeroAlphaVector;
			}
			gamma[current].clear();
		}

		// Perform an expansion based on the rule the user wishes to use.
		switch (rule) {
		case POMDPPBVIExpansionRule::RANDOM_BELIEF_SELECTION:
			expand_random_belief_selection(S);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_RANDOM_ACTION:
			expand_stochastic_simulation_random_actions();
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_GREEDY_ACTION:
			expand_stochastic_simulation_greedy_action();
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_EXPLORATORY_ACTION:
			expand_stochastic_simulation_exploratory_action();
			break;
		case POMDPPBVIExpansionRule::GREEDY_ERROR_REDUCTION:
			expand_greedy_error_reduction();
			break;
		default:
			throw PolicyException();
			break;
		};
	}

	// Set the current gamma to the policy object. Note: This transfers the responsibility of
	// memory management to the PolicyAlphaVectors object.
	policy->set(gamma[!current]);

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

#include <iostream>

/**
 * Expand the set of beliefs following Random Belief Selection.
 *
 */
void POMDPPBVI::expand_random_belief_selection(const FiniteStates *S)
{
	std::vector<const BeliefState *> Bnew;
	for (int b = 0; b < B.size(); b++) {
		std::vector<double> bTmp;
		bTmp.resize(S->get_num_states());

		for (int i = 0; i < S->get_num_states(); i++) {
			bTmp[i] = (double)rand() / (double)RAND_MAX;
		}

		std::sort(bTmp.begin(), bTmp.end(), std::less<double>());

		BeliefState *bNew = new BeliefState();

		int i = 0;
		double sum = 0.0;
		for (const State *s : *S) {
			if (i + 1 < S->get_num_states()) {
				double val = bTmp[i + 1] - bTmp[i];
				sum += val;
				bNew->set(s, val);
			} else {
				bNew->set(s, 1.0 - sum);
			}
			i++;
		}

		Bnew.push_back(bNew);
	}

	B.insert(B.end(), Bnew.begin(), Bnew.end());

//	std::cout << "New Size of B = " << B.size() << std::endl;
//	for (const BeliefState *b : B) {
//		std::cout << b->get(S->get(0)) << "  " << b->get(S->get(1)) << std::endl;
//	}
}

/**
 * Expand the set of beliefs following Stochastic Simulation with Random Actions.
 */
void POMDPPBVI::expand_stochastic_simulation_random_actions()
{

}

/**
 * Expand the set of beliefs following Stochastic Simulation with Greedy Action.
 */
void POMDPPBVI::expand_stochastic_simulation_greedy_action()
{

}

/**
 * Expand the set of beliefs following Stochastic Simulation with Exploratory Action.
 */
void POMDPPBVI::expand_stochastic_simulation_exploratory_action()
{

}

/**
 * Expand the set of beliefs following Greedy Error Reduction.
 */
void POMDPPBVI::expand_greedy_error_reduction()
{

}
