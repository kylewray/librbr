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


#ifndef FACTORED_STATE_H
#define FACTORED_STATE_H


#include <string>
#include <vector>

#include "state.h"

/**
 * A factored state object. This is essentially a list of states, each state being a state object itself.
 * Note: This class does *not* manage the memory of the states provided. Memory should be managed in
 * a class such as FiniteFactoredStates.
 */
class FactoredState : public State {
public:
	/**
	 * The constructor of the FactoredState object which allows initial specification of the
	 * number of factored states.
	 * @param	numFactoredStates	The number of factored states in the tuple.
	 */
	FactoredState(int numFactoredStates);

	/**
	 * The constructor of the FactoredState object which allows initial specification of the
	 * actual factored state tuple (vector).
	 * @param	factoredState		The list of states which define this factored state.
	 */
	FactoredState(const std::vector<const State *> &factoredState);

	/**
	 * The copy constructor of the FactoredState object.
	 * @param	other		The factored state to copy.
	 */
	FactoredState(const FactoredState &other);

	/**
	 * The default deconstructor of the FactoredState object.
	 */
	virtual ~FactoredState();

	/**
	 * Set the factored state given a list of states.
	 * @param	factoredState		The list of states which define this factored state.
	 */
	virtual void set(const std::vector<const State *> &factoredState);

	/**
	 * Get the factored state.
	 * @return	The list of states.
	 */
	virtual const std::vector<const State *> &get() const;

	/**
	 * Get a particular state at a index.
	 * @param	index		The index to retrieve the state.
	 * @return	The state at the index provided.
	 */
	virtual const State *get(int index) const;

	/**
	 * Get the number of states within the joint state.
	 * @return	The number of states within the joint state.
	 */
	virtual int get_num_states() const;

	/**
	 * Overload the equals operator to set this factored state equal to the state provided.
	 * @param	other		The factored state to copy.
	 * @return	The new version of this state.
	 */
	virtual State &operator=(const State &other);

	/**
	 * Returns a string representation of this state.
	 * @return	Returns the string representing this state.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this state in a collection of states.
	 * @return	Returns the hash value of this state.
	 */
	virtual unsigned int hash_value() const;

protected:
	/**
	 * The factored state is defined as a *tuple of states*, i.e., a specific factored state. To
	 * remain general, this is a vector, but should remain a fixed size. This class does *not*
	 * manage the memory of these state objects.
	 */
	std::vector<const State *> states;

};


#endif // FACTORED_STATE_H
