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


#ifndef NAMED_STATE_H
#define NAMED_STATE_H


#include <string>

#include "state.h"

/**
 * A state object identified by a unique name.
 */
class NamedState : virtual public State {
public:
	/**
	 * The default constructor of the NamedState object.
	 */
	NamedState();

	/**
	 * The constructor of the NamedState object which allows initial specification of the unique name.
	 * @param	initialName		The unique name of the state.
	 */
	NamedState(std::string initialName);

	/**
	 * The copy constructor of the NamedState object.
	 * @param	other		The state to copy.
	 */
	NamedState(const NamedState &other);

	/**
	 * The default deconstructor of the NamedState object.
	 */
	virtual ~NamedState();

	/**
	 * Set the name.
	 * @param	newName		The new name.
	 */
	virtual void set_name(std::string newName);

	/**
	 * Get the name.
	 * @return	The current name.
	 */
	virtual std::string get_name() const;

	/**
	 * Overload the equals operator to set this state equal to the state provided.
	 * @param	other			The state to copy.
	 * @throw	StateException	The other state is not a NamedState.
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
	 * Create a hash value for use in searching based on the name specified.
	 * @param	nameToHash		The name to convert to a hash value.
	 */
	static unsigned int hash_value(std::string nameToHash);

protected:
	/**
	 * All states must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique; however, this need not always be the case,
	 * e.g., continuous states.
	 */
	std::string name;

};


#endif // NAMED_STATE_H
