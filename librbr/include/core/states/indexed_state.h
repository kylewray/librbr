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


#ifndef INDEXED_STATE_H
#define INDEXED_STATE_H


#include <string>

#include "state.h"

/**
 * A state object identified by a unique index which is determined using a static variable,
 * incremented each time a new IndexedState is created.
 */
class IndexedState : virtual public State {
public:
	/**
	 * The default constructor of the IndexedState object.
	 */
	IndexedState();

	/**
	 * The copy constructor of the IndexedState object.
	 * @param	other		The state to copy.
	 */
	IndexedState(const IndexedState &other);

	/**
	 * The default deconstructor of the IndexedState object.
	 */
	virtual ~IndexedState();

	/**
	 * Get the index of this state.
	 * @return	The index of this state.
	 */
	virtual unsigned int get_index() const;

	/**
	 * Overload the equals operator to set this state equal to the state provided.
	 * @param	other			The state to copy.
	 * @throw	StateException	The other state is not a IndexedState.
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

	/**
	 * Get the number of states which have been created. This is the value of the indexer.
	 * @return	The number of states which have been created.
	 */
	static unsigned int get_num_states();

	/**
	 * Reset the internal static variable which assigns indexes (called the indexer).
	 */
	static void reset_indexer();

protected:
	/**
	 * All indexed states have an integer for their index. This is also used as their hash.
	 */
	unsigned int index;

	/**
	 * A static variable which assigns indexes to the states. This defaults to zero.
	 */
	static unsigned int indexer;

};


#endif // INDEXED_STATE_H
