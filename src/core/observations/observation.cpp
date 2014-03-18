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


#include "../../../include/core/observations/observation.h"

/**
 * The default constructor of the Observation object.
 */
Observation::Observation()
{ }

/**
 * The constructor of the Observation object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
Observation::Observation(std::string initialName)
{
	name = initialName;
}

/**
 * The copy constructor of the Observation object.
 * @param other The observation to copy.
 */
Observation::Observation(const Observation &other)
{
	*this = other;
}

/**
 * The default deconstructor of the Observation object.
 */
Observation::~Observation()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void Observation::set_name(std::string newName)
{
	name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string Observation::get_name() const
{
	return name;
}

/**
 * Overload the equals operator to set this observation equal to the observation provided.
 * @param other The observation to copy.
 * @return The new version of this observation.
 */
Observation &Observation::operator=(const Observation &other)
{
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The observation to compare.
 * @return Returns @code{true} if this observation is equal to the other; @code{false} otherwise.
 */
bool Observation::operator==(const Observation &other) const
{
	return name == other.get_name();
}

/**
 * Overload the less than operator for comparison.
 * @param other The observation to compare.
 * @return Returns @code{true} if this observation is less than the other; @code{false} otherwise.
 */
bool Observation::operator<(const Observation &other) const
{
	return name < other.get_name();
}
