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


#ifndef SASO_REWARDS_ARRAY_H
#define SASO_REWARDS_ARRAY_H


#include "saso_rewards.h"

#include "../states/state.h"
#include "../actions/action.h"
#include "../observations/observation.h"

/**
 * A class for state-action-state-observation rewards in an MDP-like object, internally storing
 * the rewards as an array.
 */
class SASORewardsArray : public SASORewards {
public:
	/**
	 * The default constructor for the SASORewardsArray class. This requires the number of states,
	 * actions, and observations.
	 * @param	numStates		The number of states.
	 * @param	numActions		The number of actions.
	 * @param	numObservations	The number of observations.
	 */
	SASORewardsArray(unsigned int numStates, unsigned int numActions, unsigned int numObservations);

	/**
	 * The default deconstructor for the SASORewardsArray class.
	 */
	virtual ~SASORewardsArray();

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @param	reward			The reward from the provided state-action-state-observation quadruple.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState,
			const Observation *observation, double reward);

	/**
	 * The probability of a transition following the state-action-state-observation quadruple provided.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState,
			const Observation *observation) const;

	/**
	 * Get the memory location of the 4-dimensional array.
	 * @return	A pointer to the raw rewards data.
	 */
	virtual const float ****get_rewards() const;

	/**
	 * Get the number of states used for the rewards array.
	 * @return	The number of states.
	 */
	virtual unsigned int get_num_states() const;

	/**
	 * Get the number of actions used for the rewards array.
	 * @return	The number of actions.
	 */
	virtual unsigned int get_num_actions() const;

	/**
	 * Get the number of observations used for the rewards array.
	 * @return	The number of observations.
	 */
	virtual unsigned int get_num_observations() const;

	/**
	 * Get the minimal R-value.
	 * @return	The minimal R-value.
	 */
	virtual double get_min() const;

	/**
	 * Get the maximal R-value.
	 * @return	The maximal R-value.
	 */
	virtual double get_max() const;

	/**
	 * Reset the rewards by assigning all values to zero. This does not free memory.
	 */
	virtual void reset();

private:
	/**
	 * The 4-dimensional array mapping state-action-state-observations to floats.
	 * Floats were used to improve speed.
	 */
	float ****rewards;

	/**
	 * The number of states, which is the first and third dimension of the rewards array.
	 */
	unsigned int states;

	/**
	 * The number of actions, which is the second dimension of the rewards array.
	 */
	unsigned int actions;

	/**
	 * The number of observations, which is the fourth dimension of the rewards array.
	 */
	unsigned int observations;

	/**
	 * The minimum R-value.
	 */
	float Rmin;

	/**
	 * The maximum R-value.
	 */
	float Rmax;

};


#endif // SASO_REWARDS_ARRAY_H
