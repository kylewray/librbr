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


#include "../../../include/core/observations/named_observation.h"

/**
 * The default constructor of the NamedObservation object.
 */
NamedObservation::NamedObservation()
{ }

/**
 * The constructor of the NamedObservation object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
NamedObservation::NamedObservation(std::string initialName)
{
	name = initialName;
}

/**
 * The copy constructor of the NamedObservation object.
 * @param other The observation to copy.
 */
NamedObservation::NamedObservation(const NamedObservation &other)
{
	*this = other;
}

/**
 * The default deconstructor of the NamedObservation object.
 */
NamedObservation::~NamedObservation()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void NamedObservation::set_name(std::string newName)
{
	name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string NamedObservation::get_name() const
{
	return name;
}

/**
 * Overload the equals operator to set this observation equal to the observation provided.
 * @param other The observation to copy.
 * @return The new version of this observation.
 */
Observation &NamedObservation::operator=(const Observation &other)
{
    const NamedObservation *o = static_cast<const NamedObservation*>(&other);
	name = o->get_name();
	return *this;
}

/**
 * Returns a string representation of this observation.
 * @return Returns the string representing this observation.
 */
std::string NamedObservation::to_string() const
{
	return name;
}

/**
 * Returns a hash value used to quickly identify this observation in a collection of observations.
 * @returns Returns the hash value of this observation.
 */
unsigned int NamedObservation::hash_value() const
{
	unsigned int hash = 7;
	for (char c : name) {
		hash = 31 * hash + (int)c;
	}
	return hash;
}
