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


#include "../../../include/core/observations/joint_observation.h"
#include "../../../include/core/observations/observation_exception.h"

JointObservation::JointObservation(int numJointObservations)
{
	observations.reserve(numJointObservations);
}

JointObservation::JointObservation(const std::vector<const Observation *> &jointObservation)
{
	observations.reserve(jointObservation.size());
	observations = jointObservation;
}

JointObservation::JointObservation(const JointObservation &other)
{
	*this = other;
}

JointObservation::~JointObservation()
{ }

void JointObservation::set(const std::vector<const Observation *> &jointObservation)
{
	observations = jointObservation;
}

const std::vector<const Observation *> &JointObservation::get() const
{
	return observations;
}

const Observation *JointObservation::get(int index) const
{
	if (index < 0 || index >= observations.size()) {
		throw ObservationException();
	}
	return observations[index];
}

int JointObservation::get_num_observations() const
{
	return observations.size();
}

Observation &JointObservation::operator=(const Observation &other)
{
    const JointObservation *o = static_cast<const JointObservation*>(&other);
	observations = o->get();
	return *this;
}

std::string JointObservation::to_string() const
{
	std::string jointObservation = "";
	for (int i = 0; i < observations.size(); i++) {
		jointObservation += observations[i]->to_string();
		if (i != observations.size() - 1) {
			jointObservation += " ";
		}
	}
	return "<" + jointObservation + ">";
}

unsigned int JointObservation::hash_value() const
{
	unsigned int hash = 7;
	for (const Observation *observation : observations) {
		hash = 31 * hash + observation->hash_value();
	}
	return hash;
}
