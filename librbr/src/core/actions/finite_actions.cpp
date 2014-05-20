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


#include "../../../include/core/actions/finite_actions.h"
#include "../../../include/core/actions/action_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteActions class.
 */
FiniteActions::FiniteActions()
{ }

/**
 * The constructor for the FiniteActions class which allows the specification of an initial set of actions.
 * @param actions The initial vector of actions.
 */
FiniteActions::FiniteActions(const std::vector<const Action *> &actions)
{
	set(actions);
}

/**
 * The default deconstructor for the FiniteActions class.
 */
FiniteActions::~FiniteActions()
{
	reset();
}

/**
 * Add an action to the set of available actions.
 * @param newAction The new action to include in the set of available actions.
 */
void FiniteActions::add(const Action *newAction)
{
	actions.push_back(newAction);
}

/**
 * Remove an action to the set of available actions. This frees the memory.
 * @param removeAction		The action to remove from the set of available actions.
 * @throws ActionException	The action was not found in the actions list.
 */
void FiniteActions::remove(const Action *removeAction)
{
	if (std::find(actions.begin(), actions.end(), removeAction) == actions.end()) {
		throw ActionException();
	}

	actions.erase(std::remove(actions.begin(), actions.end(), removeAction), actions.end());
	delete removeAction;
}

/**
 * Set the internal actions list given another list, performing a deep copy. This resets
 * the current list of states and frees the memory.
 * @param newActions The vector of new actions to use.
 */
void FiniteActions::set(const std::vector<const Action *> &newActions)
{
	reset();
	actions = newActions;
}

/**
 * Get the action at the corresponding index. An action's index is defined by the order
 * in which they are added and removed.
 * @param actionIndex The index of the action.
 * @return The action at the corresponding index.
 * @throws ActionException The index was invalid.
 */
const Action *FiniteActions::get(int actionIndex) const
{
	if (actionIndex < 0 || actionIndex >= actions.size()) {
		throw ActionException();
	}

	return actions[actionIndex];
}

/**
 * Return a list of the actions available given a state.
 * @param state The current state.
 * @return Return a list of available actions.
 */
const std::vector<const Action *> &FiniteActions::available(const State *state) const
{
	return actions;
}

/**
 * Return the number of actions.
 * @return The number of actions.
 */
int FiniteActions::get_num_actions() const
{
	return actions.size();
}

/**
 * Reset the actions, clearing the internal list and freeing the memory.
 */
void FiniteActions::reset()
{
	for (const Action *action : actions) {
		delete action;
	}
	actions.clear();
}

/**
 * To facilitate easy iteration, return a constant beginning of the actions vector.
 * @return The iterator which points to a constant beginning of the actions vector.
 */
std::vector<const Action *>::const_iterator FiniteActions::begin() const
{
	return actions.begin();
}

/**
 * To facilitate easy iteration, return a constant end of the actions vector.
 * @return The iterator which points to a constant end of the actions vector.
 */
std::vector<const Action *>::const_iterator FiniteActions::end() const
{
	return actions.end();
}
