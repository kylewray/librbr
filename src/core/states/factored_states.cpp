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
FactoredState::FactoredState(std::vector<State *> factoredState)
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
 * Override set name (leave get_name() alone) to raise an error when it is called.
 * @param newName The new name.
 * @throws StateException This is no longer a valid function.
 */
void FactoredState::set_name(std::string newName)
{
	throw StateException();
}

/**
 * Set the factored state given a list of states.
 * @param factoredState The list of states which define this factored state.
 */
void FactoredState::set(std::vector<State *> factoredState)
{
	states = factoredState;
}

/**
 * Get the factored state.
 * @return The list of states.
 */
std::vector<State *> FactoredState::get() const
{
	return states;
}

/**
 * Get a particular state at a index.
 * @param index The index to retrieve the state.
 * @return The state at the index provided.
 * @throws StateException The index is not valid.
 */
State *FactoredState::get_state(int index) const
{
	if (index < 0 || index >= states.size()) {
		throw StateException();
	}
	return states[index];
}

/**
 * Overload the equals operator to set this state equal to the state provided.
 * @param other The factored state to copy.
 * @return The new version of this state.
 */
FactoredState &FactoredState::operator=(const FactoredState &other)
{
	states = other.get();
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The factored state to compare.
 * @return Returns @code{true} if this state is equal to the other; @code{false} otherwise.
 */
bool FactoredState::operator==(const FactoredState &other)
{
	int counter = 0;
	for (State *state : states) {
		if (*state == *(other.get_state(counter))) {
			return false;
		}
		counter++;
	}
	return true;
}

/**
 * Overload the less than operator for comparison.
 * @param other The factored state to compare.
 * @return Returns @code{true} if this state is less than the other; @code{false} otherwise.
 */
bool FactoredState::operator<(const FactoredState &other) const
{
	return name < other.get_name();
}
