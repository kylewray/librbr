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


#ifndef OBSERVATION_H
#define OBSERVATION_H


#include <string>

/**
 * An abstract observation object.
 */
class Observation {
public:
	/**
	 * The default constructor of the Observation object.
	 */
	Observation();

	/**
	 * The copy constructor of the Observation object.
	 * @param other The observation to copy.
	 */
	Observation(const Observation &other);

	/**
	 * The default deconstructor of the Observation object.
	 */
	virtual ~Observation();

	/**
	 * Overload the equals operator to set this observation equal to the observation provided.
	 * @param other The observation to copy.
	 * @return The new version of this observation.
	 */
	virtual Observation &operator=(const Observation &other);

	/**
	 * Overload the equality comparison operator.
	 * @param other The observation to compare.
	 * @return Returns @code{true} if this observation is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const Observation &other) const;

	/**
	 * Overload the less than operator for comparison.
	 * @param other The observation to compare.
	 * @return Returns @code{true} if this observation is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const Observation &other) const;

	/**
	 * Returns a string representation of this observation.
	 * @return Returns the string representing this observation.
	 */
	virtual std::string to_string() const = 0;

	/**
	 * Returns a hash value used to quickly identify this observation in a collection of observations.
	 * @returns Returns the hash value of this observation.
	 */
	virtual int hash_value() const = 0;

};


#endif // OBSERVATION_H
