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


#include "../../../include/core/rewards/factored_rewards.h"
#include "../../../include/core/rewards/reward_exception.h"

#include <limits>

FactoredRewards::FactoredRewards()
{ }

FactoredRewards::FactoredRewards(unsigned int numRewards)
{
	if (numRewards < 1) {
		numRewards = 1;
	}

	rewards.resize(numRewards);
	for (int i = 0; i < (int)numRewards; i++) {
		rewards[i] = nullptr;
	}
}

FactoredRewards::~FactoredRewards()
{
	reset();
}

void FactoredRewards::add_factor(Rewards *newRewardsFactor)
{
	if (newRewardsFactor == nullptr) {
		throw RewardException();
	}
	rewards.push_back(newRewardsFactor);
}

void FactoredRewards::set(unsigned int factorIndex, Rewards *newRewardsFactor)
{
	if (factorIndex >= rewards.size()) {
		throw RewardException();
	}

	if (rewards[factorIndex] != nullptr) {
		delete rewards[factorIndex];
	}
	rewards[factorIndex] = newRewardsFactor;
}

Rewards *FactoredRewards::get(unsigned int factorIndex) const
{
	if (factorIndex >= rewards.size()) {
		throw RewardException();
	}

	return rewards[factorIndex];
}

unsigned int FactoredRewards::get_num_rewards() const
{
	return rewards.size();
}

void FactoredRewards::reset()
{
	for (Rewards *r : rewards) {
		if (r != nullptr) {
			delete r;
		}
	}
	rewards.clear();
}
