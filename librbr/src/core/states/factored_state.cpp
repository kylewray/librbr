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


#include "../../../include/core/states/factored_state.h"
#include "../../../include/core/states/state_exception.h"

FactoredState::FactoredState(int numFactoredStates)
{
	states.reserve(numFactoredStates);
}

FactoredState::FactoredState(const std::vector<const State *> &factoredState)
{
	states.reserve(factoredState.size());
	states = factoredState;
}

FactoredState::FactoredState(const FactoredState &other)
{
	*this = other;
}

FactoredState::~FactoredState()
{ }

void FactoredState::set(const std::vector<const State *> &factoredState)
{
	states = factoredState;
}

const std::vector<const State *> &FactoredState::get() const
{
	return states;
}

const State *FactoredState::get(int index) const
{
	if (index < 0 || index >= states.size()) {
		throw StateException();
	}
	return states[index];
}

int FactoredState::get_num_states() const
{
	return states.size();
}

State &FactoredState::operator=(const State &other)
{
    const FactoredState *s = static_cast<const FactoredState *> (&other);
	states = s->states;
	return *this;
}

std::string FactoredState::to_string() const
{
	std::string jointState = "";
	for (int i = 0; i < states.size(); i++) {
		jointState += states[i]->to_string();
		if (i != states.size() - 1) {
			jointState += " ";
		}
	}
	return "<" + jointState + ">";
}

unsigned int FactoredState::hash_value() const
{
	unsigned int hash = 7;
	for (const State *state : states) {
		hash = 31 * hash + state->hash_value();
	}
	return hash;
}
