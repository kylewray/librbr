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


#ifndef FINITE_STATES_H
#define FINITE_STATES_H


#include <vector>

#include "state.h"
#include "states.h"
#include "../actions/action.h"

/**
 * A class for finite sets of states in an MDP-like object. Informally, there are two basic ways to
 * store finite states: a vector of states or a generator function based on a state and action. In both
 * cases, we require that any class with finite states provide certain get functions so that any generic
 * solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteStates class, please create a child class which
 * implements the function in the virtual functions described below. You will likely ignore the internal
 * states vector variable here.
 */
class FiniteStates : public States {
public:
	/**
	 * The default constructor for the FiniteStates class.
	 */
	FiniteStates();

	/**
	 * The constructor for the FiniteStates class which allows the specification of an initial set of states.
	 * @param states The initial vector of states.
	 */
	FiniteStates(const std::vector<const State *> &states);

	/**
	 * The default deconstructor for the FiniteStates class.
	 */
	virtual ~FiniteStates();

	/**
	 * Add a state to the set of available states.
	 * @param newState The new state to include in the set of available states.
	 */
	void add(const State *newState);

	/**
	 * Remove a state to the set of available states. This frees the memory.
	 * @param removeState 		The state to remove from the set of available states.
	 * @throws StateException	The state was not found in the states list.
	 */
	void remove(const State *removeState);

	/**
	 * Set the internal states list given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory.
	 * @param newStates The vector of new states to use.
	 */
	void set(const std::vector<const State *> &newStates);

	/**
	 * Get the state at the corresponding index. A state's index is defined by the order
	 * in which they are added and removed.
	 * @param stateIndex The index of the state.
	 * @return The state at the corresponding index.
	 * @throws StateException The index was invalid.
	 */
	const State *get(int stateIndex) const;

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * @param state		The previous state.
	 * @param action	The action taken at the previous state.
	 * @return Return a list of available states.
	 */
	virtual const std::vector<const State *> &successor(const State *state, const Action *action) const;

	/**
	 * Return the number of states.
	 * @return The number of states.
	 */
	virtual int get_num_states() const;

	/**
	 * Reset the states, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

	/**
	 * To facilitate easy iteration, return a constant beginning of the states vector.
	 * @return The iterator which points to a constant beginning of the states vector.
	 */
	std::vector<const State *>::const_iterator begin() const;

	/**
	 * To facilitate easy iteration, return a constant end of the states vector.
	 * @return The iterator which points to a constant end of the states vector.
	 */
	std::vector<const State *>::const_iterator end() const;

protected:
	/**
	 * The list of all available states.
	 */
	std::vector<const State *> states;

};


#endif // FINITE_STATES_H
