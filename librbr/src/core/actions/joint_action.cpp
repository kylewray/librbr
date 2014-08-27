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


#include "../../../include/core/actions/joint_action.h"
#include "../../../include/core/actions/action_exception.h"

JointAction::JointAction(int numJointActions)
{
	actions.reserve(numJointActions);
}

JointAction::JointAction(const std::vector<const Action *> &jointAction)
{
	actions.reserve(jointAction.size());
	actions = jointAction;
}

JointAction::JointAction(const JointAction &other)
{
	*this = other;
}

JointAction::~JointAction()
{ }

void JointAction::set(const std::vector<const Action *> &jointAction)
{
	actions = jointAction;
}

const std::vector<const Action *> &JointAction::get() const
{
	return actions;
}

const Action *JointAction::get(int index) const
{
	if (index < 0 || index >= actions.size()) {
		throw ActionException();
	}
	return actions[index];
}

int JointAction::get_num_actions() const
{
	return actions.size();
}

Action &JointAction::operator=(const Action &other)
{
    const JointAction *a = dynamic_cast<const JointAction*>(&other);
    if (a == nullptr) {
    	throw ActionException();
    }
	actions = a->get();
	return *this;
}

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

unsigned int JointAction::hash_value() const
{
	unsigned int hash = 7;
	for (const Action *action : actions) {
		hash = 31 * hash + action->hash_value();
	}
	return hash;
}
