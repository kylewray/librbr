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


#ifndef SAS_REWARDS_MAP_H
#define SAS_REWARDS_MAP_H


#include <unordered_map>

#include "sas_rewards.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * A class for state-action-state rewards in an MDP-like object, internally storing the rewards as
 * a map.
 */
class SASRewardsMap : virtual public SASRewards {
public:
	/**
	 * The default constructor for the SASRewardsMap class.
	 */
	SASRewardsMap();

	/**
	 * The default deconstructor for the SASRewardsMap class.
	 */
	virtual ~SASRewardsMap();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param	state		The current state of the system.
	 * @param	action		The action taken in the current state.
	 * @param	nextState	The next state with which we assign the reward.
	 * @param	reward		The reward from the provided state-action-state triple.
	 */
	virtual void set(State *state, Action *action, State *nextState, double reward);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @param	reward			The reward from the provided state-action-state-observation quadruple.
	 */
	virtual void set(State *state, Action *action, State *nextState,
			Observation *observation, double reward);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param	state		The current state of the system.
	 * @param	action		The action taken at the current state.
	 * @param	nextState	The next state with which we assign the reward.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(State *state, Action *action, State *nextState);

	/**
	 * The probability of a transition following the state-action-state-observation quadruple provided.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken in the current state.
	 * @param	nextState		The next state with which we assign the reward.
	 * @param	observation		The observation made at the next state.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get(State *state, Action *action, State *nextState,
			Observation *observation);

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
	 * Reset the rewards, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The actual get function which returns a value. This will throw an error if the value is undefined.
	 * It is used as a helper function for the public get function.
	 * @param	state				The current state of the system.
	 * @param	action				The action taken at the current state.
	 * @param	nextState			The next state with which we assign the reward.
	 * @throw	RewardException		The reward was not defined.
	 * @return	The reward from taking the given action in the given state.
	 */
	virtual double get_value(State *state, Action *action, State *nextState);

	/**
	 * The list of all state-action-state rewards.
	 */
	std::unordered_map<State *,
		std::unordered_map<Action *,
		std::unordered_map<State *, double> > > rewards;

	/**
	 * A special state (implicitly constant) referring to a state wildcard.
	 */
	State *stateWildcard;

	/**
	 * A special action (implicitly constant) referring to an action wildcard.
	 */
	Action *actionWildcard;

	/**
	 * The minimum R-value.
	 */
	double Rmin;

	/**
	 * The maximum R-value.
	 */
	double Rmax;

};

#endif // SAS_REWARDS_MAP_H
