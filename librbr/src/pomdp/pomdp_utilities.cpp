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


#include "../../include/pomdp/pomdp_utilities.h"

/**
 * Create the commonly used Gamma_{a,*}.
 * @param S			The set of finite states.
 * @param A			The set of finite actions.
 * @param Z			The set of finite observations.
 * @param T			The finite state transition function.
 * @param O			The finite observation transition function.
 * @param R			The state-action-state rewards function.
 * @param action	The action taken at this time step.
 * @return The alpha vector which is the single element in Gamma_{a,*}.
 */
PolicyAlphaVector *create_gamma_a_star(const FiniteStates *S, const FiniteActions *A,
		const FiniteObservations *Z, const FiniteStateTransitions *T, const FiniteObservationTransitions *O,
		const SASORewards *R, const Action *action)
{
	PolicyAlphaVector *alpha = new PolicyAlphaVector(action);

	for (const State *state : *S) {
		// Compute the immediate state-action-state-observation reward.
		double immediateReward = 0.0;
		for (const State *nextState : *S) {
			double innerImmediateReward = 0.0;
			for (const Observation *observation : *Z) {
				innerImmediateReward += O->get(action, nextState, observation) * R->get(state, action, nextState, observation);
			}
			immediateReward += T->get(state, action, nextState) * innerImmediateReward;
		}
		alpha->set(state, immediateReward);
	}

	return alpha;
}

/**
 * Perform the belief state update equation on the current belief.
 * @param S				The finite states object.
 * @param T				The finite state transition function.
 * @param O				The finite observation transition function.
 * @param belief		The current belief state.
 * @param action		The action taken in the current belief state.
 * @param observation	The observation observed after taking the action in the current belief state.
 * @return The resultant new belief state.
 */
BeliefState belief_state_update(const BeliefState &belief, const FiniteStates *S, const FiniteStateTransitions *T,
		const FiniteObservationTransitions *O, const Action *action, const Observation *observation) {
	BeliefState nextBelief;

	// First, compute all of the numerators and the denominator independently.
	double N = 0.0;
	for (const State *nextState : *S) {
		double b = 0.0;

		for (const State *state : *S) {
			b += T->get(state, action, nextState) * belief.get(state);
		}
		b *= O->get(action, nextState, observation);

		nextBelief.set(nextState, b);
		N += b;
	}

	// Normalize by the denominator.
	for (const State *nextState : *S) {
		nextBelief.set(nextState, nextBelief.get(nextState) / N);
	}

	return nextBelief;
}

/**
 * Compute the Bellman update/backup for a given belief state. Since the value function V' = HV, is over the belief state
 * space, and we can represent the value function approximately as a PWLC set of alpha vectors, it returns this set of
 * alpha vectors.
 * @param S 		 The finite states.
 * @param A 		 The finite actions.
 * @param Z			 The finite observations.
 * @param T 		 The finite state transition function.
 * @param O			 The finite observation transition function.
 * @param R 		 The state-action-state rewards.
 * @param h 		 The horizon.
 * @param action	 The action taken.
 * @param gammaAStar The initial gamma which is always used in the cross sum: Gamma_{a,*}.
 * @param gamma		 The current Bellman backup, represented as the set Gamma storing alpha-vectors.
 * @return The Gamma_{a} which contains the new set of optimal alpha-vectors, given a particular action.
 */
std::vector<PolicyAlphaVector *> bellman_update(const FiniteStates *S, const FiniteActions *A, const FiniteObservations *Z,
		const FiniteStateTransitions *T, const FiniteObservationTransitions *O, const SASORewards *R,
		const Horizon *h, const Action *action, const std::vector<PolicyAlphaVector *> &gammaAStar,
		const std::vector<PolicyAlphaVector *> &gamma)
{
	// Perform a deep copy on the gammaAStar variable, since we will need fresh PolicyAlphaVectors in the for loop below.
	std::vector<PolicyAlphaVector *> gammaA;
	for (PolicyAlphaVector *alphaVector : gammaAStar) {
		gammaA.push_back(new PolicyAlphaVector(*alphaVector));
	}

	// Iteratively compute and apply the cross-sum of the gamma.
	for (const Observation *observation : *Z) {
		// Compute the set Gamma_{a, omega}.
		std::vector<PolicyAlphaVector *> gammaAOmega;

		// For each alpha vector in the set Gamma_{a, omega}, we have to consider a different gamma in Gamma^{t-1}.
		for (PolicyAlphaVector *alphaGamma : gamma) {
			// Note: It doesn't matter if we set the action here, since cross_sum will create new alpha vectors anyway.
			PolicyAlphaVector *newAlpha = new PolicyAlphaVector();

			// For each of the columns in the alpha vector.
			for (const State *state : *S) {
				double value = 0.0;

				// Compute the value of an element of the alpha vector.
				for (const State *nextState : *S) {
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
