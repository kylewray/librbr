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


#include "../../../include/core/states/factored_state.h"
#include "../../../include/core/states/state_exception.h"

/**
 * The constructor of the FactoredState object which allows initial specification of the
 * number of factored states.
 * @param numFactoredStates The number of factored states in the tuple.
 */
FactoredState::FactoredState(int numFactoredStates)
{
	states.reserve(numFactoredStates);
}

/**
 * The constructor of the FactoredState object which allows initial specification of the
 * actual factored state tuple (vector).
 * @param factoredState The list of states which define this factored state.
 */
FactoredState::FactoredState(const std::vector<const State *> &factoredState)
{
	states.reserve(factoredState.size());
	states = factoredState;
}

/**
 * The copy constructor of the FactoredState object.
 * @param other The state to copy.
 */
FactoredState::FactoredState(const FactoredState &other)
{
	*this = other;
}

/**
 * The default deconstructor of the FactoredState object.
 */
FactoredState::~FactoredState()
{ }

/**
 * Set the factored state given a list of states.
 * @param factoredState The list of states which define this factored state.
 */
void FactoredState::set(const std::vector<const State *> &factoredState)
{
	states = factoredState;
}

/**
 * Get the factored state.
 * @return The list of states.
 */
const std::vector<const State *> &FactoredState::get() const
{
	return states;
}

/**
 * Get a particular state at a index.
 * @param index The index to retrieve the state.
 * @return The state at the index provided.
 * @throws StateException The index is not valid.
 */
const State *FactoredState::get(int index) const
{
	if (index < 0 || index >= states.size()) {
		throw StateException();
	}
	return states[index];
}

/**
 * Get the number of states within the joint state.
 * @return The number of states within the joint state.
 */
int FactoredState::get_num_states() const
{
	return states.size();
}

/**
 * Overload the equals operator to set this state equal to the state provided.
 * @param other The factored state to copy.
 * @return The new version of this state.
 */
State &FactoredState::operator=(const State &other)
{
    const FactoredState *s = static_cast<const FactoredState *> (&other);
	states = s->states;
	return *this;
}

/**
 * Returns a string representation of this state.
 * @return Returns the string representing this state.
 */
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

/**
 * Returns a hash value used to quickly identify this state in a collection of states.
 * @returns Returns the hash value of this state.
 */
unsigned int FactoredState::hash_value() const
{
	unsigned int hash = 7;
	for (const State *state : states) {
		hash = 31 * hash + state->hash_value();
	}
	return hash;
}
