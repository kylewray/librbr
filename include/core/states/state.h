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


#ifndef STATE_H
#define STATE_H


#include <string>


/**
 * An abstract state object. Typically, this will just be a string; however, by abstracting
 * the states, we are able to have states that are classes in-and-of-themselves.
 */
struct State {
	/**
	 * The default constructor of the State object.
	 */
	State();

	/**
	 * The constructor of the State object which allows initial specification of the unique name.
	 * @param initialName The unique name of the state.
	 */
	State(std::string initialName);

	/**
	 * The copy constructor of the State object.
	 * @param other The state to copy.
	 */
	State(const State &other);

	/**
	 * The default deconstructor of the State object.
	 */
	virtual ~State();

	/**
	 * Overload the equals operator to set this state equal to the state provided.
	 * @param other The state to copy.
	 * @return The new version of this state.
	 */
	virtual State &operator=(const State &other);

	/**
	 * Overload the equality comparison operator.
	 * @param other The state to compare.
	 * @return Returns @code{true} if this state is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const State &other);

	/**
	 * Overload the less than operator for comparison.
	 * @param other The state to compare.
	 * @return Returns @code{true} if this state is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const State &other) const;

	/**
	 * All states must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique; however, this need not always be the case,
	 * e.g., continuous states.
	 */
	std::string name;

};


#endif // STATE_H
