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
	 * The default deconstructor for the FiniteStates class.
	 */
	virtual ~FiniteStates();

	/**
	 * Add a state to the set of available states.
	 * @param newState The new state to include in the set of available states.
	 */
	void add(State *newState);

	/**
	 * Remove a state to the set of available states. This frees the memory.
	 * @param removeState The state to remove from the set of available states.
	 */
	void remove(State *removeState);

	/**
	 * Set the internal states list given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory.
	 * @param newStates The vector of new states to use.
	 */
	void set(std::vector<State *> newStates);

	/**
	 * Return a list of all the available states.
	 * @return Return a list of available states.
	 */
	virtual std::vector<State *> all() const;

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * @param state		The previous state.
	 * @param action	The action taken at the previous state.
	 * @return Return a list of available states.
	 *
	 */
	virtual std::vector<State *> available(State *state, Action *action) const;

	/**
	 * Return the number of states.
	 * @return The number of states.
	 */
	virtual int get_num_states() const;

	/**
	 * Get a particular state given the name.
	 * @param stateName The name of the state.
	 * @return The state with the corresponding name provided.
	 * @throws StateException The name was invalid.
	 */
	virtual State *find(std::string stateName);

	/**
	 * Reset the states, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

protected:
	/**
	 * The list of all available states.
	 */
	std::vector<State *> states;

};


#endif // FINITE_STATES_H
