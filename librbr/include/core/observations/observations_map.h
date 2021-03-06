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


#ifndef OBSERVATIONS_MAP_H
#define OBSERVATIONS_MAP_H


#include <unordered_map>
#include <vector>

#include "observation.h"
#include "observations.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * A class for finite sets of observations in an MDP-like object. Informally, there are two basic ways to
 * store finite observations: a vector of observations or a generator function based on a state and action.
 * In both cases, we require that any class with finite observations provide certain get functions so that
 * any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteObservations class, please create a child class which
 * implements the function in the virtual functions described below. You will likely ignore the internal
 * observations vector variable here.
 */
class ObservationsMap : public Observations {
public:
	/**
	 * The default constructor for the FiniteObservations class.
	 */
	ObservationsMap();

	/**
	 * The constructor for the FiniteObservations class which allows the specification of an initial set of observations.
	 * @param	observations	The initial vector of observations.
	 */
	ObservationsMap(const std::vector<Observation *> &observations);

	/**
	 * The default deconstructor for the FiniteObservations class.
	 */
	virtual ~ObservationsMap();

	/**
	 * Add a observation to the set of available observations.
	 * @param	newObservation	The new observation to include in the set of available states.
	 */
	void add(Observation *newObservation);

	/**
	 * Remove a observation to the set of available observations. This frees the memory.
	 * @param	removeObservation 		The state to remove from the set of available observations.
	 * @throw	ObservationException	The observation was not found in the observations list.
	 */
	void remove(Observation *removeObservation);

	/**
	 * Set the internal observations list given another list, performing a deep copy. This resets
	 * the current list of observations and frees the memory.
	 * @param	newObservations		The vector of new observations to use.
	 */
	void set(const std::vector<Observation *> &newObservations);

	/**
	 * Check if this observation has already been created or not.
	 * @param	observation		The observation to check if it is created or not.
	 * @return	Returns @code{true} if the observation exists in the observations hash; @code{false} otherwise.
	 */
	bool exists(Observation *observation);

	/**
	 * Get an observation with a particular hash value.
	 * @param	hash					The hash of the observation.
	 * @throw	ObservationException	There are no observations with the hash value specified.
	 * @return	The observation with the particular hash value specified.
	 */
	Observation *get(unsigned int hash);

	/**
	 * Return the number of observations.
	 * @return	The number of observations.
	 */
	unsigned int get_num_observations() const;

	/**
	 * Reset the observations, clearing the internal list and freeing the memory.
	 */
	void reset();

	/**
	 * To facilitate easy iteration, return a constant beginning of the observations vector.
	 * @return	The iterator which points to a constant beginning of the observations vector.
	 */
	std::unordered_map<unsigned int, Observation *>::iterator begin();

	/**
	 * To facilitate easy iteration, return a constant end of the observations vector.
	 * @return	The iterator which points to a constant end of the observations vector.
	 */
	std::unordered_map<unsigned int, Observation *>::iterator end();

protected:
	/**
	 * The mapping of observation hash values to observations. This is the main
	 * container of observations.
	 */
	std::unordered_map<unsigned int, Observation *> observations;

};

/**
 * Get the observation pointer of a observation iterator.
 * @param	observationIterator		The observation iterator to retrieve the observation pointer from.
 */
Observation *resolve(std::unordered_map<unsigned int, Observation *>::value_type &observationIterator);

/**
 * Get the hash of a observation iterator.
 * @param	observationIterator		The observation iterator to retrieve the hash value from.
 */
unsigned int hash_value(std::unordered_map<unsigned int, Observation *>::value_type &observationIterator);


#endif // OBSERVATIONS_MAP_H
