/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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


#ifndef INDEXED_ACTION_H
#define INDEXED_ACTION_H


#include <string>

#include "action.h"

/**
 * An action object identified by a unique index which is determined using a static variable,
 * incremented each time a new IndexedAction is created.
 */
class IndexedAction : public Action {
public:
	/**
	 * The default constructor of the IndexedAction object.
	 */
	IndexedAction();

	/**
	 * The copy constructor of the IndexedAction object.
	 * @param	other		The action to copy.
	 */
	IndexedAction(const IndexedAction &other);

	/**
	 * The default deconstructor of the IndexedAction object.
	 */
	virtual ~IndexedAction();

	/**
	 * Get the index of this action.
	 * @return	The index of this action.
	 */
	virtual unsigned int get_index() const;

	/**
	 * Overload the equals operator to set this action equal to the action provided.
	 * @param	other		The action to copy.
	 * @return	The new version of this action.
	 */
	virtual Action &operator=(const Action &other);

	/**
	 * Returns a string representation of this action.
	 * @return	Returns the string representing this action.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this action in a collection of actions.
	 * @return	Returns the hash value of this action.
	 */
	virtual unsigned int hash_value() const;

	/**
	 * Get the number of actions which have been created. This is the value of the indexer.
	 * @return	The number of actions which have been created.
	 */
	static unsigned int get_num_actions();

	/**
	 * Reset the internal static variable which assigns indexes (called the indexer).
	 */
	static void reset_indexer();

protected:
	/**
	 * All indexed actions have an integer for their index. This is also used as their hash.
	 */
	unsigned int index;

	/**
	 * A static variable which assigns indexes to the actions. This defaults to zero.
	 */
	static unsigned int indexer;

};


#endif // INDEXED_ACTION_H
