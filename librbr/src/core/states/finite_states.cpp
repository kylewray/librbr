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


#include "../../../include/core/states/finite_states.h"
#include "../../../include/core/states/state_exception.h"

#include <algorithm>

FiniteStates::FiniteStates()
{ }

FiniteStates::FiniteStates(const std::vector<const State *> &states)
{
	set(states);
}

FiniteStates::~FiniteStates()
{
	reset();
}

void FiniteStates::add(const State *newState)
{
	states[newState->hash_value()] = newState;
}

void FiniteStates::remove(const State *removeState)
{
	// Ensure that the element exists in the hash before removing it.
	std::unordered_map<unsigned int, const State *>::const_iterator result = states.find(removeState->hash_value());
	if (result == states.end()) {
		throw StateException();
	}

	states.erase(removeState->hash_value());
	delete removeState;
}

void FiniteStates::set(const std::vector<const State *> &newStates)
{
	reset();
	for (const State *state : newStates) {
		states[state->hash_value()] = state;
	}
}

int FiniteStates::get_num_states() const
{
	return states.size();
}

void FiniteStates::reset()
{
	for (auto state : states) {
		delete state.second;
	}
	states.clear();
}

std::unordered_map<unsigned int, const State *>::const_iterator FiniteStates::begin() const
{
	return states.begin();
}

std::unordered_map<unsigned int, const State *>::const_iterator FiniteStates::end() const
{
	return states.end();
}

const State *resolve(std::unordered_map<unsigned int, const State *>::value_type &stateIterator)
{
	return stateIterator.second;
}

unsigned int hash_value(std::unordered_map<unsigned int, const State *>::value_type &stateIterator)
{
	return stateIterator.first;
}
