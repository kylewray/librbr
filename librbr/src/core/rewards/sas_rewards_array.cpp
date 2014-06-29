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


#include "../../../include/core/rewards/sas_rewards_array.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/indexed_state.h"
#include "../../../include/core/actions/indexed_action.h"

#include <limits.h>

SASRewardsArray::SASRewardsArray(unsigned int numStates, unsigned int numActions)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	rewards = new float**[states];
	for (int s = 0; s < states; s++) {
		rewards[s] = new float*[actions];
		for (int a = 0; a < actions; a++) {
			rewards[s][a] = new float[states];
		}
	}

	reset();
}

SASRewardsArray::~SASRewardsArray()
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			delete [] rewards[s][a];
		}
		delete [] rewards[s];
	}
	delete rewards;
}

void SASRewardsArray::set(const State *state, const Action *action, const State *nextState, double reward)
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);
	const IndexedState *sp = dynamic_cast<const IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions ||
			sp->get_index() >= states) {
		throw RewardException();
	}

	rewards[s->get_index()][a->get_index()][sp->get_index()] = reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

void SASRewardsArray::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{
	set(state, action, nextState, reward);
}

double SASRewardsArray::get(const State *state, const Action *action, const State *nextState) const
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);
	const IndexedState *sp = dynamic_cast<const IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions ||
			sp->get_index() >= states) {
		throw RewardException();
	}

	return rewards[s->get_index()][a->get_index()][sp->get_index()];
}

double SASRewardsArray::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	return get(state, action, nextState);
}

void SASRewardsArray::reset()
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			for (int sp = 0; sp < states; sp++) {
				rewards[s][a][sp] = 0.0f;
			}
		}
	}

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

double SASRewardsArray::get_min() const
{
	return Rmin;
}

double SASRewardsArray::get_max() const
{
	return Rmax;
}
