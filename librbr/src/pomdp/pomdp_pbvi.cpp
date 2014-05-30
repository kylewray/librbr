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

POMDPPBVI::POMDPPBVI()
{
	set_expansion_rule(POMDPPBVIExpansionRule::RANDOM_BELIEF_SELECTION);
	set_num_update_iterations(1);
	set_num_expansion_iterations(1);
}

POMDPPBVI::POMDPPBVI(POMDPPBVIExpansionRule expansionRule, unsigned int updateIterations, unsigned int expansionIterations)
{
	set_expansion_rule(expansionRule);
	set_num_update_iterations(updateIterations);
	set_num_expansion_iterations(expansionIterations);
}

POMDPPBVI::~POMDPPBVI()
{
	// Free the memory of the initial belief states, as well as the belief states computed
	// as part of the solver.
	reset();
}

void POMDPPBVI::add_initial_belief_state(const BeliefState *b)
{
	if (b == nullptr) {
		return;
	}
	initialB.push_back(b);
}

void POMDPPBVI::set_initial_belief_states(const std::vector<const BeliefState *> &initialBeliefStates)
{
	initialB = initialBeliefStates;
}

void POMDPPBVI::set_expansion_rule(POMDPPBVIExpansionRule expansionRule)
{
	rule = expansionRule;
}

void POMDPPBVI::set_num_update_iterations(unsigned int iterations)
{
	updates = iterations;
	if (updates == 0) {
		updates = 1;
	}
}

void POMDPPBVI::set_num_expansion_iterations(unsigned int iterations)
{
	expansions = iterations;
	if (expansions == 0) {
		expansions = 1;
	}
}

const std::vector<const BeliefState *> &POMDPPBVI::get_initial_belief_states() const
{
	return initialB;
}

POMDPPBVIExpansionRule POMDPPBVI::get_expansion_rule() const
{
	return rule;
}

unsigned int POMDPPBVI::get_num_update_iterations() const
{
	return updates;
}

unsigned int POMDPPBVI::get_num_expansion_iterations() const
{
	return expansions;
}

const std::vector<const BeliefState *> &POMDPPBVI::get_belief_states() const
{
	return B;
}

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
		for (auto s : *S) {
			zeroAlphaVector->set(resolve(s), 0.0);
		}
		gamma[!current].push_back(zeroAlphaVector);
	}

	// Perform a predefined number of expansions. Each update adds more belief points to the set B.
	for (int e = 0; e < expansions; e++) {
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

		// Perform an expansion based on the rule the user wishes to use.
		switch (rule) {
		case POMDPPBVIExpansionRule::NONE:
			e = expansions; // Stop immediately if the user does not want to expand.
			break;
		case POMDPPBVIExpansionRule::RANDOM_BELIEF_SELECTION:
			expand_random_belief_selection(S);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_RANDOM_ACTION:
			expand_stochastic_simulation_random_actions(S, A, Z, T, O);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_GREEDY_ACTION:
			expand_stochastic_simulation_greedy_action(S, A, Z, T, O, gamma[!current]);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_EXPLORATORY_ACTION:
			expand_stochastic_simulation_exploratory_action(S, A, Z, T, O);
			break;
		case POMDPPBVIExpansionRule::GREEDY_ERROR_REDUCTION:
			expand_greedy_error_reduction();
			break;
		default:
			throw PolicyException();
			break;
		};
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
		for (auto s : *S) {
			zeroAlphaVector->set(resolve(s), 0.0);
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
		case POMDPPBVIExpansionRule::NONE:
			e = expansions; // Stop immediately if the user does not want to expand.
			break;
		case POMDPPBVIExpansionRule::RANDOM_BELIEF_SELECTION:
			expand_random_belief_selection(S);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_RANDOM_ACTION:
			expand_stochastic_simulation_random_actions(S, A, Z, T, O);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_GREEDY_ACTION:
			expand_stochastic_simulation_greedy_action(S, A, Z, T, O, gamma[!current]);
			break;
		case POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_EXPLORATORY_ACTION:
			expand_stochastic_simulation_exploratory_action(S, A, Z, T, O);
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
		for (auto s : *S) {
			if (i + 1 < S->get_num_states()) {
				double val = bTmp[i + 1] - bTmp[i];
				sum += val;
				bNew->set(resolve(s), val);
			} else {
				bNew->set(resolve(s), 1.0 - sum);
			}
			i++;
		}

		Bnew.push_back(bNew);
	}

	B.insert(B.end(), Bnew.begin(), Bnew.end());
}

