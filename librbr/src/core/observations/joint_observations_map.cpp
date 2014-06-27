/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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


#include "../../../include/core/observations/joint_observations_map.h"
#include "../../../include/core/observations/joint_observation.h"
#include "../../../include/core/observations/observation_exception.h"

#include <algorithm>

JointObservationsMap::JointObservationsMap(int numFactors)
{
	if (numFactors < 1) {
		numFactors = 1;
	}
	factoredObservations.resize(numFactors);
}

JointObservationsMap::~JointObservationsMap()
{
	reset();
}

void JointObservationsMap::add(int factorIndex, const Observation *newObservation)
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size()) {
		throw ObservationException();
	}

	factoredObservations[factorIndex].push_back(newObservation);
}

void JointObservationsMap::remove(int factorIndex, const Observation *removeObservation)
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

void JointObservationsMap::set(int factorIndex, const std::vector<const Observation *> &newObservations)
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

const Observation *JointObservationsMap::get(int factorIndex, int observationIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredObservations.size() ||
			observationIndex < 0 || observationIndex >= factoredObservations[factorIndex].size()) {
		throw ObservationException();
	}

	return factoredObservations[factorIndex][observationIndex];
}

void JointObservationsMap::update()
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

int JointObservationsMap::get_num_factors()
{
	return factoredObservations.size();
}

void JointObservationsMap::reset()
{
	for (std::vector<const Observation *> &factor : factoredObservations) {
		for (const Observation *observation : factor) {
			delete observation;
		}
		factor.clear();
	}

	observations.clear();
}

void JointObservationsMap::update_step(std::vector<const Observation *> currentJointObservation, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of joint observations.
	for (const Observation *observation : factoredObservations[currentFactorIndex]) {
		// Begin by pushing a current factor's observation on the vector (tuple).
		currentJointObservation.push_back(observation);

		// If this is the final index, then create a joint observation object and append it to the list of observations.
		// Otherwise, recurse to the next index, using the new currentJointObservation object.
		if (currentFactorIndex == factoredObservations.size() - 1) {
			const JointObservation *newObservation = new JointObservation(currentJointObservation);
			observations[newObservation->hash_value()] = newObservation;
		} else {
			update_step(currentJointObservation, currentFactorIndex + 1);
		}

		// Pop off the observation that was just appended, to prepare for the next loop.
		currentJointObservation.pop_back();
	}
}
