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
	update_name();
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
 * Override set name (leave get_name() alone) to raise an error when it is called.
 * @param newName The new name.
 * @throws ActionException This is no longer a valid function.
 */
void JointAction::set_name(std::string newName)
{
	throw ActionException();
}

/**
 * Set the joint action given a list of actions.
 * @param jointAction The list of actions which define this joint action.
 */
void JointAction::set(const std::vector<const Action *> &jointAction)
{
	actions = jointAction;
	update_name();
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
 * Overload the equals operator to set this joint action equal to the state provided.
 * @param other The joint action to copy.
 * @return The new version of this action.
 */
JointAction &JointAction::operator=(const JointAction &other)
{
	actions = other.get();
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The joint action to compare.
 * @return Returns @code{true} if this action is equal to the other; @code{false} otherwise.
 */
bool JointAction::operator==(const JointAction &other)
{
	int counter = 0;
	for (const Action *action : actions) {
		if (*action == *(other.get(counter))) {
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
bool JointAction::operator<(const JointAction &other) const
{
	return name < other.get_name();
}

/**
 * A helper function to compute the name of the joint action, once the actions are set.
 */
void JointAction::update_name()
{
	name = "";

	int counter = 0;
	for (const Action *action : actions) {
		name += action->get_name();

		counter++;
		if (counter < actions.size()) {
			name += " ";
		}
	}
}
