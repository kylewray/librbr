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


#include "../../../include/core/observations/finite_observations.h"


/**
 * The default constructor for the FiniteObservations class.
 */
FiniteObservations::FiniteObservations()
{ }

/**
 * The default deconstructor for the FiniteObservations class.
 */
FiniteObservations::~FiniteObservations()
{
	observations.clear();
}

/**
 * Add a observation to the set of available observations.
 * @param newObservation The new observation to include in the set of available states.
 */
void FiniteObservations::add(Observation newObservation)
{
	observations.push_back(newObservation);
}

/**
 * Remove a observation to the set of available observations.
 * @param removeObservation The state to remove from the set of available observations.
 */
void FiniteObservations::remove(Observation removeObservation)
{
	observations.erase(std::remove(observations.begin(), observations.end(), removeObservation), observations.end());
}

/**
 * Set the internal observations list given another list, performing a deep copy.
 * @param newObservations The vector of new observations to use.
 */
void FiniteObservations::set(std::vector<Observation> newObservations)
{
	observations = newObservations;
}

/**
 * Return a list of all the available observations.
 * @return Return a list of available observations.
 */
std::vector<Observation> FiniteObservations::all() const
{
	return observations;
}

/**
 * Return a list of the observations available given a previous state and the action taken there.
 * @param state 	The previous state.
 * @param action	The action taken at the previous state.
 * @return Return a list of available observations.
 *
 */
std::vector<Observation> FiniteObservations::available(State state, Action action) const
{
	return observations;
}
