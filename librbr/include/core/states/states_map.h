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


#ifndef STATES_MAP_H
#define STATES_MAP_H


#include <unordered_map>
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
 * If you want to create a generator function-based StatesMap class, please create a child class which
 * implements the function in the virtual functions described below. You will likely ignore the internal
 * states vector variable here.
 */
class StatesMap : virtual public States {
public:
	/**
	 * The default constructor for the StatesMap class.
	 */
	StatesMap();

	/**
	 * The constructor for the StatesMap class which allows the specification of an initial set of states.
	 * @param	states	The initial vector of states.
	 */
	StatesMap(const std::vector<State *> &states);

	/**
	 * The default deconstructor for the StatesMap class.
	 */
	virtual ~StatesMap();

	/**
	 * Add a state to the set of available states.
	 * @param	newState	The new state to include in the set of available states.
	 */
	void add(State *newState);

	/**
	 * Remove a state to the set of available states. This frees the memory.
	 * @param	removeState 		The state to remove from the set of available states.
	 * @throw	StateException		The state was not found in the states list.
	 */
	void remove(State *removeState);

	/**
	 * Set the internal states list given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory.
	 * @param	newStates	The vector of new states to use.
	 */
	void set(const std::vector<State *> &newStates);

	/**
	 * Check if this state has already been created or not.
	 * @param	state		The state to check if it is created or not.
	 * @return	Returns @code{true} if the state exists in the states hash; @code{false} otherwise.
	 */
	bool exists(const State *state) const;

	/**
	 * Get a state with a particular hash value.
	 * @param	hash				The hash of the state.
	 * @throw	StateException		There are no states with the hash value specified.
	 * @return	The state with the particular hash value specified.
	 */
	State *get(unsigned int hash);

	/**
	 * Return the number of states.
	 * @return	The number of states.
	 */
	unsigned int get_num_states() const;

	/**
	 * Reset the states, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

	/**
	 * To facilitate easy iteration, return a constant beginning of the states vector.
	 * @return	The iterator which points to a constant beginning of the states vector.
	 */
	std::unordered_map<unsigned int, State *>::iterator begin();

	/**
	 * To facilitate easy iteration, return a constant end of the states vector.
	 * @return	The iterator which points to a constant end of the states vector.
	 */
	std::unordered_map<unsigned int, State *>::iterator end();

protected:
	/**
	 * The mapping of state hash values to states. This is the main container of states.
	 */
	std::unordered_map<unsigned int, State *> states;

};

/**
 * Get the state pointer of a state iterator.
 * @param	stateIterator	The state iterator to retrieve the state pointer from.
 */
State *resolve(std::unordered_map<unsigned int, State *>::value_type &stateIterator);

/**
 * Get the hash of a state iterator.
 * @param	stateIterator	The state iterator to retrieve the hash value from.
 */
unsigned int hash_value(std::unordered_map<unsigned int, State *>::value_type &stateIterator);


#endif // STATES_MAP_H