void POMDPPBVI::expand_stochastic_simulation_random_actions(const FiniteStates *S, const FiniteActions *A,
		const FiniteObservations *Z, const FiniteStateTransitions *T, const FiniteObservationTransitions *O)
{
	std::vector<const BeliefState *> Bnew;

	for (const BeliefState *b : B) {
		// Randomly select the state following Multinomial(b).
		const State *state = nullptr;

		double rnd = (double)rand() / (double)RAND_MAX;
		double sum = 0.0f;

		for (auto s : *S) {
			sum += b->get(resolve(s));
			if (sum >= rnd) {
				state = resolve(s);
				break;
			}
		}

		// Randomly select the action following Uniform(A).
		const Action *action = nullptr;

		rnd = (double)((int)rand() % A->get_num_actions());
		sum = 0.0;

		for (const Action *a : *A) {
			sum += 1.0;
			if (sum >= rnd) {
				action = a;
				break;
			}
		}

		// Randomly select the state following Multinomial(T(s, a, *)).
		const State *nextState = nullptr;

		rnd = (double)rand() / (double)RAND_MAX;
		sum = 0.0f;

		for (auto s : *S) {
			sum += T->get(state, action, resolve(s));
			if (sum >= rnd) {
				nextState = resolve(s);
				break;
			}
		}

		// Randomly select the state following Multinomial(O(a, s', *)).
		const Observation *observation = nullptr;

		rnd = (double)rand() / (double)RAND_MAX;
		sum = 0.0f;

		for (const Observation *z : *Z) {
			sum += O->get(action, nextState, z);
			if (sum >= rnd) {
				observation = z;
				break;
			}
		}

		BeliefState *bNew = belief_state_update(S, T, O, b, action, observation);
		Bnew.push_back(bNew);
	}

	B.insert(B.end(), Bnew.begin(), Bnew.end());
}

void POMDPPBVI::expand_stochastic_simulation_greedy_action(const FiniteStates *S, const FiniteActions *A,
		const FiniteObservations *Z, const FiniteStateTransitions *T, const FiniteObservationTransitions *O,
		const std::vector<PolicyAlphaVector *> &gamma)
{
	std::vector<const BeliefState *> Bnew;

	for (const BeliefState *b : B) {
		// Randomly select the state following Multinomial(b).
		const State *state = nullptr;

		double rnd = (double)rand() / (double)RAND_MAX;
		double sum = 0.0f;

		for (auto s : *S) {
			sum += b->get(resolve(s));
			if (sum >= rnd) {
				state = resolve(s);
				break;
			}
		}

		// Randomly select the action following Uniform(A) with probability epsilon. Or, with probability
		// 1 - epsilon, select the 'optimal' action.
		const Action *action = nullptr;

		// Assume epsilon is 0.1.
		if ((int)rand() % 10 == 0) {
			// Explore the actions.
			rnd = (double)((int)rand() % A->get_num_actions());
			sum = 0.0;

			for (const Action *a : *A) {
				sum += 1.0;
				if (sum >= rnd) {
					action = a;
					break;
				}
			}
		} else {
			// Optimize the action.
			double maxVal = std::numeric_limits<double>::lowest();
			for (const PolicyAlphaVector *alpha : gamma) {
				double val = alpha->compute_value(b);
				if (val > maxVal) {
					maxVal = val;
					action = alpha->get_action();
				}
			}
		}

		// Randomly select the state following Multinomial(T(s, a, *)).
		const State *nextState = nullptr;

		rnd = (double)rand() / (double)RAND_MAX;
		sum = 0.0f;

		for (auto s : *S) {
			sum += T->get(state, action, resolve(s));
			if (sum >= rnd) {
				nextState = resolve(s);
				break;
			}
		}

		// Randomly select the state following Multinomial(O(a, s', *)).
		const Observation *observation = nullptr;

		rnd = (double)rand() / (double)RAND_MAX;
		sum = 0.0f;

		for (const Observation *z : *Z) {
			sum += O->get(action, nextState, z);
			if (sum >= rnd) {
				observation = z;
				break;
			}
		}

		BeliefState *bNew = belief_state_update(S, T, O, b, action, observation);
		Bnew.push_back(bNew);
	}

	B.insert(B.end(), Bnew.begin(), Bnew.end());
}

