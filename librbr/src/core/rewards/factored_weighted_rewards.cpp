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


#include "../../../include/core/rewards/factored_weighted_rewards.h"
#include "../../../include/core/rewards/reward_exception.h"

#include <limits>

FactoredWeightedRewards::FactoredWeightedRewards()
{ }

FactoredWeightedRewards::FactoredWeightedRewards(unsigned int numRewards,
		std::vector<double> &weights) : FactoredRewards(numRewards)
{
	if (weights.size() != numRewards) {
		throw RewardException();
	}

	for (double weight : weights) {
		w.push_back(weight);
	}
}

FactoredWeightedRewards::~FactoredWeightedRewards()
{
	reset();
}

void FactoredWeightedRewards::add_factor(Rewards *newRewardsFactor)
{
	const SASORewards *R = dynamic_cast<const SASORewards *>(newRewardsFactor);
	if (R == nullptr) {
		throw RewardException();
	}

	FactoredRewards::add_factor(newRewardsFactor);
}

void FactoredWeightedRewards::set(unsigned int factorIndex, Rewards *newRewardsFactor)
{
	const SASORewards *R = dynamic_cast<const SASORewards *>(newRewardsFactor);
	if (R == nullptr) {
		throw RewardException();
	}

	FactoredRewards::set(factorIndex, newRewardsFactor);
}

const SASORewards *FactoredWeightedRewards::get(unsigned int factorIndex) const
{
	const SASORewards *R = dynamic_cast<const SASORewards *>(FactoredRewards::get(factorIndex));
	if (R == nullptr) {
		throw RewardException();
	}
	return R;
}

void FactoredWeightedRewards::set(const State *state, const Action *action, const State *nextState,
		const Observation *observation, double reward)
{
	for (Rewards *R : rewards) {
		SASORewards *Ri = dynamic_cast<SASORewards *>(R);
		Ri->set(state, action, nextState, observation, reward);
	}
}

double FactoredWeightedRewards::get(const State *state, const Action *action, const State *nextState,
		const Observation *observation) const
{
	double weightedAverage = 0.0;

	for (int i = 0; i < (int)rewards.size(); i++) {
		SASORewards *Ri = dynamic_cast<SASORewards *>(rewards[i]);

		weightedAverage += w[i] * Ri->get(state, action, nextState, observation);
	}

	return weightedAverage;
}

double FactoredWeightedRewards::get_min() const
{
	double Rmin = 0.0;

	for (int i = 0; i < (int)rewards.size(); i++) {
		SASORewards *Ri = dynamic_cast<SASORewards *>(rewards[i]);

		Rmin += w[i] * Ri->get_min();
	}

	return Rmin;
}

double FactoredWeightedRewards::get_max() const
{
	double Rmax = 0.0;

	for (int i = 0; i < (int)rewards.size(); i++) {
		SASORewards *Ri = dynamic_cast<SASORewards *>(rewards[i]);

		Rmax += w[i] * Ri->get_max();
	}

	return Rmax;
}

void FactoredWeightedRewards::reset()
{
	FactoredRewards::reset();

	for (Rewards *R : rewards) {
		SASORewards *Ri = dynamic_cast<SASORewards *>(R);
		Ri->reset();
	}
}
