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


#include "../../../include/core/observations/finite_joint_observations.h"
#include "../../../include/core/observations/joint_observation.h"
#include "../../../include/core/observations/observation_exception.h"

#include <algorithm>

/**
 * The default constructor for the FiniteJointObservations class which requires that you
 * specify the number of factors.
 * @param numFactors The number of observation factors.
 */
FiniteJointObservations::FiniteJointObservations(int numFactors)
{
	factoredObservations.resize(numFactors);
}

/**
 * The default deconstructor for the FiniteJointObservations class.
 */
FiniteJointObservations::~FiniteJointObservations()
{
	reset();
}

/**
 * Add an observation to the set of available observations in a factor. This does *not* update the
 * observations list; please call update() once all factors have been set.
 * @param factorIndex 			The index of the factor to add the observation to.
 * @param newObservation 		The new observation to include in the set of available observations.
 * @throws ObservationException	The index was invalid.
 */
void FiniteJointObservations::add(int factorIndex, const Observation *newObservation)
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size()) {
		throw ObservationException();
	}

	factoredObservations[factorIndex].push_back(newObservation);
}

/**
 * Remove an observation to the set of available observations in a factor. This frees the memory. This
 * does *not* update the observations list; please call update() once all factors have been set.
 * @param factorIndex 			The index of the factor to add the observations to.
 * @param removeObservation 	The observation to remove from the set of available observations.
 * @throws ObservationException	The index was invalid, or the observation was not found in the observations list.
 */
void FiniteJointObservations::remove(int factorIndex, const Observation *removeObservation)
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size()) {
		throw ObservationException();
	} else if (std::find(factoredObservations[factorIndex].begin(), factoredObservations[factorIndex].end(),
			removeObservation) == factoredObservations[factorIndex].end()) {
		throw ObservationException();
	}

	factoredObservations[factorIndex].erase(
			std::remove(factoredObservations[factorIndex].begin(),
					factoredObservations[factorIndex].end(), removeObservation),
			factoredObservations[factorIndex].end());
	delete removeObservation;
}

/**
 * Set the internal observations list for a factor given another list, performing a deep copy. This resets
 * the current list of observations and frees the memory. This does *not* update the observations list; please
 * call update() once all factors have been set.
 * @param factorIndex 			The index of the factor to add the observation to.
 * @param newObservations 		The vector of new observations to use.
 * @throws ObservationException	The index was invalid, or newObservations was empty.
 */
void FiniteJointObservations::set(int factorIndex, const std::vector<const Observation *> &newObservations)
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size() || newObservations.size() == 0) {
		throw ObservationException();
	}

	// Delete the current factor's observations list.
	for (const Observation *observation : factoredObservations[factorIndex]) {
		delete observation;
	}
	factoredObservations[factorIndex].clear();

	factoredObservations[factorIndex] = newObservations;
}

/**
 * Get the observation at the corresponding index, given the particular factor. The factor index
 * is defined by the agent, and an observation's index is defined by the order in which they are
 * added and removed.
 * @param factorIndex THe index of the factor.
 * @param observationIndex The index of the observation.
 * @return The observation at the corresponding index.
 * @throws ObservationException The index was invalid.
 */
const Observation *FiniteJointObservations::get(int factorIndex, int observationIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size() ||
			observationIndex < 0 || observationIndex >= factoredObservations[factorIndex].size()) {
		throw ObservationException();
	}

	return factoredObservations[factorIndex][observationIndex];
}

/**
 * Update the internal observations list which holds all permutations of joint observations in an efficient structure.
 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
 * @throws ObservationException If a state factor has not been defined.
 */
void FiniteJointObservations::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<const Observation *> &factor : factoredObservations) {
		if (factor.size() == 0) {
			throw ObservationException();
		}
	}

	observations.clear();

	std::vector<const Observation *> create;
	update_step(create, 0);
}

/**
 * Get the number of factored observations.
 */
int FiniteJointObservations::get_num_factors()
{
	return factoredObservations.size();
}

/**
 * Reset the joint observations, clearing the internal list and freeing the memory.
 */
void FiniteJointObservations::reset()
{
	for (std::vector<const Observation *> &factor : factoredObservations) {
		for (const Observation *observation : factor) {
			delete observation;
		}
		factor.clear();
	}

	observations.clear();
}


/**
 * A helper function for updating the internal "observations" variable as part of the update() function.
 * @param currentJointObservation	The current (incomplete) joint observation as a vector of observations.
 * @param currentFactorIndex		The current factor index.
 */
void FiniteJointObservations::update_step(std::vector<const Observation *> currentJointObservation, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of joint observations.
	for (const Observation *observation : factoredObservations[currentFactorIndex]) {
		// Begin by pushing a current factor's observation on the vector (tuple).
		currentJointObservation.push_back(observation);

		// If this is the final index, then create a joint observation object and append it to the list of observations.
		// Otherwise, recurse to the next index, using the new currentJointObservation object.
		if (currentFactorIndex == factoredObservations.size() - 1) {
			observations.push_back(new JointObservation(currentJointObservation));
		} else {
			update_step(currentJointObservation, currentFactorIndex + 1);
		}

		// Pop off the observation that was just appended, to prepare for the next loop.
		currentJointObservation.pop_back();
	}
}
