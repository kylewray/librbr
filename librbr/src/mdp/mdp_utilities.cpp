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


#include "../../include/mdp/mdp_utilities.h"

#include <limits>
#include <math.h>

void bellman_update(const StatesMap *S, const ActionsMap *A, const StateTransitions *T,
		const SASRewards *R, const Horizon *h, const State *s,
		std::unordered_map<const State *, double> &V, const Action *&aBest)
{
	double maxQsa = std::numeric_limits<double>::lowest();

	// For all the actions, compute max Q(s, a), and argmax Q(s, a), both over the set of available actions.
	for (auto action : A->available(s)) {
		const Action *a = resolve(action);

		// Compute the Q(s, a) estimate.
		double Qsa = 0.0;

		std::vector<const State *> successors;
		T->successors(S, s, a, successors);

		for (const State *sPrime : successors) {
			Qsa += T->get(s, a, sPrime) * (R->get(s, a, sPrime) + h->get_discount_factor() * V[sPrime]);
		}

		// While we are looping over actions, find the max and argmax.
		if (Qsa > maxQsa) {
			maxQsa = Qsa;
			aBest = a;
		}
	}

	V[s] = maxQsa;
}

void compute_V_pi(const StatesMap *S, const ActionsMap *A, const StateTransitions *T, const SASRewards *R, const Horizon *h,
		double epsilon, const PolicyMap *pi, std::unordered_map<const State *, double> &V)
{
	for (auto state : *S) {
		const State *s = resolve(state);
		V[s] = 0.0;
	}

	double convergenceCriterion = epsilon * (1.0 - h->get_discount_factor()) / h->get_discount_factor();
	double delta = convergenceCriterion + 1.0;

	while (delta > convergenceCriterion) {
		delta = 0.0;

		// For all the states, compute V(s).
		for (auto state : *S) {
			const State *s = resolve(state);

			std::vector<const State *> successors;
			T->successors(S, s, pi->get(s), successors);

			double QsPIs = 0.0;

			for (const State *sPrime : successors) {
				QsPIs += T->get(s, pi->get(s), sPrime) * (R->get(s, pi->get(s), sPrime) + h->get_discount_factor() * V[sPrime]);
			}

			if (fabs(V[s] - QsPIs) > delta) {
				delta = fabs(V[s] - QsPIs);
			}

			V[s] = QsPIs;
		}
	}
}

void compute_V_pi(const StatesMap *S, const ActionsMap *A, const StateTransitions *T, const FactoredRewards *R, const Horizon *h,
		double epsilon, const PolicyMap *pi, std::vector<std::unordered_map<const State *, double> > &V)
{
	V.clear();
	V.resize(R->get_num_rewards());

	for (int i = 0; i < (int)R->get_num_rewards(); i++) {
		const SASRewards *Ri = dynamic_cast<const SASRewards *>(R->get(i));
		compute_V_pi(S, A, T, Ri, h, epsilon, pi, V[i]);
	}
}
