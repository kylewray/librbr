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


#include "../../../include/core/states/finite_states.h"
#include "../../../include/core/states/state_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteStates class.
 */
FiniteStates::FiniteStates()
{ }

/**
 * The constructor for the FiniteStates class which allows the specification of an initial set of FiniteStates.
 * @param	FiniteStates	The initial vector of FiniteStates.
 */
FiniteStates::FiniteStates(const std::vector<const State *> &states)
{
	set(states);
}

/**
 * The default deconstructor for the FiniteStates class.
 */
FiniteStates::~FiniteStates()
{
	reset();
}

/**
 * Add a state to the set of available states.
 * @param	newState	The new state to include in the set of available states.
 */
void FiniteStates::add(const State *newState)
{
	states[newState->hash_value()] = newState;
}

/**
 * Remove a state to the set of available states. This frees the memory.
 * @param	removeState 		The state to remove from the set of available states.
 * @throw	StateException		The state was not found in the states list.
 */
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

/**
 * Set the internal states list given another list, performing a deep copy. This resets
 * the current list of states and frees the memory.
 * @param	newStates	The vector of new states to use.
 */
void FiniteStates::set(const std::vector<const State *> &newStates)
{
	reset();
	for (const State *state : newStates) {
		states[state->hash_value()] = state;
	}
}

/**
 * Return the number of states.
 * @return	The number of states.
 */
int FiniteStates::get_num_states() const
{
	return states.size();
}

/**
 * Reset the states, clearing the internal list and freeing the memory.
 */
void FiniteStates::reset()
{
	for (auto state : states) {
		delete state.second;
	}
	states.clear();
}

/**
 * To facilitate easy iteration, return a constant beginning of the states vector.
 * @return	The iterator which points to a constant beginning of the states vector.
 */
std::unordered_map<unsigned int, const State *>::const_iterator FiniteStates::begin() const
{
	return states.begin();
}

/**
 * To facilitate easy iteration, return a constant end of the states vector.
 * @return	The iterator which points to a constant end of the states vector.
 */
std::unordered_map<unsigned int, const State *>::const_iterator FiniteStates::end() const
{
	return states.end();
}

/**
 * Get the state pointer of a state iterator.
 * @param	stateIterator	The state iterator to retrieve the state pointer from.
 */
const State *resolve(std::unordered_map<unsigned int, const State *>::value_type &stateIterator)
{
	return stateIterator.second;
}

/**
 * Get the hash of a state iterator.
 * @param	stateIterator	The state iterator to retrieve the hash value from.
 */
unsigned int hash_value(std::unordered_map<unsigned int, const State *>::value_type &stateIterator)
{
	return stateIterator.first;
}
