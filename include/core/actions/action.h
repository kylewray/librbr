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


#ifndef ACTION_H
#define ACTION_H


#include <string>


/**
 * An abstract action object. Typically, this will just be a string; however, by abstracting
 * the actions, we are able to have actions that are classes in-and-of-themselves.
 */
class Action {
public:
	/**
	 * The default constructor of the Action object.
	 */
	Action();

	/**
	 * The constructor of the Action object which allows initial specification of the unique name.
	 * @param initialName The unique name of the state.
	 */
	Action(std::string initialName);

	/**
	 * The copy constructor of the Action object. Required by use as a map key.
	 * @param other The action to copy.
	 */
	Action(const Action &other);

	/**
	 * The default deconstructor of the Action object.
	 */
	virtual ~Action();

	/**
	 * Overload the equals operator to set this action equal to the action provided.
	 * @param other The action to copy.
	 * @return The new version of this action.
	 */
	virtual Action &operator=(const Action &other);

	/**
	 * Overload the equality comparison operator.
	 * @param other The action to compare.
	 * @return Returns @code{true} if this action is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const Action &other);

	/**
	 * Overload the less than operator for comparison.
	 * @param other The action to compare.
	 * @return Returns @code{true} if this action is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const Action &other) const;

	/**
	 * All actions must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique; however, this need not always be the case,
	 * e.g., continuous actions.
	 */
	std::string name;

};


#endif // ACTION_H