void POMDPPBVI::expand_stochastic_simulation_exploratory_action(const FiniteStates *S, const FiniteActions *A,
		const FiniteObservations *Z, const FiniteStateTransitions *T, const FiniteObservationTransitions *O)
{
	std::vector<const BeliefState *> Bnew;

	for (const BeliefState *b : B) {
		BeliefState *bNew = nullptr;
		double bVal = std::numeric_limits<double>::lowest();

		// For each action, we will randomly generate a belief.
		for (const Action *action : *A) {
			// Randomly select the state following Multinomial(b).
			const State *state = nullptr;

			double rnd = (double)rand() / (double)RAND_MAX;
			double sum = 0.0f;

			for (auto s : *S) {
				sum += b->get(resolve(s));
				if (sum >= rnd) {
					state = resolve(s);
					break;
				}
			}

			// Randomly select the state following Multinomial(T(s, a, *)).
			const State *nextState = nullptr;

			rnd = (double)rand() / (double)RAND_MAX;
			sum = 0.0f;

			for (auto s : *S) {
				sum += T->get(state, action, resolve(s));
				if (sum >= rnd) {
					nextState = resolve(s);
					break;
				}
			}

			// Randomly select the state following Multinomial(O(a, s', *)).
			const Observation *observation = nullptr;

			rnd = (double)rand() / (double)RAND_MAX;
			sum = 0.0f;

			for (const Observation *z : *Z) {
				sum += O->get(action, nextState, z);
				if (sum >= rnd) {
					observation = z;
					break;
				}
			}

			// Compute the belief given this action, and the other selected items.
			BeliefState *ba = belief_state_update(S, T, O, b, action, observation);

			// Compute the min over the belief points, finding the 1-norm between the ba and all
			// other belief points possible.
			double baMin = std::numeric_limits<double>::lowest() * -1.0;
			for (const BeliefState *bp : B) {
				sum = 0.0;
				for (auto s : *S) {
					sum += std::fabs(ba->get(resolve(s)) - bp->get(resolve(s)));
				}
				if (sum < baMin) {
					baMin = sum;
				}
			}
			for (const BeliefState *bp : Bnew) {
				sum = 0.0;
				for (auto s : *S) {
					sum += std::fabs(ba->get(resolve(s)) - bp->get(resolve(s)));
				}
				if (sum < baMin) {
					baMin = sum;
				}
			}

			// Finally, we are computing the max over the actions. Thus, if this is a better max,
			// then store it, otherwise destroy it.
			if (baMin > bVal) {
				bNew = ba;
				bVal = baMin;
			} else {
				delete ba;
			}
		}

		Bnew.push_back(bNew);
	}

	B.insert(B.end(), Bnew.begin(), Bnew.end());
}

void POMDPPBVI::expand_greedy_error_reduction()
{
	// TODO: Implement.
}
