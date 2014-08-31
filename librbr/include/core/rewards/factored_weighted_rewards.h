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


#ifndef FACTORED_WEIGHTED_REWARDS_H
#define FACTORED_WEIGHTED_REWARDS_H


#include "factored_rewards.h"
#include "sa_rewards.h"

#include "../states/state.h"
#include "../actions/action.h"
#include "../observations/observation.h"

#include <vector>

/**
 * A class for factored rewards in an MDP-like object, i.e., multi-objective MDPs (MOMDPs).
 * This differs from FactoredRewards due to the inclusion of a weight vector used to weigh
 * the components evenly.
 *
 * The class does not re-implement all of the FactoredRewards methods; however, since
 * SASORewards is the most 'ancestral' of the rewards objects, and is abstract, we define
 * those methods.
 *
 * TODO: Inherit from SRewards, once created. Then, implement the missing 'get' and 'set' methods.
 */
class FactoredWeightedRewards : virtual public FactoredRewards, virtual public SARewards {
public:
	/**
	 * The default constructor for the FactoredWeightedRewards class.
	 */
	FactoredWeightedRewards();

	/**
	 * The constructor for the FactoredWeightedRewards class which allows the specification
	 * of the number of rewards.
	 * @param	numRewards		The number of rewards in the multi-rewards vector, minimum of 1.
	 * @param	weights			The weight vector, with dimension equal to numRewards, on the simplex.
	 * @throw	RewardException	The weight dimension is not equal to the number of rewards.
	 */
	FactoredWeightedRewards(unsigned int numRewards, std::vector<double> &weights);

	/**
	 * The default deconstructor for the FactoredWeightedRewards class.
	 */
	virtual ~FactoredWeightedRewards();

	/**
	 * Add a new rewards factor to the vector of available rewards. This relinquishes control of the
	 * new rewards factor to this object. This places the additional requirement that the newRewardsFactor
	 * must be of type SASORewards.
	 * @param	newRewardsFactor	The new rewards factor to include in the vector of available rewards.
	 * @throw	RewardException		The new rewards factor was null.
	 */
	virtual void add_factor(Rewards *newRewardsFactor);

	/**
	 * Set the particular rewards element in the multi-rewards vector. This frees memory. This places the
	 * additional requirement that the newRewardsFactor must be of type S***Rewards.
	 * @param	factorIndex 		The index of the factor to add the rewards factor to.
	 * @param	newRewardsFactor	The new rewards factor for this index.
	 * @throw	RewardException		The index was invalid.
	 */
	virtual void set(unsigned int factorIndex, Rewards *newRewardsFactor);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * In the case of FactoredWeightedRewards, all factors will be set to the reward, so that the sum of
	 * the weighted rewards equals the reward defined here. This method is defined for SARewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	reward			The reward from the provided state-action-state-observation quadruple.
	 * @throw	RewardException	A reward was found which was not of type SARewards.
	 */
	virtual void set(const State *state, const Action *action, double reward);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * In the case of FactoredWeightedRewards, all factors will be set to the reward, so that the sum of
	 * the weighted rewards equals the reward defined here. This method is defined for SARewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	reward			The reward from the provided state-action-state-observation quadruple.
	 * @throw	RewardException	A reward was found which was not of type SASRewards.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState, double reward);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * In the case of FactoredWeightedRewards, all factors will be set to the reward, so that the sum of
	 * the weighted rewards equals the reward defined here. This method is defined for SARewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @param	reward			The reward from the provided state-action-state-observation quadruple.
	 * @throw	RewardException	A reward was found which was not of type SASORewards.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState,
			const Observation *observation, double reward);

	/**
	 * The probability of a transition following the state-action pair provided.
	 * This method is defined for SARewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @throw	RewardException	A reward was found which was not of type SARewards.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action) const;

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * This method is defined for SASRewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @throw	RewardException	A reward was found which was not of type SASRewards.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState) const;

	/**
	 * The probability of a transition following the state-action-state-observation quadruple provided.
	 * This method is defined for SASORewards.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @throw	RewardException	A reward was found which was not of type SASORewards.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState,
			const Observation *observation) const;

	/**
	 * Set the weights for the factored weighted rewards.
	 * @param	weights		The new weight vector.
	 */
	void set_weights(const std::vector<double> &weights);

	/**
	 * Get the weights for the factored weighted rewards.
	 * @return	The weight vector.
	 */
	const std::vector<double> &get_weights() const;

	/**
	 * Get the minimal R-value. This method is defined for SASORewards.
	 * @return	The minimal R-value.
	 */
	virtual double get_min() const;

	/**
	 * Get the maximal R-value. This method is defined for SASORewards.
	 * @return	The maximal R-value.
	 */
	virtual double get_max() const;

	/**
	 * Reset the rewards, clearing the internal mapping. This method is defined for
	 * SASORewards. It calls both FactoredRewards' reset method as well as all factors'
	 * reset methods.
	 */
	virtual void reset();

protected:
	/**
	 * The weight vector used to combine each of the factors into a single value.
	 */
	std::vector<double> w;

};


#endif // FACTORED_WEIGHTED_REWARDS_H
