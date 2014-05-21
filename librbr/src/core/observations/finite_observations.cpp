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


#include "../../../include/core/observations/finite_observations.h"
#include "../../../include/core/observations/observation_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteObservations class.
 */
FiniteObservations::FiniteObservations()
{ }

/**
 * The constructor for the FiniteObservations class which allows the specification of an initial set of observations.
 * @param observations The initial vector of observations.
 */
FiniteObservations::FiniteObservations(const std::vector<const Observation *> &observations)
{
	set(observations);
}

/**
 * The default deconstructor for the FiniteObservations class.
 */
FiniteObservations::~FiniteObservations()
{
	reset();
}

/**
 * Add a observation to the set of available observations.
 * @param newObservation The new observation to include in the set of available states.
 */
void FiniteObservations::add(const Observation *newObservation)
{
	observations.push_back(newObservation);
}

/**
 * Remove a observation to the set of available observations. This frees the memory.
 * @param removeObservation 	The state to remove from the set of available observations.
 * @throws ObservationException	The observation was not found in the observations list.
 */
void FiniteObservations::remove(const Observation *removeObservation)
{
	if (std::find(observations.begin(), observations.end(), removeObservation) == observations.end()) {
		throw ObservationException();
	}

	observations.erase(std::remove(observations.begin(), observations.end(), removeObservation), observations.end());
	delete removeObservation;
}

/**
 * Set the internal observations list given another list, performing a deep copy. This resets
 * the current list of observations and frees the memory.
 * @param newObservations The vector of new observations to use.
 */
void FiniteObservations::set(const std::vector<const Observation *> &newObservations)
{
	reset();
	observations = newObservations;
}

/**
 * Get the observation at the corresponding index. An observation's index is defined by the order
 * in which they are added and removed.
 * @param observationIndex The index of the observation.
 * @return The observation at the corresponding index.
 * @throws ObservationException The index was invalid.
 */
const Observation *FiniteObservations::get(int observationIndex) const
{
	if (observationIndex < 0 || observationIndex >= observations.size()) {
		throw ObservationException();
	}

	return observations[observationIndex];
}

/**
 * Return the number of observations.
 * @return The number of observations.
 */
int FiniteObservations::get_num_observations() const
{
	return observations.size();
}

/**
 * Reset the observations, clearing the internal list and freeing the memory.
 */
void FiniteObservations::reset()
{
	for (const Observation *observation : observations) {
		delete observation;
	}
	observations.clear();
}

/**
 * To facilitate easy iteration, return a constant beginning of the observations vector.
 * @return The iterator which points to a constant beginning of the observations vector.
 */
std::vector<const Observation *>::const_iterator FiniteObservations::begin() const
{
	return observations.begin();
}

/**
 * To facilitate easy iteration, return a constant end of the observations vector.
 * @return The iterator which points to a constant end of the observations vector.
 */
std::vector<const Observation *>::const_iterator FiniteObservations::end() const
{
	return observations.end();
}
