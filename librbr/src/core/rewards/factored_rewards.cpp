/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#include "../../../include/core/rewards/factored_rewards.h"
#include "../../../include/core/rewards/reward_exception.h"

#include <limits>

/**
 * The default constructor for the FactoredRewards class.
 */
FactoredRewards::FactoredRewards()
{ }

/**
 * The constructor for the FactoredRewards class which allows the specification
 * of the number of rewards.
 * @param numRewards The number of rewards in the multi-rewards vector, minimum of 1.
 */
FactoredRewards::FactoredRewards(int numRewards)
{
	if (numRewards < 1) {
		numRewards = 1;
	}

	rewards.resize(numRewards);
	for (int i = 0; i < numRewards; i++) {
		rewards[i] = nullptr;
	}
}

/**
 * The default deconstructor for the MultiRewards class.
 */
FactoredRewards::~FactoredRewards()
{
	reset();
}

/**
 * Add a new rewards factor to the vector of available rewards. This relinquishes control of the
 * new rewards factor to this object.
 * @param newRewardsFactor	The new rewards factor to include in the vector of available rewards.
 * @throw RewardException	The new rewards factor was null.
 */
void FactoredRewards::add_factor(Rewards *newRewardsFactor)
{
	if (newRewardsFactor == nullptr) {
		throw RewardException();
	}
	rewards.push_back(newRewardsFactor);
}

/**
 * Set the particular rewards element in the multi-rewards vector. This frees memory.
 * @param factorIndex		The index of the factor to add the states to.
 * @param newRewardsFactor	The new rewards factor for this index.
 * @throws RewardException	The index was invalid.
 */
void FactoredRewards::set(int factorIndex, Rewards *newRewardsFactor)
{
	if (factorIndex < 0 || factorIndex >= rewards.size()) {
		throw RewardException();
	}

	if (rewards[factorIndex] != nullptr) {
		delete rewards[factorIndex];
	}
	rewards[factorIndex] = newRewardsFactor;
}

/**
 * Get the particular rewards element in the factored rewards vector.
 * @param factorIndex The index of the factor to add the rewards factor to.
 * @returns The rewards for this index.
 * @throws RewardException	The index was invalid.
 */
Rewards *FactoredRewards::get(int factorIndex) const
{
	if (factorIndex < 0 || factorIndex >= rewards.size()) {
		throw RewardException();
	}

	return rewards[factorIndex];
}

/**
 * Get the number of rewards, i.e., the dimension of the multi-rewards vector.
 * @return The number of rewards.
 */
int FactoredRewards::get_num_rewards() const
{
	return rewards.size();
}

/**
 * Reset the rewards, freeing all the memory allocated by each object in the vector.
 */
void FactoredRewards::reset()
{
	for (Rewards *r : rewards) {
		if (r != nullptr) {
			delete r;
		}
	}
	rewards.clear();
}
