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


#include "../../../include/core/rewards/saso_rewards.h"
#include "../../../include/core/rewards/reward_exception.h"
#include "../../../include/core/states/named_state.h"

/**
 * The default constructor for the SASORewards class.
 */
SASORewards::SASORewards()
{ }

/**
 * The default deconstructor for the SASORewards class.
 */
SASORewards::~SASORewards()
{
	reset();
}

/**
 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
 * @param state			The current state of the system.
 * @param action		The action taken in the current state.
 * @param nextState		The next state with which we assign the reward.
 * @param observation	The observation made at the next state.
 * @param reward		The reward from the provided state-action-state-observation quadruple.
 */
void SASORewards::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{ }

/**
 * The probability of a transition following the state-action-state-observation quadruple provided.
 * @param state			The current state of the system.
 * @param action		The action taken in the current state.
 * @param nextState		The next state with which we assign the reward.
 * @param observation	The observation made at the next state.
 * @return The reward from taking the given action in the given state.
 */
double SASORewards::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	return 0.0;
}

/**
 * Get the minimal R-value.
 * @return The minimal R-value.
 */
double SASORewards::get_min() const
{
	return 0.0;
}

/**
 * Get the maximal R-value.
 * @return The maximal R-value.
 */
double SASORewards::get_max() const
{
	return 0.0;
}

/**
 * Reset the rewards, clearing the internal mapping.
 */
void SASORewards::reset()
{ }
