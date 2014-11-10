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


#ifndef FACTORED_REWARDS_H
#define FACTORED_REWARDS_H


#include "rewards.h"

#include "../states/state.h"
#include "../actions/action.h"
#include "../observations/observation.h"

#include <vector>

/**
 * A class for factored rewards in an MDP-like object, i.e., multi-objective MDPs (MOMDPs). This
 * class stores a fixed set of reward functions, each a Rewards child object in-and-of-itself.
 */
class FactoredRewards : virtual public Rewards {
public:
	/**
	 * The default constructor for the FactoredRewards class.
	 */
	FactoredRewards();

	/**
	 * The constructor for the FactoredRewards class which allows the specification
	 * of the number of rewards.
	 * @param	numRewards		The number of rewards in the multi-rewards vector, minimum of 1.
	 */
	FactoredRewards(unsigned int numRewards);

	/**
	 * The default deconstructor for the FactoredRewards class.
	 */
	virtual ~FactoredRewards();

	/**
	 * Add a new rewards factor to the vector of available rewards. This relinquishes control of the
	 * new rewards factor to this object.
	 * @param	newRewardsFactor	The new rewards factor to include in the vector of available rewards.
	 * @throw	RewardException		The new rewards factor was null.
	 */
	virtual void add_factor(Rewards *newRewardsFactor);

	/**
	 * Set the particular rewards element in the multi-rewards vector. This frees memory.
	 * @param	factorIndex 		The index of the factor to add the rewards factor to.
	 * @param	newRewardsFactor	The new rewards factor for this index.
	 * @throw	RewardException		The index was invalid.
	 */
	virtual void set(unsigned int factorIndex, Rewards *newRewardsFactor);

	/**
	 * Get the particular rewards element in the factored rewards vector.
	 * @param	factorIndex			The index of the factor to add the rewards factor to.
	 * @throw	RewardException		The index was invalid.
	 * @return	The rewards for this index.
	 */
	virtual Rewards *get(unsigned int factorIndex);

	/**
	 * Get the number of rewards, i.e., the dimension of the factored rewards vector.
	 * @return	The number of rewards.
	 */
	virtual unsigned int get_num_rewards() const;

	/**
	 * Reset the rewards, freeing all the memory allocated by each object in the vector.
	 */
	virtual void reset();

protected:
	/**
	 * The vector of modifiable reward objects.
	 */
	std::vector<Rewards *> rewards;

};


#endif // FACTORED_REWARDS_H
