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


#include "../../../include/core/actions/joint_action.h"
#include "../../../include/core/actions/action_exception.h"

/**
 * The constructor of the JointAction object which allows initial specification of the
 * number of joint actions.
 * @param numJointActions The number of joint actions in the tuple.
 */
JointAction::JointAction(int numJointActions)
{
	actions.reserve(numJointActions);
}

/**
 * The constructor of the JointAction object which allows initial specification of the
 * actual joint action tuple (vector).
 * @param jointState The list of actions which define this joint action.
 */
JointAction::JointAction(const std::vector<const Action *> &jointAction)
{
	actions.reserve(jointAction.size());
	actions = jointAction;
}

/**
 * The copy constructor of the JointAction object.
 * @param other The joint action to copy.
 */
JointAction::JointAction(const JointAction &other)
{
	*this = other;
}

/**
 * The default deconstructor of the JointAction object.
 */
JointAction::~JointAction()
{ }

/**
 * Set the joint action given a list of actions.
 * @param jointAction The list of actions which define this joint action.
 */
void JointAction::set(const std::vector<const Action *> &jointAction)
{
	actions = jointAction;
}

/**
 * Get the joint action.
 * @return The list of actions.
 */
const std::vector<const Action *> &JointAction::get() const
{
	return actions;
}

/**
 * Get a particular action at a index.
 * @param index The index to retrieve the action.
 * @return The action at the index provided.
 */
const Action *JointAction::get(int index) const
{
	if (index < 0 || index >= actions.size()) {
		throw ActionException();
	}
	return actions[index];
}

/**
 * Get the number of actions within the joint action.
 * @return The number of actions within the joint action.
 */
int JointAction::get_num_actions() const
{
	return actions.size();
}

/**
 * Overload the equals operator to set this joint action equal to the state provided.
 * @param other The joint action to copy.
 * @return The new version of this action.
 */
JointAction &JointAction::operator=(const Action &other)
{
    const JointAction *a = static_cast<const JointAction*>(&other);
	actions = a->get();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The joint action to compare.
 * @return Returns @code{true} if this action is equal to the other; @code{false} otherwise.
 */
bool JointAction::operator==(const Action &other) const
{
    const JointAction *a = static_cast<const JointAction*>(&other);
	int counter = 0;
	for (const Action *action : actions) {
		if (*action == *(a->get(counter))) {
			return false;
		}
		counter++;
	}
	return true;
}

/**
 * Overload the less than operator for comparison.
 * @param other The joint action to compare.
 * @return Returns @code{true} if this action is less than the other; @code{false} otherwise.
 */
bool JointAction::operator<(const Action &other) const
{
	return hash_value() < other.hash_value();
}

/**
 * Returns a string representation of this action.
 * @return Returns the string representing this action.
 */
std::string JointAction::to_string() const
{
	std::string jointAction = "";
	for (int i = 0; i < actions.size(); i++) {
		jointAction += actions[i]->to_string();
		if (i != actions.size() - 1) {
			jointAction += " ";
		}
	}
	return "<" + jointAction + ">";
}

/**
 * Returns a hash value used to quickly identify this action in a collection of actions.
 * @returns Returns the hash value of this action.
 */
int JointAction::hash_value() const
{
	int hash = 7;
	for (const Action *action : actions) {
		hash = 31 * hash + action->hash_value();
	}
	return hash;
}
