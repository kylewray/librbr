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


#include "../../../include/core/rewards/sa_rewards_array.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/indexed_state.h"
#include "../../../include/core/actions/indexed_action.h"

#include <limits>

SARewardsArray::SARewardsArray(unsigned int numStates, unsigned int numActions)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	rewards = new float[states * actions];

	reset();
}

SARewardsArray::~SARewardsArray()
{
	delete [] rewards;
}

void SARewardsArray::set(const State *state, const Action *action, double reward)
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);

	if (s == nullptr || a == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions) {
		throw RewardException();
	}

	rewards[s->get_index() * actions + a->get_index()] = reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

void SARewardsArray::set(const State *state, const Action *action, const State *nextState, double reward)
{
	set(state, action, reward);
}

void SARewardsArray::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{
	set(state, action, reward);
}

double SARewardsArray::get(const State *state, const Action *action) const
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);

	if (s == nullptr || a == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions) {
		throw RewardException();
	}

	return rewards[s->get_index() * actions + a->get_index()];
}

double SARewardsArray::get(const State *state, const Action *action, const State *nextState) const
{
	return get(state, action);
}

double SARewardsArray::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	return get(state, action);
}

void SARewardsArray::reset()
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			rewards[s * actions + a] = 0.0f;
		}
	}

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

void SARewardsArray::set_rewards(const float *R)
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			rewards[s * actions + a] = R[s * actions + a];

			if (Rmin > R[s * actions + a]) {
				Rmin = R[s * actions + a];
			}
			if (Rmax < R[s * actions + a]) {
				Rmax = R[s * actions + a];
			}
		}
	}
}

const float *SARewardsArray::get_rewards() const
{
	return (const float *)rewards;
}

unsigned int SARewardsArray::get_num_states() const
{
	return states;
}

unsigned int SARewardsArray::get_num_actions() const
{
	return actions;
}

double SARewardsArray::get_min() const
{
	return Rmin;
}

double SARewardsArray::get_max() const
{
	return Rmax;
}
