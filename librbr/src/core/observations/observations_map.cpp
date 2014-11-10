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


#include "../../../include/core/observations/observations_map.h"
#include "../../../include/core/observations/observation_exception.h"

#include <algorithm>

ObservationsMap::ObservationsMap()
{ }

ObservationsMap::ObservationsMap(const std::vector<Observation *> &observations)
{
	set(observations);
}

ObservationsMap::~ObservationsMap()
{
	reset();
}

void ObservationsMap::add(Observation *newObservation)
{
	observations[newObservation->hash_value()] = newObservation;
}

void ObservationsMap::remove(Observation *removeObservation)
{
	// Ensure that the element exists in the hash before removing it.
	std::unordered_map<unsigned int, Observation *>::const_iterator result = observations.find(removeObservation->hash_value());
	if (result == observations.end()) {
		throw ObservationException();
	}

	observations.erase(removeObservation->hash_value());
	delete removeObservation;
}

void ObservationsMap::set(const std::vector<Observation *> &newObservations)
{
	reset();
	for (Observation *observation : newObservations) {
		observations[observation->hash_value()] = observation;
	}
}

bool ObservationsMap::exists(Observation *observation)
{
	try {
		observations.at(observation->hash_value());
	} catch (const std::out_of_range &err) {
		return false;
	}
	return true;
}

Observation *ObservationsMap::get(unsigned int hash)
{
	try {
		return observations.at(hash);
	} catch (const std::out_of_range &err) {
		throw ObservationException();
	}
}

unsigned int ObservationsMap::get_num_observations() const
{
	return observations.size();
}

void ObservationsMap::reset()
{
	for (auto observation : observations) {
		delete resolve(observation);
	}
	observations.clear();
}

std::unordered_map<unsigned int, Observation *>::iterator ObservationsMap::begin()
{
	return observations.begin();
}

std::unordered_map<unsigned int, Observation *>::iterator ObservationsMap::end()
{
	return observations.end();
}

Observation *resolve(std::unordered_map<unsigned int, Observation *>::value_type &observationIterator)
{
	return observationIterator.second;
}

unsigned int hash_value(std::unordered_map<unsigned int, Observation *>::value_type &observationIterator)
{
	return observationIterator.first;
}
