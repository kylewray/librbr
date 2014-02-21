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


#ifndef SOLVER_UTILITIES_H
#define SOLVER_UTILITIES_H


#include "../../include/core/states/state.h"
#include "../../include/core/states/finite_states.h"
#include "../../include/core/actions/finite_actions.h"
#include "../../include/core/state_transitions/finite_state_transitions.h"
#include "../../include/core/rewards/sas_rewards.h"
#include "../../include/core/horizon.h"

/**
 * Compute the Bellman update/backup for a given state and compute the action which achieves the
 * highest Q(s, a), and store the value and action in the variables provided.
 * @param S 	The finite states.
 * @param A 	The finite actions.
 * @param T 	The finite state transition function.
 * @param R 	The state-action-state rewards.
 * @param h 	The horizon.
 * @param s 	The current state being examined, i.e., V(s).
 * @param V		The current Bellman backup, mapping states to expected rewards. This will be updated.
 * @param aBest The action which produced the maximum V(s) value. This will be updated.
 */
void bellman_update(const FiniteStates *S, const FiniteActions *A, const FiniteStateTransitions *T,
		const SASRewards *R, const Horizon *h, State *s, std::map<State *, double> &V, Action *&aBest);


#endif // SOLVER_UTILITIES_H
