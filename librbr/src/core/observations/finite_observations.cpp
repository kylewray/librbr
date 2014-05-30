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


#include "../../../include/core/observations/finite_observations.h"
#include "../../../include/core/observations/observation_exception.h"

#include <algorithm>

FiniteObservations::FiniteObservations()
{ }

FiniteObservations::FiniteObservations(const std::vector<const Observation *> &observations)
{
	set(observations);
}

FiniteObservations::~FiniteObservations()
{
	reset();
}

void FiniteObservations::add(const Observation *newObservation)
{
	observations.push_back(newObservation);
}

void FiniteObservations::remove(const Observation *removeObservation)
{
	if (std::find(observations.begin(), observations.end(), removeObservation) == observations.end()) {
		throw ObservationException();
	}

	observations.erase(std::remove(observations.begin(), observations.end(), removeObservation), observations.end());
	delete removeObservation;
}

void FiniteObservations::set(const std::vector<const Observation *> &newObservations)
{
	reset();
	observations = newObservations;
}

const Observation *FiniteObservations::get(int observationIndex) const
{
	if (observationIndex < 0 || observationIndex >= observations.size()) {
		throw ObservationException();
	}

	return observations[observationIndex];
}

int FiniteObservations::get_num_observations() const
{
	return observations.size();
}

void FiniteObservations::reset()
{
	for (const Observation *observation : observations) {
		delete observation;
	}
	observations.clear();
}

std::vector<const Observation *>::const_iterator FiniteObservations::begin() const
{
	return observations.begin();
}

std::vector<const Observation *>::const_iterator FiniteObservations::end() const
{
	return observations.end();
}
