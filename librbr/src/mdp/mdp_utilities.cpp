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

#include "../../include/core/state_transitions/state_transition_exception.h"

#include <limits>
#include <math.h>

void bellman_update(StatesMap *S, ActionsMap *A, StateTransitions *T,
		SASRewards *R, Horizon *h, State *s,
		std::unordered_map<State *, double> &V, Action *&aBest)
{
	double maxQsa = std::numeric_limits<double>::lowest();

	// For all the actions, compute max Q(s, a), and argmax Q(s, a), both over the set of available actions.
	for (auto action : A->available(s)) {
		Action *a = resolve(action);

		// Compute the Q(s, a) estimate.
		double Qsa = 0.0;

		try {
			for (State *sp : T->successors(S, s, a)) {
				Qsa += T->get(s, a, sp) * (R->get(s, a, sp) + h->get_discount_factor() * V[sp]);
			}
		} catch (StateTransitionException &err) {
			for (auto sPrime : *S) {
				State *sp = resolve(sPrime);
				Qsa += T->get(s, a, sp) * (R->get(s, a, sp) + h->get_discount_factor() * V[sp]);
			}
		}

		// While we are looping over actions, find the max and argmax.
		if (Qsa > maxQsa) {
			maxQsa = Qsa;
			aBest = a;
		}
	}

	V[s] = maxQsa;
}

void compute_V_pi(StatesMap *S, ActionsMap *A, StateTransitions *T, SASRewards *R, Horizon *h,
		double epsilon, PolicyMap *pi, std::unordered_map<State *, double> &V)
{
	for (auto state : *S) {
		State *s = resolve(state);
		V[s] = 0.0;
	}

	double convergenceCriterion = epsilon * (1.0 - h->get_discount_factor()) / h->get_discount_factor();
	double delta = convergenceCriterion + 1.0;

	while (delta > convergenceCriterion) {
		delta = 0.0;

		// For all the states, compute V(s).
		for (auto state : *S) {
			State *s = resolve(state);

			double QsPIs = 0.0;

			try {
				for (State *sp : T->successors(S, s, pi->get(s))) {
					QsPIs += T->get(s, pi->get(s), sp) * (R->get(s, pi->get(s), sp) + h->get_discount_factor() * V[sp]);
				}
			} catch (StateTransitionException &err) {
				for (auto sPrime : *S) {
					State *sp = resolve(sPrime);
					QsPIs += T->get(s, pi->get(s), sp) * (R->get(s, pi->get(s), sp) + h->get_discount_factor() * V[sp]);
				}
			}

//			try {
//				for (State *nextState : T->successors(S, state, action)) {
//					value += T->get(state, action, nextState) * O->get(action, nextState, observation) * alphaGamma->get(nextState);
//				}
//			} catch (StateTransitionException &err) {
//				for (auto sp : *S) {
//					State *nextState = resolve(sp);
//					value += T->get(state, action, nextState) * O->get(action, nextState, observation) * alphaGamma->get(nextState);
//				}
//			}

			if (fabs(V[s] - QsPIs) > delta) {
				delta = fabs(V[s] - QsPIs);
			}

			V[s] = QsPIs;
		}
	}
}

void compute_V_pi(StatesMap *S, ActionsMap *A, StateTransitions *T, FactoredRewards *R, Horizon *h,
		double epsilon, PolicyMap *pi, std::vector<std::unordered_map<State *, double> > &V)
{
	V.clear();
	V.resize(R->get_num_rewards());

	for (int i = 0; i < (int)R->get_num_rewards(); i++) {
		SASRewards *Ri = dynamic_cast<SASRewards *>(R->get(i));
		compute_V_pi(S, A, T, Ri, h, epsilon, pi, V[i]);
	}
}
