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


#include "../../../include/core/states/finite_states.h"
#include "../../../include/core/states/state_exception.h"


/**
 * The default constructor for the FiniteStates class.
 */
FiniteStates::FiniteStates()
{ }

/**
 * The constructor for the FiniteStates class which allows the specification of an initial set of FiniteStates.
 * @param FiniteStates The initial vector of FiniteStates.
 */
FiniteStates::FiniteStates(std::vector<State *> states)
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
 * @param newState The new state to include in the set of available states.
 */
void FiniteStates::add(State *newState)
{
	states.push_back(newState);
}

/**
 * Remove a state to the set of available states. This frees the memory.
 * @param removeState 		The state to remove from the set of available states.
 * @throws StateException	The state was not found in the states list.
 */
void FiniteStates::remove(State *removeState)
{
	if (std::find(states.begin(), states.end(), removeState) == states.end()) {
		throw StateException();
	}

	states.erase(std::remove(states.begin(), states.end(), removeState), states.end());
	delete removeState;
}

/**
 * Set the internal states list given another list, performing a deep copy. This resets
 * the current list of states and frees the memory.
 * @param newStates The vector of new states to use.
 */
void FiniteStates::set(std::vector<State *> newStates)
{
	reset();
	states = newStates;
}

/**
 * Get the state at the corresponding index. A state's index is defined by the order
 * in which they are added and removed.
 * @param stateIndex The index of the state.
 * @return The state at the corresponding index.
 * @throws StateException The index was invalid.
 */
State *FiniteStates::get(int stateIndex) const
{
	if (stateIndex < 0 || stateIndex >= states.size()) {
		throw StateException();
	}

	return states[stateIndex];
}

/**
 * Return a list of all the available states.
 * @return Return a list of available states.
 */
std::vector<State *> FiniteStates::all() const
{
	return states;
}

/**
 * Return a list of the states available given a previous state and the action taken there.
 * @param state		The previous state.
 * @param action	The action taken at the previous state.
 * @return Return a list of available states.
 *
 */
std::vector<State *> FiniteStates::available(State *state, Action *action) const
{
	return states;
}

/**
 * Return the number of states.
 * @return The number of states.
 */
int FiniteStates::get_num_states() const
{
	return states.size();
}

/**
 * Get a particular state given the name.
 * @param stateName The name of the state.
 * @return The state with the corresponding name provided.
 * @throws StateException The name was invalid.
 */
State *FiniteStates::find(std::string stateName) const
{
	for (State *state : states) {
		if (state->get_name().compare(stateName) == 0) {
			return state;
		}
	}

	throw StateException();
}

/**
 * Reset the states, clearing the internal list and freeing the memory.
 */
void FiniteStates::reset()
{
	for (State *state : states) {
		delete state;
	}
	states.clear();
}
