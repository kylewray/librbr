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


#include "../../../include/core/actions/action.h"

/**
 * The default constructor of the Action object.
 */
Action::Action()
{ }

/**
 * The constructor of the Action object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
Action::Action(std::string initialName)
{
	name = initialName;
}

/**
 * The copy constructor of the Action object. Required by use as a map key.
 * @param other The action to copy.
 */
Action::Action(const Action &other)
{
	*this = other;
}

/**
 * The default deconstructor of the Action object.
 */
Action::~Action()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void Action::set_name(std::string newName)
{
	name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string Action::get_name() const
{
	return name;
}

/**
 * Overload the equals operator to set this action equal to the action provided.
 * @param other The action to copy.
 * @return The new version of this action.
 */
Action &Action::operator=(const Action &other)
{
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The action to compare.
 * @return Returns @code{true} if this action is equal to the other; @code{false} otherwise.
 */
bool Action::operator==(const Action &other) const
{
	return name == other.get_name();
}

/**
 * Overload the less than operator for comparison.
 * @param other The action to compare.
 * @return Returns @code{true} if this action is less than the other; @code{false} otherwise.
 */
bool Action::operator<(const Action &other) const
{
	return name < other.get_name();
}
