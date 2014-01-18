/**
 *  The MIT License (MIT)
 *
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


/**
 * The default constructor for the FiniteActions class.
 */
FiniteActions::FiniteActions()
{ }

/**
 * The default deconstructor for the FiniteActions class.
 */
FiniteActions::~FiniteActions()
{
	actions.clear();
}

/**
 * Add an action to the set of available actions.
 * @param newAction The new action to include in the set of available actions.
 */
void FiniteActions::add(Action newAction)
{
	actions.push_back(newAction);
}

/**
 * Remove an action to the set of available actions.
 * @param removeAction The action to remove from the set of available actions.
 */
void FiniteActions::remove(Action removeAction)
{
	actions.erase(std::remove(actions.begin(), actions.end(), removeAction), actions.end());
}

/**
 * Set the internal actions list given another list, performing a deep copy.
 * @param newActions The vector of new actions to use.
 */
void FiniteActions::set(std::vector<Action> newActions)
{
	actions = newActions;
}

/**
 * Return a list of all the available actions.
 * @return Return a list of available actions.
 */
std::vector<Action> FiniteActions::all() const
{
	return actions;
}

/**
 * Return a list of the actions available given a state.
 * @param state The current state.
 * @return Return a list of available actions.
 */
std::vector<Action> FiniteActions::available(State state) const
{
	return actions;
}
