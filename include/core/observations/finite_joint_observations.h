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


#ifndef FINITE_JOINT_OBSERVATIONS_H
#define FINITE_JOINT_OBSERVATIONS_H


#include <vector>

#include "observation.h"
#include "observations.h"
#include "finite_observations.h"


/**
 * A class for finite sets of observations in an MDP-like object. Informally, there are two basic ways to
 * store finite observations: a vector of observations or a generator function based on a state and action.
 * In both cases, we require that any class with finite observations provide certain get functions so that
 * any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteJointObservations class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore the
 * internal observations vector variable here.
 *
 * Note: The protected "observations" variable implicitly must only store JointObservation objects, not
 * Observation objects.
 */

class FiniteJointObservations : public FiniteObservations {
public:
	/**
	 * The default constructor for the FiniteJointObservations class which requires that you
	 * specify the number of factors.
	 * @param numFactors The number of observation factors.
	 */
	FiniteJointObservations(int numFactors);

	/**
	 * The default deconstructor for the FiniteJointObservations class.
	 */
	virtual ~FiniteJointObservations();

	/**
	 * Add an observation to the set of available observations in a factor. This does *not* update the
	 * observations list; please call update() once all factors have been set.
	 * @param factorIndex 			The index of the factor to add the observation to.
	 * @param newObservation 		The new observation to include in the set of available observations.
	 * @throws ObservationException	The index was invalid.
	 */
	void add(int factorIndex, Observation *newObservation);

	/**
	 * Remove an observation to the set of available observations in a factor. This frees the memory. This
	 * does *not* update the observations list; please call update() once all factors have been set.
	 * @param factorIndex 			The index of the factor to add the observations to.
	 * @param removeObservation 	The observation to remove from the set of available observations.
	 * @throws ObservationException	The index was invalid.
	 */
	void remove(int factorIndex, Observation *removeObservation);

	/**
	 * Set the internal observations list for a factor given another list, performing a deep copy. This resets
	 * the current list of observations and frees the memory. This does *not* update the observations list; please
	 * call update() once all factors have been set.
	 * @param factorIndex 			The index of the factor to add the observation to.
	 * @param newObservations 		The vector of new observations to use.
	 * @throws ObservationException	The index was invalid, or newObservations was empty.
	 */
	void set(int factorIndex, std::vector<Observation *> newObservations);

	/**
	 * Update the internal observations list which holds all permutations of joint observations in an efficient structure.
	 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
	 * @throws ObservationException If a state factor has not been defined.
	 */
	void update();

	/**
	 * Reset the joint observations, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

protected:
	/**
	 * The list of all available observations for each observation factor.
	 */
	std::vector<std::vector<Observation *> > factoredObservations;

private:
	/**
	 * A helper function for updating the internal "observations" variable as part of the update() function.
	 * @param currentJointObservation	The current (incomplete) joint observation as a vector of observations.
	 * @param currentFactorIndex		The current factor index.
	 */
	void update_step(std::vector<Observation *> currentJointObservation, int currentFactorIndex);

};


#endif // FINITE_JOINT_OBSERVATIONS_H
