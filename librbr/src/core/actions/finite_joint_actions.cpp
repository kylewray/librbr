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


#include "../../../include/core/actions/finite_joint_actions.h"
#include "../../../include/core/actions/joint_action.h"
#include "../../../include/core/actions/action_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteJointActions class which requires that you
 * specify the number of factors.
 * @param numFactors The number of action factors.
 */
FiniteJointActions::FiniteJointActions(int numFactors)
{
	factoredActions.resize(numFactors);
}

/**
 * The default deconstructor for the FiniteJointActions class.
 */
FiniteJointActions::~FiniteJointActions()
{
	reset();
}

/**
 * Add a action to the set of available actions in a factor. This does *not* update the
 * actions list; please call update() once all factors have been set.
 * @param factorIndex 		The index of the factor to add the actions to.
 * @param newAction			The new actions to include in the set of available actions.
 * @throws ActionException	The index was invalid.
 */
void FiniteJointActions::add(int factorIndex, const Action *newAction)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size()) {
		throw ActionException();
	}

	factoredActions[factorIndex].push_back(newAction);
}

/**
 * Remove an action to the set of available actions in a factor. This frees the memory. This does *not*
 * update the actions list; please call update() once all factors have been set.
 * @param factorIndex 		The index of the factor to add the actions to.
 * @param removeAction 		The action to remove from the set of available actions.
 * @throws ActionException	The index was invalid, or the action was not found in the actions list.
 */
void FiniteJointActions::remove(int factorIndex, const Action *removeAction)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size()) {
		throw ActionException();
	} else if (std::find(factoredActions[factorIndex].begin(), factoredActions[factorIndex].end(),
			removeAction) == factoredActions[factorIndex].end()) {
		throw ActionException();
	}

	factoredActions[factorIndex].erase(
			std::remove(factoredActions[factorIndex].begin(), factoredActions[factorIndex].end(), removeAction),
			factoredActions[factorIndex].end());
	delete removeAction;
}

/**
 * Set the internal actions list for a factor given another list, performing a deep copy. This resets
 * the current list of actions and frees the memory. This does *not* update the actions list; please
 * call update() once all factors have been set.
 * @param factorIndex 		The index of the factor to add the actions to.
 * @param newActions 		The vector of new actions to use.
 * @throws ActionException	The index was invalid, or newActions was empty.
 */
void FiniteJointActions::set(int factorIndex, const std::vector<const Action *> &newActions)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size() || newActions.size() == 0) {
		throw ActionException();
	}

	// Delete the current factor's actions list.
	for (const Action *action : factoredActions[factorIndex]) {
		delete action;
	}
	factoredActions[factorIndex].clear();

	factoredActions[factorIndex] = newActions;
}

/**
 * Get the action at the corresponding index, given the particular factor. The factor index
 * is defined by the agent, and an action's index is defined by the order in which they are
 * added and removed.
 * @param factorIndex The index of the factor.
 * @param actionIndex The index of the action.
 * @return The action at the corresponding index.
 * @throws ActionException The index was invalid.
 */
const Action *FiniteJointActions::get(int factorIndex, int actionIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size() ||
			actionIndex < 0 || actionIndex >= factoredActions[factorIndex].size()) {
		throw ActionException();
	}

	return factoredActions[factorIndex][actionIndex];
}

/**
 * Update the internal actions list which holds all permutations of joint actions in an efficient structure.
 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
 * @throws ActionException If a action factor has not been defined.
 */
void FiniteJointActions::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<const Action *> &factor : factoredActions) {
		if (factor.size() == 0) {
			throw ActionException();
		}
	}

	actions.clear();

	std::vector<const Action *> create;
	update_step(create, 0);
}

/**
 * Get the number of factored actions.
 */
int FiniteJointActions::get_num_factors()
{
	return factoredActions.size();
}

/**
 * Reset the joint actions, clearing the internal list and freeing the memory.
 */
void FiniteJointActions::reset()
{
	for (std::vector<const Action *> &factor : factoredActions) {
		for (const Action *action : factor) {
			delete action;
		}
		factor.clear();
	}

	actions.clear();
}

/**
 * A helper function for updating the internal "actions" variable as part of the update() function.
 * @param currentJointAction	The current (incomplete) joint action as a vector of actions.
 * @param currentFactorIndex	The current factor index.
 */
void FiniteJointActions::update_step(std::vector<const Action *> currentJointAction, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of joint actions.
	for (const Action *action : factoredActions[currentFactorIndex]) {
		// Begin by pushing a current factor's action on the vector (tuple).
		currentJointAction.push_back(action);

		// If this is the final index, then create a joint action object and append it to the list of actions.
		// Otherwise, recurse to the next index, using the new currentJointAction object.
		if (currentFactorIndex == factoredActions.size() - 1) {
			actions.push_back(new JointAction(currentJointAction));
		} else {
			update_step(currentJointAction, currentFactorIndex + 1);
		}

		// Pop off the action that was just appended, to prepare for the next loop.
		currentJointAction.pop_back();
	}
}
