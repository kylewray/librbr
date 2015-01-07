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


#ifndef SSP_UCT_H
#define SSP_UCT_H


#include "ssp.h"

#include "../core/states/states.h"
#include "../core/actions/actions.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/rewards/rewards.h"
#include "../core/initial.h"
#include "../core/horizon.h"

#include <unordered_map>

///**
// * Solve an SSP via UCT. This solver has the following requirements:
// * - SSP states must be of type FiniteStates.
// * - SSP actions must be of type FiniteActions.
// * - SSP state transitions must be of type FiniteStateTransitions.
// * - SSP rewards must be of type SASRewards.
// */
//class SSPUCT {
//public:
//	/**
//	 * The default constructor for the SSPUCT class. The default tolerance is 0.001.
//	 */
//	SSPUCT();
//
//	/**
//	 * A constructor for the SSPUCT class which allows for the specification
//	 * of the convergence criterion (tolerance).
//	 * @param	tolerance		The tolerance which determines convergence of UCT.
//	 */
//	SSPUCT(double tolerance);
//
//	/**
//	 * The deconstructor for the SSPUCT class.
//	 */
//	virtual ~SSPUCT();
//
//	/**
//	 * Solve the MDP provided using value iteration.
//	 * @param	ssp							The stochastic shortest path to solve.
//	 * @throw	StateException				The SSP did not have a StatesMap states object.
//	 * @throw	ActionException				The SSP did not have a ActionsMap actions object.
//	 * @throw	StateTransitionsException	The SSP did not have a FiniteStateTransitions state transitions object.
//	 * @throw	RewardException				The SSP did not have a SASRewards rewards object.
//	 * @throw	PolicyException				An error occurred computing the policy.
//	 * @return	Return the optimal policy.
//	 */
//	PolicyMap *solve(SSP *ssp);
//
//	/**
//	 * Get the values of the states' mapping.
//	 * @return	The mapping from states to values.
//	 */
//	const std::unordered_map<State *, double> &get_V() const;
//
//private:
//	/**
//	 * Solve a finite horizon SSP using value iteration.
//	 * @param	S					The finite states.
//	 * @param	A					The finite actions.
//	 * @param	T					The finite state transition function.
//	 * @param	R					The state-action-state rewards.
//	 * @param	h					The horizon.
//	 * @throw	PolicyException		An error occurred computing the policy.
//	 * @return	Return the optimal policy.
//	 */
//	PolicyMap *solve_finite_horizon(StatesMap *S, ActionsMap *A, StateTransitions *T,
//			SASRewards *R, Horizon *h);
//
//	/**
//	 * Solve an infinite horizon SSP using value iteration.
//	 * @param	S					The finite states.
//	 * @param	A					The finite actions.
//	 * @param	T					The finite state transition function.
//	 * @param	R					The state-action-state rewards.
//	 * @param	h					The horizon.
//	 * @throw	PolicyException		An error occurred computing the policy.
//	 * @return	Return the optimal policy.
//	 */
//	PolicyMap *solve_infinite_horizon(StatesMap *S, ActionsMap *A, StateTransitions *T,
//			SASRewards *R, Horizon *h);
//
//	/**
//	 * The value of a states and state's actions.
//	 */
//	std::unordered_map<State *, double> V;
//
//	/**
//	 * The tolerance convergence criterion.
//	 */
//	double epsilon;
//
//};


#endif // SSP_UCT_H
