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


#include "../../../include/core/actions/actions_map.h"
#include "../../../include/core/actions/action_exception.h"

#include <algorithm>

ActionsMap::ActionsMap()
{ }

ActionsMap::ActionsMap(const std::vector<Action *> &actions)
{
	set(actions);
}

ActionsMap::~ActionsMap()
{
	reset();
}

void ActionsMap::add(Action *newAction)
{
	actions[newAction->hash_value()] = newAction;
}

void ActionsMap::remove(Action *removeAction)
{
	std::unordered_map<unsigned int, Action *>::const_iterator result = actions.find(removeAction->hash_value());
	if (result == actions.end()) {
		throw ActionException();
	}

	actions.erase(removeAction->hash_value());
	delete removeAction;
}

void ActionsMap::set(const std::vector<Action *> &newActions)
{
	reset();
	for (Action *action : newActions) {
		actions[action->hash_value()] = action;
	}
}

bool ActionsMap::exists(Action *action) const
{
	try {
		actions.at(action->hash_value());
	} catch (const std::out_of_range &err) {
		return false;
	}
	return true;
}

Action *ActionsMap::get(unsigned int hash)
{
	try {
		return actions.at(hash);
	} catch (const std::out_of_range &err) {
		throw ActionException();
	}
}

unsigned int ActionsMap::get_num_actions() const
{
	return actions.size();
}

std::unordered_map<unsigned int, Action *> ActionsMap::available(State *state)
{
	return actions;
}

void ActionsMap::reset()
{
	for (auto action : actions) {
		delete resolve(action);
	}
	actions.clear();
}

std::unordered_map<unsigned int, Action *>::iterator ActionsMap::begin()
{
	return actions.begin();
}

std::unordered_map<unsigned int, Action *>::iterator ActionsMap::end()
{
	return actions.end();
}

Action *resolve(std::unordered_map<unsigned int, Action *>::value_type &actionIterator)
{
	return actionIterator.second;
}

unsigned int hash_value(std::unordered_map<unsigned int, Action *>::value_type &actionIterator)
{
	return actionIterator.first;
}
