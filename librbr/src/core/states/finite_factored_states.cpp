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


#include "../../../include/core/states/finite_factored_states.h"
#include "../../../include/core/states/factored_state.h"
#include "../../../include/core/states/state_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteFactoredStates class.
 */
FiniteFactoredStates::FiniteFactoredStates()
{ }

/**
 * The constructor for the FiniteFactoredStates class which lets you specify the number of factors.
 * @param numFactors The number of state factors, minimum of 1.
 */
FiniteFactoredStates::FiniteFactoredStates(int numFactors)
{
	if (numFactors < 1) {
		numFactors = 1;
	}
	factoredStates.resize(numFactors);
}

/**
 * The default deconstructor for the FiniteFactoredStates class.
 */
FiniteFactoredStates::~FiniteFactoredStates()
{
	reset();
}

/**
 * Add a new factor states to the set of available states in a factor. This does *not* update the
 * states list; please call update() once all factors have been set.
 * @param newStates			The new states to include in the set of available states.
 * @throws StateException	The vector was empty, or contained null pointers.
 */
void FiniteFactoredStates::add_factor(const std::vector<const State *> &newStates)
{
	if (newStates.size() == 0) {
		throw StateException();
	}

	for (const State *state : newStates) {
		if (state == nullptr) {
			throw StateException();
		}
	}

	factoredStates.push_back(newStates);
}

/**
 * Add a state to the set of available states in a factor.
 * @param factorIndex 		The index of the factor to add the states to.
 * @param newState 			The new state to include in the set of available states.
 * @throws StateException	The index was invalid.
 */
void FiniteFactoredStates::add(int factorIndex, const State *newState)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size()) {
		throw StateException();
	}

	factoredStates[factorIndex].push_back(newState);
}

/**
 * Remove a state to the set of available states in a factor. This frees the memory.
 * @param factorIndex 		The index of the factor to add the states to.
 * @param removeState 		The state to remove from the set of available states.
 * @throws StateException	The index was invalid, or the state was not found in the states list.
 */
void FiniteFactoredStates::remove(int factorIndex, const State *removeState)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size()) {
		throw StateException();
	} else if (std::find(factoredStates[factorIndex].begin(), factoredStates[factorIndex].end(),
			removeState) == factoredStates[factorIndex].end()) {
		throw StateException();
	}

	factoredStates[factorIndex].erase(
			std::remove(factoredStates[factorIndex].begin(), factoredStates[factorIndex].end(), removeState),
			factoredStates[factorIndex].end());
	delete removeState;
}

/**
 * Set the internal states list for a factor given another list, performing a deep copy. This resets
 * the current list of states and frees the memory. This does *not* update the states list; please
 * call update() once all factors have been set.
 * @param factorIndex 		The index of the factor to add the states to.
 * @param newStates 		The vector of new states to use.
 * @throws StateException	The index was invalid, or newStates was empty.
 */
void FiniteFactoredStates::set(int factorIndex, const std::vector<const State *> &newStates)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size() || newStates.size() == 0) {
		throw StateException();
	}

	// Delete the current factor's states list.
	for (const State *state : factoredStates[factorIndex]) {
		delete state;
	}
	factoredStates[factorIndex].clear();

	factoredStates[factorIndex] = newStates;
}

/**
 * Get the state at the corresponding index, given the particular factor. The factor index
 * is defined by the number of factored states, and an state's index is defined by the
 * order in which they are added and removed.
 * @param factorIndex THe index of the factor.
 * @param stateIndex The index of the state.
 * @return The state at the corresponding index.
 * @throws StateException The index was invalid.
 */
const State *FiniteFactoredStates::get(int factorIndex, int stateIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size() ||
			stateIndex < 0 || stateIndex >= factoredStates[factorIndex].size()) {
		throw StateException();
	}

	return factoredStates[factorIndex][stateIndex];
}

/**
 * Update the internal states list which holds all permutations of factored states in an efficient structure.
 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
 * @throws StateException If a state factor has not been defined.
 */
void FiniteFactoredStates::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<const State *> &factor : factoredStates) {
		if (factor.size() == 0) {
			throw StateException();
		}
	}

	states.clear();

	std::vector<const State *> create;
	update_step(create, 0);
}

/**
 * Get the number of factored states.
 * @return The number of factors.
 */
int FiniteFactoredStates::get_num_factors()
{
	return factoredStates.size();
}

/**
 * Reset the factored states, clearing the internal list and freeing the memory.
 */
void FiniteFactoredStates::reset()
{
	for (std::vector<const State *> &factor : factoredStates) {
		for (const State *state : factor) {
			delete state;
		}
		factor.clear();
	}

	states.clear();
}

/**
 * A helper function for updating the internal "states" variable as part of the update() function.
 * @param currentFactoredState	The current (incomplete) factored state as a vector of states.
 * @param currentFactorIndex	The current factor index.
 */
void FiniteFactoredStates::update_step(std::vector<const State *> currentFactoredState, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of factored states.
	for (const State *state : factoredStates[currentFactorIndex]) {
		// Begin by pushing a current factor's state on the vector (tuple).
		currentFactoredState.push_back(state);

		// If this is the final index, then create a factored state object and append it to the list of states.
		// Otherwise, recurse to the next index, using the new currentFactoredState object.
		if (currentFactorIndex == factoredStates.size() - 1) {
			const FactoredState *newState = new FactoredState(currentFactoredState);
			states[newState->hash_value()] = newState;
		} else {
			update_step(currentFactoredState, currentFactorIndex + 1);
		}

		// Pop off the state that was just appended, to prepare for the next loop.
		currentFactoredState.pop_back();
	}
}
