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


#ifndef NAMED_ACTION_H
#define NAMED_ACTION_H


#include <string>

#include "action.h"

/**
 * An action object identified by a unique name.
 */
class NamedAction : public Action {
public:
	/**
	 * The default constructor of the NamedAction object.
	 */
	NamedAction();

	/**
	 * The constructor of the NamedAction object which allows initial specification of the unique name.
	 * @param initialName The unique name of the state.
	 */
	NamedAction(std::string initialName);

	/**
	 * The copy constructor of the NamedAction object. Required by use as a map key.
	 * @param other The action to copy.
	 */
	NamedAction(const NamedAction &other);

	/**
	 * The default deconstructor of the NamedAction object.
	 */
	virtual ~NamedAction();

	/**
	 * Set the name.
	 * @param newName The new name.
	 */
	virtual void set_name(std::string newName);

	/**
	 * Get the name.
	 * @return The current name.
	 */
	virtual std::string get_name() const;

	/**
	 * Overload the equals operator to set this action equal to the action provided.
	 * @param other The action to copy.
	 * @return The new version of this action.
	 */
	virtual Action &operator=(const Action &other);

	/**
	 * Returns a string representation of this action.
	 * @return Returns the string representing this action.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this action in a collection of actions.
	 * @returns Returns the hash value of this action.
	 */
	virtual unsigned int hash_value() const;

protected:
	/**
	 * All actions must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique; however, this need not always be the case,
	 * e.g., continuous actions.
	 */
	std::string name;

};


#endif // NAMED_ACTION_H
