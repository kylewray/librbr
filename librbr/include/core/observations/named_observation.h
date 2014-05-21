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


#ifndef NAMED_OBSERVATION_H
#define NAMED_OBSERVATION_H


#include <string>

#include "observation.h"

/**
 * An observation object identified by a unique name.
 */
class NamedObservation : public Observation {
public:
	/**
	 * The default constructor of the NamedObservation object.
	 */
	NamedObservation();

	/**
	 * The constructor of the NamedObservation object which allows initial specification of the unique name.
	 * @param initialName The unique name of the state.
	 */
	NamedObservation(std::string initialName);

	/**
	 * The copy constructor of the NamedObservation object.
	 * @param other The observation to copy.
	 */
	NamedObservation(const NamedObservation &other);

	/**
	 * The default deconstructor of the NamedObservation object.
	 */
	virtual ~NamedObservation();

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
	 * Overload the equals operator to set this observation equal to the observation provided.
	 * @param other The observation to copy.
	 * @return The new version of this observation.
	 */
	virtual Observation &operator=(const Observation &other);

	/**
	 * Returns a string representation of this observation.
	 * @return Returns the string representing this observation.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this observation in a collection of observations.
	 * @returns Returns the hash value of this observation.
	 */
	virtual unsigned int hash_value() const;

protected:
	/**
	 * All observations must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique; however, this need not always be the case,
	 * e.g., continuous observations.
	 */
	std::string name;

};


#endif // NAMED_OBSERVATION_H
