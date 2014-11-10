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


#ifndef POLICY_MAP_H
#define POLICY_MAP_H


#include <vector>
#include <string>
#include <map>

#include "policy.h"

#include "../states/state.h"
#include "../states/states_map.h"
#include "../actions/action.h"
#include "../actions/actions_map.h"
#include "../horizon.h"

/**
 * A simple map policy which works for MDPs and Dec-MDPs; each actual state
 * deterministically maps to a single action. This will likely be used
 * exclusively for MDPs and Dec-MDPs.
 *
 * Additionally, the class supports finite horizon solutions, too. By default
 * it assumes infinite horizon.
 */
class PolicyMap : virtual public Policy {
public:
	/**
	 * The default constructor for a PolicyMap object. It defaults to a horizon of 1.
	 */
	PolicyMap();

	/**
	 * A constructor for a PolicyMap object which specifies the horizon.
	 * @param	horizon		The horizon of the problem; 0 represents infinite horizon.
	 */
	PolicyMap(unsigned int horizon);

	/**
	 * A constructor for a PolicyMap object which specifies the horizon.
	 * @param	horizon		The horizon object from the MDP-like object.
	 */
	PolicyMap(Horizon *horizon);

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~PolicyMap();

	/**
	 * Set the mapping from a state to an action. For finite horizon, it assumes 0 by default.
	 * @param	state		The state to define.
	 * @param	action		The action which should be taken at the state.
	 */
	void set(State *state, Action *action);

	/**
	 * Set the mapping from a state to an action, allowing the explicit specification of the horizon.
	 * @param	horizon				The horizon to set.
	 * @param	state				The state to define.
	 * @param	action				The action which should be taken at the state.
	 * @throw	PolicyException		The horizon was invalid.
	 */
	void set(unsigned int horizon, State *state, Action *action);

	/**
	 * Get the action for a given state. For finite horizon, it assumes 0 by default.
	 * @param	state				The state to retrieve a mapping.
	 * @throw	PolicyException		The policy was not defined for this state.
	 * @return	The action to take at the given state.
	 */
	Action *get(State *state);

	/**
	 * Get the action for a given state, allowing the explicit specification of the horizon.
	 * @param	horizon				The horizon to set.
	 * @param	state				The state to retrieve a mapping.
	 * @throw	PolicyException		The policy was not defined for this state, or horizon was invalid.
	 * @return	The action to take at the given state.
	 */
	Action *get(unsigned int horizon, State *state);

	/**
	 * A function which must load a policy file.
	 * @param	filename	The name and path of the file to load.
	 * @param	states		The states object which contains the actual state objects to be mapped.
	 * @param	actions		The actions object which contains the actual action objects to be mapped.
	 * @param	horizon		The horizons object to ensure valid policy creation.
	 * @return	Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename, StatesMap *states, ActionsMap *actions, Horizon *horizon);

	/**
	 * A function which must save a policy file.
	 * @param	filename	The name and path of the file to save.
	 * @return	Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename);

	/**
	 * Reset the policy mapping.
	 */
	virtual void reset();

private:
	/**
	 * Defines the policy itself; it's the internal mapping from states to actions. There is
	 * one of these mappings for each horizon.
	 */
	std::vector<std::map<State *, Action *> > policy;

};


#endif // POLICY_MAP_H
