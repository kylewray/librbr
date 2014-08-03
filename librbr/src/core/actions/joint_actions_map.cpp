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


#include "../../../include/core/actions/joint_actions_map.h"
#include "../../../include/core/actions/joint_action.h"
#include "../../../include/core/actions/action_exception.h"

#include <algorithm>

JointActionsMap::JointActionsMap(int numFactors)
{
	if (numFactors < 1) {
		numFactors = 1;
	}
	factoredActions.resize(numFactors);
}

JointActionsMap::~JointActionsMap()
{
	reset();
}

void JointActionsMap::add(int factorIndex, const Action *newAction)
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size()) {
		throw ActionException();
	}

	factoredActions[factorIndex].push_back(newAction);
}

void JointActionsMap::remove(int factorIndex, const Action *removeAction)
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

void JointActionsMap::set(int factorIndex, const std::vector<const Action *> &newActions)
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

const Action *JointActionsMap::get(int factorIndex, int actionIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredActions.size() ||
			actionIndex < 0 || actionIndex >= factoredActions[factorIndex].size()) {
		throw ActionException();
	}

	return factoredActions[factorIndex][actionIndex];
}

void JointActionsMap::update()
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

unsigned int JointActionsMap::get_num_factors()
{
	return factoredActions.size();
}

void JointActionsMap::reset()
{
	for (std::vector<const Action *> &factor : factoredActions) {
		for (const Action *action : factor) {
			delete action;
		}
		factor.clear();
	}

	actions.clear();
}

void JointActionsMap::update_step(std::vector<const Action *> currentJointAction, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of joint actions.
	for (const Action *action : factoredActions[currentFactorIndex]) {
		// Begin by pushing a current factor's action on the vector (tuple).
		currentJointAction.push_back(action);

		// If this is the final index, then create a joint action object and append it to the list of actions.
		// Otherwise, recurse to the next index, using the new currentJointAction object.
		if (currentFactorIndex == factoredActions.size() - 1) {
			const JointAction *newAction = new JointAction(currentJointAction);
			actions[newAction->hash_value()] = newAction;
		} else {
			update_step(currentJointAction, currentFactorIndex + 1);
		}

		// Pop off the action that was just appended, to prepare for the next loop.
		currentJointAction.pop_back();
	}
}
