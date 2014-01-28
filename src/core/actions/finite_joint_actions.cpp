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
void FiniteJointActions::add(int factorIndex, Action *newAction)
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
 * @throws ActionException	The index was invalid.
 */
void FiniteJointActions::remove(int factorIndex, Action *removeAction)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size()) {
		throw ActionException();
	}

	factoredActions[factorIndex].erase(
			std::remove(factoredActions[factorIndex].begin(), factoredActions[factorIndex].end(), removeAction),
			factoredActions[factorIndex].end());
	delete removeAction;
}

/**
 * Set the internal states list for a factor given another list, performing a deep copy. This resets
 * the current list of states and frees the memory. This does *not* update the states list; please
 * call update() once all factors have been set.
 * @param factorIndex 		The index of the factor to add the actions to.
 * @param newActions 		The vector of new actions to use.
 * @throws ActionException	The index was invalid, or newActions was empty.
 */
void FiniteJointActions::set(int factorIndex, std::vector<Action *> newActions)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size() || newActions.size() == 0) {
		throw ActionException();
	}

	// Delete the current factor's actions list.
	for (Action *action : factoredActions[factorIndex]) {
		delete action;
	}
	factoredActions[factorIndex].clear();

	factoredActions[factorIndex] = newActions;
}

/**
 * Update the internal actions list which holds all permutations of joint actions in an efficient structure.
 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
 * @throws ActionException If a action factor has not been defined.
 */
void FiniteJointActions::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<Action *> &factor : factoredActions) {
		if (factor.size() == 0) {
			throw ActionException();
		}
	}

	std::vector<Action *> create;
	update_step(create, 0);
}

/**
 * Reset the joint actions, clearing the internal list and freeing the memory.
 */
void FiniteJointActions::reset()
{
	for (std::vector<Action *> &factor : factoredActions) {
		for (Action *action : factor) {
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
void FiniteJointActions::update_step(std::vector<Action *> currentJointAction, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of joint actions.
	for (Action *action : factoredActions[currentFactorIndex]) {
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