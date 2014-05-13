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


#ifndef SAS_REWARDS_H
#define SAS_REWARDS_H


#include <map>

#include "saso_rewards.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * A class for state-action-state rewards in an MDP-like object. Informally, there are two basic
 * ways to store state-action rewards: a table lookup mapping state-action-state triples to real
 * values, or a generator function based on a given state-action-state triple. In both cases, we
 * require that any class with provide certain get functions so that any generic solver can handle
 * both cases.
 *
 * If you want to create a map, table, or generator SASRewards class, please create a child class
 * which implements the function in the virtual functions described below.
 *
 * Always use this class for MDP-like object solvers (as part of a dynamic cast, for example). The
 * particular underlying storage structure will be defined by a child class of SASRewards, e.g.,
 * SASRewardsMap.
 */
class SASRewards : public SASORewards {
public:
	/**
	 * The default constructor for the SASRewards class.
	 */
	SASRewards();

	/**
	 * The default deconstructor for the SASRewards class.
	 */
	virtual ~SASRewards();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param state		The current state of the system.
	 * @param action	The action taken in the current state.
	 * @param nextState	The next state with which we assign the reward.
	 * @param reward	The reward from the provided state-action-state triple.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState, double reward);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * @param state			The current state of the system.
	 * @param action		The action taken in the current state.
	 * @param nextState		The next state with which we assign the reward.
	 * @param observation	The observation made at the next state.
	 * @param reward		The reward from the provided state-action-state-observation quadruple.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState,
			const Observation *observation, double reward);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param state		The current state of the system.
	 * @param action	The action taken at the current state.
	 * @param nextState	The next state with which we assign the reward.
	 * @return The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState) const;

	/**
	 * The probability of a transition following the state-action-state-observation quadruple provided.
	 * @param state			The current state of the system.
	 * @param action		The action taken in the current state.
	 * @param nextState		The next state with which we assign the reward.
	 * @param observation	The observation made at the next state.
	 * @return The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState,
			const Observation *observation) const;

	/**
	 * Reset the rewards, clearing the internal mapping.
	 */
	virtual void reset();

};

#endif // SAS_REWARDS_H