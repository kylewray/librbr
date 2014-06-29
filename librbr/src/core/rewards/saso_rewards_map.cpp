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


#include "../../../include/core/rewards/saso_rewards_map.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"
#include "../../../include/core/observations/named_observation.h"

#include <limits.h>

SASORewardsMap::SASORewardsMap()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");
	observationWildcard = new NamedObservation("*");

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

SASORewardsMap::~SASORewardsMap()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
	delete observationWildcard;
}

void SASORewardsMap::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}
	if (nextState == nullptr) {
		nextState = stateWildcard;
	}
	if (observation == nullptr) {
		observation = observationWildcard;
	}

	rewards[state][action][nextState][observation] = reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

double SASORewardsMap::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 16; i++) {
		const State *alpha = stateWildcard;
		if (!(bool)(i & (1 << 0))) {
			alpha = state;
		}

		const Action *beta = actionWildcard;
		if (!(bool)(i & (1 << 1))) {
			beta = action;
		}

		const State *gamma = stateWildcard;
		if (!(bool)(i & (1 << 2))) {
			gamma = nextState;
		}

		const Observation *delta = observationWildcard;
		if (!(bool)(i & (1 << 3))) {
			delta = observation;
		}

		try {
			return get_value(alpha, beta, gamma, delta);
		} catch (const RewardException &err) { }
	}

	return 0.0;
}

void SASORewardsMap::reset()
{
	rewards.clear();
	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

double SASORewardsMap::get_value(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	std::unordered_map<const State *,
		std::unordered_map<const Action *,
		std::unordered_map<const State *,
		std::unordered_map<const Observation *, double> > > >::const_iterator alpha = rewards.find(state);
	if (alpha == rewards.end()) {
		throw RewardException();
	}

	std::unordered_map<const Action *,
		std::unordered_map<const State *,
		std::unordered_map<const Observation *, double>> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw RewardException();
	}

	std::unordered_map<const State *,
		std::unordered_map<const Observation *, double>>::const_iterator gamma = beta->second.find(nextState);
	if (gamma == beta->second.end()) {
		throw RewardException();
	}

	std::unordered_map<const Observation *, double>::const_iterator delta = gamma->second.find(observation);
	if (delta == gamma->second.end()) {
		throw RewardException();
	}

	return delta->second;
}

double SASORewardsMap::get_min() const
{
	return Rmin;
}

double SASORewardsMap::get_max() const
{
	return Rmax;
}
