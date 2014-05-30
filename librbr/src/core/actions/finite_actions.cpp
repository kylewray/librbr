/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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

FiniteActions::FiniteActions()
{ }

FiniteActions::FiniteActions(const std::vector<const Action *> &actions)
{
	set(actions);
}

FiniteActions::~FiniteActions()
{
	reset();
}

void FiniteActions::add(const Action *newAction)
{
	actions.push_back(newAction);
}

void FiniteActions::remove(const Action *removeAction)
{
	if (std::find(actions.begin(), actions.end(), removeAction) == actions.end()) {
		throw ActionException();
	}

	actions.erase(std::remove(actions.begin(), actions.end(), removeAction), actions.end());
	delete removeAction;
}

void FiniteActions::set(const std::vector<const Action *> &newActions)
{
	reset();
	actions = newActions;
}

const Action *FiniteActions::get(int actionIndex) const
{
	if (actionIndex < 0 || actionIndex >= actions.size()) {
		throw ActionException();
	}

	return actions[actionIndex];
}

const std::vector<const Action *> &FiniteActions::available(const State *state) const
{
	return actions;
}

int FiniteActions::get_num_actions() const
{
	return actions.size();
}

void FiniteActions::reset()
{
	for (const Action *action : actions) {
		delete action;
	}
	actions.clear();
}

std::vector<const Action *>::const_iterator FiniteActions::begin() const
{
	return actions.begin();
}

std::vector<const Action *>::const_iterator FiniteActions::end() const
{
	return actions.end();
}
