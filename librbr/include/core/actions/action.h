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


#ifndef ACTION_H
#define ACTION_H


#include <string>

/**
 * An abstract action object.
 */
class Action {
public:
	/**
	 * The default constructor of the Action object.
	 */
	Action();

	/**
	 * The copy constructor of the Action object. Required by use as a map key.
	 * @param	other	The action to copy.
	 */
	Action(const Action &other);

	/**
	 * The default deconstructor of the Action object.
	 */
	virtual ~Action();

	/**
	 * Overload the equals operator to set this action equal to the action provided.
	 * @param	other	The action to copy.
	 * @return	The new version of this action.
	 */
	virtual Action &operator=(const Action &other);

	/**
	 * Overload the equality comparison operator.
	 * @param	other	The action to compare.
	 * @return	Returns @code{true} if this action is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const Action &other) const;

	/**
	 * Overload the less than operator for comparison.
	 * @param	other	The action to compare.
	 * @return	Returns @code{true} if this action is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const Action &other) const;

	/**
	 * Returns a string representation of this action.
	 * @return	Returns the string representing this action.
	 */
	virtual std::string to_string() const = 0;

	/**
	 * Returns a hash value used to quickly identify this action in a collection of actions.
	 * @return	Returns the hash value of this action.
	 */
	virtual unsigned int hash_value() const = 0;

};


#endif // ACTION_H
