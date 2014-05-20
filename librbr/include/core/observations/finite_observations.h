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


#ifndef FINITE_OBSERVATIONS_H
#define FINITE_OBSERVATIONS_H


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
class FiniteObservations : public Observations {
public:
	/**
	 * The default constructor for the FiniteObservations class.
	 */
	FiniteObservations();

	/**
	 * The constructor for the FiniteObservations class which allows the specification of an initial set of observations.
	 * @param observations The initial vector of observations.
	 */
	FiniteObservations(const std::vector<const Observation *> &observations);

	/**
	 * The default deconstructor for the FiniteObservations class.
	 */
	virtual ~FiniteObservations();

	/**
	 * Add a observation to the set of available observations.
	 * @param newObservation The new observation to include in the set of available states.
	 */
	void add(const Observation *newObservation);

	/**
	 * Remove a observation to the set of available observations. This frees the memory.
	 * @param removeObservation 	The state to remove from the set of available observations.
	 * @throws ObservationException	The observation was not found in the observations list.
	 */
	void remove(const Observation *removeObservation);

	/**
	 * Set the internal observations list given another list, performing a deep copy. This resets
	 * the current list of observations and frees the memory.
	 * @param newObservations The vector of new observations to use.
	 */
	void set(const std::vector<const Observation *> &newObservations);

	/**
	 * Get the observation at the corresponding index. An observation's index is defined by the order
	 * in which they are added and removed.
	 * @param observationIndex The index of the observation.
	 * @return The observation at the corresponding index.
	 * @throws ObservationException The index was invalid.
	 */
	const Observation *get(int observationIndex) const;

	/**
	 * Return a list of the observations available given a previous state and the action taken there.
	 * @param state 	The previous state.
	 * @param action	The action taken at the previous state.
	 * @return Return a list of available observations.
	 *
	 */
	virtual const std::vector<const Observation *> &available(const State *state, const Action *action) const;

	/**
	 * Return the number of observations.
	 * @return The number of observations.
	 */
	virtual int get_num_observations() const;

	/**
	 * Reset the observations, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

	/**
	 * To facilitate easy iteration, return a constant beginning of the observations vector.
	 * @return The iterator which points to a constant beginning of the observations vector.
	 */
	std::vector<const Observation *>::const_iterator begin() const;

	/**
	 * To facilitate easy iteration, return a constant end of the observations vector.
	 * @return The iterator which points to a constant end of the observations vector.
	 */
	std::vector<const Observation *>::const_iterator end() const;

protected:
	/**
	 * The list of all available observations.
	 */
	std::vector<const Observation *> observations;

};


#endif // FINITE_OBSERVATIONS_H
