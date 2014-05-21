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


#include "../../../include/core/actions/named_action.h"

/**
 * The default constructor of the NamedAction object.
 */
NamedAction::NamedAction()
{ }

/**
 * The constructor of the NamedAction object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
NamedAction::NamedAction(std::string initialName)
{
	name = initialName;
}

/**
 * The copy constructor of the NamedAction object. Required by use as a map key.
 * @param other The action to copy.
 */
NamedAction::NamedAction(const NamedAction &other)
{
	*this = other;
}

/**
 * The default deconstructor of the NamedAction object.
 */
NamedAction::~NamedAction()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void NamedAction::set_name(std::string newName)
{
	name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string NamedAction::get_name() const
{
	return name;
}

/**
 * Overload the equals operator to set this action equal to the action provided.
 * @param other The action to copy.
 * @return The new version of this action.
 */
Action &NamedAction::operator=(const Action &other)
{
    const NamedAction *a = static_cast<const NamedAction*>(&other);
	name = a->get_name();
	return *this;
}

/**
 * Returns a string representation of this action.
 * @return Returns the string representing this action.
 */
std::string NamedAction::to_string() const
{
	return name;
}

/**
 * Returns a hash value used to quickly identify this action in a collection of actions.
 * @returns Returns the hash value of this action.
 */
unsigned int NamedAction::hash_value() const
{
	unsigned int hash = 7;
	for (char c : name) {
		hash = 31 * hash + (int)c;
	}
	return hash;
}
