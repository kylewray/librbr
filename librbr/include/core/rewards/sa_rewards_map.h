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


#ifndef SA_REWARDS_MAP_H
#define SA_REWARDS_MAP_H


#include <unordered_map>

#include "sa_rewards.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * A class for state-action rewards in an MDP-like object, internally storing the rewards as
 * a map.
 */
class SARewardsMap : public SARewards {
public:
	/**
	 * The default constructor for the SARewardsMap class.
	 */
	SARewardsMap();

	/**
	 * The default deconstructor for the SARewardsMap class.
	 */
	virtual ~SARewardsMap();

	/**
	 * Set a state transition from a particular state-action pair to a probability.
	 * @param state		The current state of the system.
	 * @param action	The action taken in the current state.
	 * @param reward	The reward from the provided state-action-state triple.
	 */
	virtual void set(const State *state, const Action *action, double reward);

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param state		The current state of the system.
	 * @param action	The action taken in the current state.
	 * @param nextState	The next state with which we assign the reward.
	 * @param reward	The reward from the provided state-action-state triple.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState, double reward);

	/**
	 * Set a state transition from a particular state-action-state-observation quadruple to a probability.
	 * @param state			The current state of the system.
	 * @param action		The action taken in the current state.
	 * @param nextState		The next state with which we assign the reward.
	 * @param observation	The observation made at the next state.
	 * @param reward		The reward from the provided state-action-state-observation quadruple.
	 */
	virtual void set(const State *state, const Action *action, const State *nextState,
			const Observation *observation, double reward);

	/**
	 * The probability of a transition following the state-action pair provided.
	 * @param state		The current state of the system.
	 * @param action	The action taken at the current state.
	 * @return The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action) const;

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param state		The current state of the system.
	 * @param action	The action taken at the current state.
	 * @param nextState	The next state with which we assign the reward.
	 * @return The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState) const;

	/**
	 * The probability of a transition following the state-action-state-observation quadruple provided.
	 * @param state			The current state of the system.
	 * @param action		The action taken in the current state.
	 * @param nextState		The next state with which we assign the reward.
	 * @param observation	The observation made at the next state.
	 * @return The reward from taking the given action in the given state.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState,
			const Observation *observation) const;

	/**
	 * Get the minimal R-value.
	 * @return The minimal R-value.
	 */
	virtual double get_min() const;

	/**
	 * Get the maximal R-value.
	 * @return The maximal R-value.
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
	 * @param state		The current state of the system.
	 * @param action	The action taken at the current state.
	 * @return The reward from taking the given action in the given state.
	 * @throws RewardException The reward was not defined.
	 */
	virtual double get_value(const State *state, const Action *action) const;

	/**
	 * The list of all state-action rewards.
	 */
	std::unordered_map<const State *,
		std::unordered_map<const Action *, double> > rewards;

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

#endif // SA_REWARDS_MAP_H
