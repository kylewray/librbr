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


#include "../../../include/core/rewards/sa_rewards.h"
#include "../../../include/core/rewards/reward_exception.h"
#include "../../../include/core/states/named_state.h"

/**
 * The default constructor for the SARewards class.
 */
SARewards::SARewards()
{ }

/**
 * The default deconstructor for the SARewards class.
 */
SARewards::~SARewards()
{
	reset();
}

/**
 * Set a state transition from a particular state-action pair to a probability.
 * @param state		The current state of the system.
 * @param action	The action taken in the current state.
 * @param reward	The reward from the provided state-action-state triple.
 */
void SARewards::set(const State *state, const Action *action, double reward)
{ }

/**
 * Set a state transition from a particular state-action-state triple to a probability.
 * @param state		The current state of the system.
 * @param action	The action taken in the current state.
 * @param nextState	The next state with which we assign the reward.
 * @param reward	The reward from the provided state-action-state triple.
 */
void SARewards::set(const State *state, const Action *action, const State *nextState, double reward)
{ }

/**
 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
 * @param state			The current state of the system.
 * @param action		The action taken in the current state.
 * @param nextState		The next state with which we assign the reward.
 * @param observation	The observation made at the next state.
 * @param reward		The reward from the provided state-action-state-observation quadruple.
 */
void SARewards::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{ }

/**
 * The probability of a transition following the state-action pair provided.
 * @param state		The current state of the system.
 * @param action	The action taken at the current state.
 * @return The reward from taking the given action in the given state.
 */
double SARewards::get(const State *state, const Action *action) const
{
	return 0.0;
}

/**
 * The probability of a transition following the state-action-state triple provided.
 * @param state		The current state of the system.
 * @param action	The action taken at the current state.
 * @param nextState	The next state with which we assign the reward.
 * @return The reward from taking the given action in the given state.
 */
double SARewards::get(const State *state, const Action *action, const State *nextState) const
{
	return 0.0;
}

/**
 * The probability of a transition following the state-action-state-observation quadruple provided.
 * @param state			The current state of the system.
 * @param action		The action taken in the current state.
 * @param nextState		The next state with which we assign the reward.
 * @param observation	The observation made at the next state.
 * @return The reward from taking the given action in the given state.
 */
double SARewards::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	return 0.0;
}

/**
 * Reset the rewards, clearing the internal mapping.
 */
void SARewards::reset()
{ }
