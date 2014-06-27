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


#include "../../include/mdp/mdp_utilities.h"

#include <limits>

void bellman_update(const StatesMap *S, const FiniteActions *A, const StateTransitionsMap *T,
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
