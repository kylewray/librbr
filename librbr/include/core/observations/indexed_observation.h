/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Hollins Wray, University of Massachusetts
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


#ifndef INDEXED_OBSERVATION_H
#define INDEXED_OBSERVATION_H


#include <string>

#include "observation.h"

/**
 * A observation object identified by a unique index which is determined using a static variable,
 * incremented each time a new IndexedObservation is created.
 */
class IndexedObservation : public Observation {
public:
	/**
	 * The default constructor of the IndexedObservation object.
	 */
	IndexedObservation();

	/**
	 * The copy constructor of the IndexedObservation object.
	 * @param	other		The observation to copy.
	 */
	IndexedObservation(const IndexedObservation &other);

	/**
	 * The default deconstructor of the IndexedObservation object.
	 */
	virtual ~IndexedObservation();

	/**
	 * Get the index of this observation.
	 * @return	The index of this observation.
	 */
	virtual unsigned int get_index() const;

	/**
	 * Overload the equals operator to set this observation equal to the observation provided.
	 * @param	other		The observation to copy.
	 * @return	The new version of this observation.
	 */
	virtual Observation &operator=(const Observation &other);

	/**
	 * Returns a string representation of this observation.
	 * @return	Returns the string representing this observation.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this observation in a collection of observations.
	 * @return	Returns the hash value of this observation.
	 */
	virtual unsigned int hash_value() const;

	/**
	 * Get the number of observations which have been created. This is the value of the indexer.
	 * @return	The number of observations which have been created.
	 */
	static unsigned int get_num_observations();

	/**
	 * Reset the internal static variable which assigns indexes (called the indexer).
	 */
	static void reset_indexer();

protected:
	/**
	 * All indexed observations have an integer for their index. This is also used as their hash.
	 */
	unsigned int index;

	/**
	 * A static variable which assigns indexes to the observations. This defaults to zero.
	 */
	static unsigned int indexer;

};


#endif // INDEXED_OBSERVATION_H
