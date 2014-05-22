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


#ifndef FINITE_FACTORED_STATES_H
#define FINITE_FACTORED_STATES_H


#include <vector>

#include "state.h"
#include "states.h"
#include "finite_states.h"

/**
 * A class for finite sets of factored states in an MDP-like object. Informally, there are two basic ways to
 * store finite states: a vector of states or a generator function based on a state and action. In both
 * cases, we require that any class with finite states provide certain get functions so that any generic
 * solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteFactoredStates class, please create a child class which
 * implements the function in the virtual functions described below. You will likely ignore the internal
 * states vector variable here.
 *
 * Note: The protected "states" variable implicitly must only store FactoredState objects, not State objects.
 */
class FiniteFactoredStates : public FiniteStates {
public:
	/**
	 * The default constructor for the FiniteFactoredStates class.
	 */
	FiniteFactoredStates();

	/**
	 * The constructor for the FiniteFactoredStates class which requires that you
	 * specify the number of factors.
	 * @param numFactors The number of state factors, minimum of 1.
	 */
	FiniteFactoredStates(int numFactors);

	/**
	 * The default deconstructor for the FiniteFactoredStates class.
	 */
	virtual ~FiniteFactoredStates();

	/**
	 * Add a new factor states to the set of available states in a factor. This does *not* update the
	 * states list; please call update() once all factors have been set.
	 * @param newStates			The new states to include in the set of available states.
	 * @throws StateException	The vector was empty, or contained null pointers.
	 */
	void add_factor(const std::vector<const State *> &newStates);

	/**
	 * Add a state to the set of available states in a factor. This does *not* update the states list; please
	 * call update() once all factors have been set.
	 * @param factorIndex 		The index of the factor to add the states to.
	 * @param newState 			The new state to include in the set of available states.
	 * @throws StateException	The index was invalid.
	 */
	void add(int factorIndex, const State *newState);

	/**
	 * Remove a state to the set of available states in a factor. This frees the memory. This does *not*
	 * update the states list; please call update() once all factors have been set.
	 * @param factorIndex 		The index of the factor to add the states to.
	 * @param removeState 		The state to remove from the set of available states.
	 * @throws StateException	The index was invalid, or the state was not found in the states list.
	 */
	void remove(int factorIndex, const State *removeState);

	/**
	 * Set the internal states list for a factor given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory. This does *not* update the states list; please
	 * call update() once all factors have been set.
	 * @param factorIndex 		The index of the factor to add the states to.
	 * @param newStates 		The vector of new states to use.
	 * @throws StateException	The index was invalid, or newStates was empty.
	 */
	void set(int factorIndex, const std::vector<const State *> &newStates);

	/**
	 * Get the state at the corresponding index, given the particular factor. The factor index
	 * is defined by the number of factored states, and an state's index is defined by the
	 * order in which they are added and removed.
	 * @param factorIndex The index of the factor.
	 * @param stateIndex The index of the state.
	 * @return The state at the corresponding index.
	 * @throws StateException The index was invalid.
	 */
	const State *get(int factorIndex, int stateIndex) const;

	/**
	 * Update the internal states list which holds all permutations of factored states in an efficient structure.
	 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
	 * @throws StateException If a state factor has not been defined.
	 */
	void update();

	/**
	 * Get the number of factored states.
	 */
	int get_num_factors();

	/**
	 * Reset the factored states, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

	/**
	 * Get the factored state at the corresponding index.
	 * @param stateIndex The index of the state.
	 * @return The state at the corresponding index.
	 * @throws StateException The index was invalid.
	 */
	using FiniteStates::get;

protected:
	/**
	 * The list of all available states for each state factor.
	 */
	std::vector<std::vector<const State *> > factoredStates;

private:
	/**
	 * A helper function for updating the internal "states" variable as part of the update() function.
	 * @param currentFactoredState	The current (incomplete) factored state as a vector of states.
	 * @param currentFactorIndex	The current factor index.
	 */
	void update_step(std::vector<const State *> currentFactoredState, int currentFactorIndex);

};


#endif // FINITE_FACTORED_STATES_H
