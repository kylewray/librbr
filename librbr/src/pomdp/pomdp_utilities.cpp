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


#include "../../include/pomdp/pomdp_utilities.h"

PolicyAlphaVector *create_gamma_a_star(const StatesMap *S, const FiniteActions *A,
		const FiniteObservations *Z, const FiniteStateTransitions *T, const FiniteObservationTransitions *O,
		const SASORewards *R, const Action *action)
{
	PolicyAlphaVector *alpha = new PolicyAlphaVector(action);

	for (auto s : *S) {
		const State *state = resolve(s);

		// Compute the immediate state-action-state-observation reward.
		double immediateReward = 0.0;
		for (auto sp : *S) {
			const State *nextState = resolve(sp);

			double innerImmediateReward = 0.0;
			for (auto z : *Z) {
				const Observation *observation = resolve(z);
				innerImmediateReward += O->get(action, nextState, observation) * R->get(state, action, nextState, observation);
			}
			immediateReward += T->get(state, action, nextState) * innerImmediateReward;
		}
		alpha->set(state, immediateReward);
	}

	return alpha;
}

BeliefState *belief_state_update(const StatesMap *S, const FiniteStateTransitions *T,
		const FiniteObservationTransitions *O, const BeliefState *belief, const Action *action,
		const Observation *observation) {
	BeliefState *nextBelief = new BeliefState();

	// First, compute all of the numerators and the denominator independently.
	double N = 0.0;
	for (auto sp : *S) {
		const State *nextState = resolve(sp);

		double b = 0.0;
		for (auto s : *S) {
			const State *state = resolve(s);
			b += T->get(state, action, nextState) * belief->get(state);
		}
		b *= O->get(action, nextState, observation);

		nextBelief->set(nextState, b);
		N += b;
	}

	// Normalize by the denominator.
	for (auto sp : *S) {
		const State *nextState = resolve(sp);
		nextBelief->set(nextState, nextBelief->get(nextState) / N);
	}

	return nextBelief;
}

std::vector<PolicyAlphaVector *> bellman_update_cross_sum(const StatesMap *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
		const Horizon *h, const std::vector<PolicyAlphaVector *> &gammaAStar, const std::vector<PolicyAlphaVector *> &gamma,
		const Action *action)
{
	// Perform a deep copy on the gammaAStar variable, since we will need fresh PolicyAlphaVectors in the for loop below.
	std::vector<PolicyAlphaVector *> gammaA;
	for (PolicyAlphaVector *alphaVector : gammaAStar) {
		gammaA.push_back(new PolicyAlphaVector(*alphaVector));
	}

	// Iteratively compute and apply the cross-sum of the gamma.
	for (auto z : *Z) {
		const Observation *observation = resolve(z);

		// Compute the set Gamma_{a, omega}.
		std::vector<PolicyAlphaVector *> gammaAOmega;

		// For each alpha vector in the set Gamma_{a, omega}, we have to consider a different gamma in Gamma^{t-1}.
		for (PolicyAlphaVector *alphaGamma : gamma) {
			// Note: It doesn't matter if we set the action here, since cross_sum will create new alpha vectors anyway.
			PolicyAlphaVector *newAlpha = new PolicyAlphaVector();

			// For each of the columns in the alpha vector.
			for (auto s : *S) {
				const State *state = resolve(s);

				// Compute the value of an element of the alpha vector.
				double value = 0.0;
				for (auto sp : *S) {
					const State *nextState = resolve(sp);
					value += T->get(state, action, nextState) * O->get(action, nextState, observation) * alphaGamma->get(nextState);
				}
				value *= h->get_discount_factor();

				newAlpha->set(state, value);
			}

			// Store the newly created alpha vector in Gamma_{a, omega}.
			gammaAOmega.push_back(newAlpha);
		}

		// Perform the Minkowski sum (cross-sum) and expand the final result (gammaA) by gammaAOmega.
		std::vector<PolicyAlphaVector *> crossSum = PolicyAlphaVector::cross_sum(gammaA, gammaAOmega);

		// Free the memory of the previous values of gammaA, since cross_sum allocates new memory for the alpha vectors. The new
		// value of gammaA is the result of the Minkowski sum.
		for (PolicyAlphaVector *alphaGamma : gammaA) {
			delete alphaGamma;
		}
		gammaA = crossSum;

		// Since the cross_sum function allocates memory for the alpha vectors, but we also allocated memory
		// for the newAlpha inside the for loop above, free this memory. Keep the gammaA memory though, since
		// that is part of the result of this function.
		for (PolicyAlphaVector *alphaGamma : gammaAOmega) {
			delete alphaGamma;
		}
		gammaAOmega.clear();

		// Lastly, since we made new alpha vectors inside cross_sum, we need to set their actions.
		for (PolicyAlphaVector *alphaGamma : gammaA) {
			alphaGamma->set_action(action);
		}
	}

	return gammaA;
}

PolicyAlphaVector *bellman_update_belief_state(const StatesMap *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
		const Horizon *h, const std::vector<PolicyAlphaVector *> &gammaAStar, const std::vector<PolicyAlphaVector *> &gamma,
		const Action *action, const BeliefState *b)
{
	// Create the alpha vector and initialize its value equal to that of the element inside gammaAStar.
	PolicyAlphaVector *alphaBAStar = new PolicyAlphaVector(*gammaAStar[0]);
	alphaBAStar->set_action(action);

	// Add to its value for each state following a summation, instead of the cross-sum. This operation iterates over the
	// possible observations and selects the optimal policy for that subtree given the current set of alpha vectors in gamma.
	for (auto z : *Z) {
		const Observation *observation = resolve(z);

		// The argmax of alpha dot beta given the belief point, action, and observation.
		PolicyAlphaVector *maxAlphaBAOmega = nullptr;
		double maxAlphaDotBeta = 0.0;

		// For each alpha vector in the set Gamma_{b, a, omega}, we have to consider a different gamma in Gamma^{t-1}.
		for (PolicyAlphaVector *alphaGamma : gamma) {
			// Note: It doesn't matter if we set the action here, since cross_sum will create new alpha vectors anyway.
			PolicyAlphaVector *newAlpha = new PolicyAlphaVector();

			// For each of the columns in the alpha vector.
			for (auto s : *S) {
				const State *state = resolve(s);

				// Compute the value of an element of the alpha vector.
				double value = 0.0;
				for (auto sp : *S) {
					const State *nextState = resolve(sp);
					value += T->get(state, action, nextState) * O->get(action, nextState, observation) * alphaGamma->get(nextState);
				}
				value *= h->get_discount_factor();

				newAlpha->set(state, value);
			}

			// Compute the value at the belief state for this alpha vector.
			double alphaDotBeta = newAlpha->compute_value(b);
			if (maxAlphaBAOmega == nullptr || alphaDotBeta > maxAlphaDotBeta) {
				// This is better, so keep this alpha vector in Gamma_{b, a, omega} and free the
				// current best.
				if (maxAlphaBAOmega != nullptr) {
					delete maxAlphaBAOmega;
				}
				maxAlphaBAOmega = newAlpha;
				maxAlphaDotBeta = alphaDotBeta;
			} else {
				// Free the memory of the newly created alpha vector in Gamma_{b, a, omega}, since
				// it is not better than the current best.
				delete newAlpha;
			}
		}

		// Add this optimal value given the observation to the alpha vector we are computing.
		*alphaBAStar += *maxAlphaBAOmega;

		// Since the loop above allocates memory for the alpha vector, free this memory.
		delete maxAlphaBAOmega;
	}

	return alphaBAStar;
}
