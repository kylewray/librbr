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


#include "../../../include/core/observations/joint_observation.h"
#include "../../../include/core/observations/observation_exception.h"

/**
 * The constructor of the JointObservation object which allows initial specification of the
 * number of joint states.
 * @param numJointStates The number of joint states in the tuple.
 */
JointObservation::JointObservation(int numJointObservations)
{
	observations.reserve(numJointObservations);
}

/**
 * The constructor of the JointObservation object which allows initial specification of the
 * actual joint observation tuple (vector).
 * @param jointObservation The list of states which define this joint observation.
 */
JointObservation::JointObservation(const std::vector<const Observation *> &jointObservation)
{
	observations.reserve(jointObservation.size());
	observations = jointObservation;
	update_name();
}

/**
 * The copy constructor of the JointObservation object.
 * @param other The observation to copy.
 */
JointObservation::JointObservation(const JointObservation &other)
{
	*this = other;
}

/**
 * The default deconstructor of the JointObservation object.
 */
JointObservation::~JointObservation()
{ }

/**
 * Override set name (leave get_name() alone) to raise an error when it is called.
 * @param newName The new name.
 * @throws ObservationException This is no longer a valid function.
 */
void JointObservation::set_name(std::string newName)
{
	throw ObservationException();
}

/**
 * Set the joint observation given a list of observations.
 * @param jointObservation The list of observations which define this joint observation.
 */
void JointObservation::set(const std::vector<const Observation *> &jointObservation)
{
	observations = jointObservation;
	update_name();
}

/**
 * Get the joint observation.
 * @return The list of observations.
 */
const std::vector<const Observation *> &JointObservation::get() const
{
	return observations;
}

/**
 * Get a particular observation at a index.
 * @param index The index to retrieve the observation.
 * @return The observation at the index provided.
 * @throws ObservationException The index is not valid.
 */
const Observation *JointObservation::get(int index) const
{
	if (index < 0 || index >= observations.size()) {
		throw ObservationException();
	}
	return observations[index];
}

/**
 * Overload the equals operator to set this joint observation equal to the observation provided.
 * @param other The joint observation to copy.
 * @return The new version of this observation.
 */
JointObservation &JointObservation::operator=(const JointObservation &other)
{
	observations = other.get();
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The joint observation to compare.
 * @return Returns @code{true} if this observation is equal to the other; @code{false} otherwise.
 */
bool JointObservation::operator==(const JointObservation &other)
{
	int counter = 0;
	for (const Observation *observation : observations) {
		if (*observation == *(other.get(counter))) {
			return false;
		}
		counter++;
	}
	return true;
}

/**
 * Overload the less than operator for comparison.
 * @param other The joint observation to compare.
 * @return Returns @code{true} if this observation is less than the other; @code{false} otherwise.
 */
bool JointObservation::operator<(const JointObservation &other) const
{
	return name < other.get_name();
}

/**
 * A helper function to compute the name of the joint observation, once the observations are set.
 */
void JointObservation::update_name()
{
	name = "";

	int counter = 0;
	for (const Observation *observation : observations) {
		name += observation->get_name();

		counter++;
		if (counter < observations.size()) {
			name += " ";
		}
	}
}
