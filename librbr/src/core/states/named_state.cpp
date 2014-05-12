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


#include "../../../include/core/states/named_state.h"

/**
 * The default constructor of the NamedState object.
 */
NamedState::NamedState()
{ }

/**
 * The constructor of the NamedState object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
NamedState::NamedState(std::string initialName)
{
    name = initialName;
}

/**
 * The copy constructor of the State object.
 * @param other The state to copy.
 */
NamedState::NamedState(const NamedState &other)
{
    *this = other;
}

/**
 * The default deconstructor of the State object.
 */
NamedState::~NamedState()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void NamedState::set_name(std::string newName)
{
    name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string NamedState::get_name() const
{
    return name;
}

/**
 * Overload the equals operator to set this state equal to the state provided.
 * @param other The state to copy.
 * @return The new version of this state.
 */
State &NamedState::operator=(const State &s)
{
    const NamedState *other = static_cast<const NamedState*> (&s);
    name = other->name;
    return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The state to compare.
 * @return Returns @code{true} if this state is equal to the other; @code{false} otherwise.
 */
bool NamedState::operator==(const State &s) const
{
    const NamedState *other = static_cast<const NamedState*> (&s);
    return name == other->name;
}

/**
 * Overload the less than operator for comparison.
 * @param other The state to compare.
 * @return Returns @code{true} if this state is less than the other; @code{false} otherwise.
 */
bool NamedState::operator<(const State &s) const
{
    const NamedState *other = static_cast<const NamedState*> (&s);
    return name < other->name;
}

/**
  * This hash function is based on Java's String.hashCode() implementation.
  */
int NamedState::hash_value() const 
{
    int hash = 7;
    for (char c : name) {
        hash = 31*hash + (int) c;
    }
    return hash;
}

std::string NamedState::to_string() const 
{
    return name;
}
