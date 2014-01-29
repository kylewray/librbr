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


#include "../../../include/core/states/finite_factored_states.h"
#include "../../../include/core/states/factored_state.h"
#include "../../../include/core/states/state_exception.h"


/**
 * The default constructor for the FiniteFactoredStates class.
 */
FiniteFactoredStates::FiniteFactoredStates()
{ }

/**
 * The constructor for the FiniteFactoredStates class which lets you specify the number of factors.
 * @param numFactors The number of state factors.
 */
FiniteFactoredStates::FiniteFactoredStates(int numFactors)
{
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
 * @param newState The new state to include in the set of available states.
 */
void FiniteFactoredStates::add_factor(std::vector<State *> newStates)
{
	factoredStates.push_back(newStates);
}

/**
 * Add a state to the set of available states in a factor.
 * @param factorIndex 		The index of the factor to add the states to.
 * @param newState 			The new state to include in the set of available states.
 * @throws StateException	The index was invalid.
 */
void FiniteFactoredStates::add(int factorIndex, State *newState)
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
 * @throws StateException	The index was invalid.
 */
void FiniteFactoredStates::remove(int factorIndex, State *removeState)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size()) {
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
void FiniteFactoredStates::set(int factorIndex, std::vector<State *> newStates)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size() || newStates.size() == 0) {
		throw StateException();
	}

	// Delete the current factor's states list.
	for (State *state : factoredStates[factorIndex]) {
		delete state;
	}
	factoredStates[factorIndex].clear();

	factoredStates[factorIndex] = newStates;
}

/**
 * Update the internal states list which holds all permutations of factored states in an efficient structure.
 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
 * @throws StateException If a state factor has not been defined.
 */
void FiniteFactoredStates::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<State *> &factor : factoredStates) {
		if (factor.size() == 0) {
			throw StateException();
		}
	}

	std::vector<State *> create;
	update_step(create, 0);
}

/**
 * Get the number of factored states.
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
	for (std::vector<State *> &factor : factoredStates) {
		for (State *state : factor) {
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
void FiniteFactoredStates::update_step(std::vector<State *> currentFactoredState, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of factored states.
	for (State *state : factoredStates[currentFactorIndex]) {
		// Begin by pushing a current factor's state on the vector (tuple).
		currentFactoredState.push_back(state);

		// If this is the final index, then create a factored state object and append it to the list of states.
		// Otherwise, recurse to the next index, using the new currentFactoredState object.
		if (currentFactorIndex == factoredStates.size() - 1) {
			states.push_back(new FactoredState(currentFactoredState));
		} else {
			update_step(currentFactoredState, currentFactorIndex + 1);
		}

		// Pop off the state that was just appended, to prepare for the next loop.
		currentFactoredState.pop_back();
	}
}
