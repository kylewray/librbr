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


#ifndef JOINT_OBSERVATION_H
#define JOINT_OBSERVATION_H


#include <string>
#include <vector>

#include "observation.h"

/**
 * An abstract observation object. Typically, this will just be a string; however, by abstracting
 * the observations, we are able to have observations that are classes in-and-of-themselves.
 */
class JointObservation : public Observation {
public:
	/**
	 * The constructor of the JointObservation object which allows initial specification of the
	 * number of joint states.
	 * @param numJointStates The number of joint states in the tuple.
	 */
	JointObservation(int numJointObservations);

	/**
	 * The constructor of the JointObservation object which allows initial specification of the
	 * actual joint observation tuple (vector).
	 * @param jointObservation The list of states which define this joint observation.
	 */
	JointObservation(const std::vector<const Observation *> &jointObservation);

	/**
	 * The copy constructor of the JointObservation object.
	 * @param other The observation to copy.
	 */
	JointObservation(const JointObservation &other);

	/**
	 * The default deconstructor of the JointObservation object.
	 */
	virtual ~JointObservation();

	/**
	 * Set the joint observation given a list of observations.
	 * @param jointObservation The list of observations which define this joint observation.
	 */
	virtual void set(const std::vector<const Observation *> &jointObservation);

	/**
	 * Get the joint observation.
	 * @return The list of observations.
	 */
	virtual const std::vector<const Observation *> &get() const;

	/**
	 * Get a particular observation at a index.
	 * @param index The index to retrieve the observation.
	 * @return The observation at the index provided.
	 * @throws ObservationException The index is not valid.
	 */
	virtual const Observation *get(int index) const;

	/**
	 * Get the number of observations within the joint observation.
	 * @return The number of observations within the joint observation.
	 */
	virtual int get_num_observations() const;

	/**
	 * Overload the equals operator to set this joint observation equal to the observation provided.
	 * @param other The joint observation to copy.
	 * @return The new version of this observation.
	 */
	virtual Observation &operator=(const Observation &other);

	/**
	 * Returns a string representation of this action.
	 * @return Returns the string representing this action.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this action in a collection of actions.
	 * @returns Returns the hash value of this action.
	 */
	virtual int hash_value() const;

protected:
	/**
	 * The joint observation is defined as a tuple of observations. To remain general, this is a vector, but
	 * should remain a fixed size. This class will manage the memory of these observation objects.
	 */
	std::vector<const Observation *> observations;

};


#endif // JOINT_OBSERVATION_H
