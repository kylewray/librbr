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


#ifndef OBSERVATION_TRANSITIONS_H
#define OBSERVATION_TRANSITIONS_H


#include <vector>

#include "../observations/observation.h"
#include "../observations/observations.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * An abstract class which defines how to interact with a observation transitions object.
 * This general structure should work for any implementation of observation transitions,
 * such as a full table lookup, a partial table lookup with 'wildcard' capability,
 * a function, etc. This should be able to map any observation-action-state triple to a
 * real value, or a distribution over possible next observations for continuous states
 * and/or actions.
 *
 * This high level class allows for a single definition of each MDP-like object, instead
 * of one for every kind of combination of actions, states, etc.
 */
class ObservationTransitions {
public:
	/**
	 * The deconstructor for the ObservationTransitions class, which ensures that children
	 * classes properly deconstruct themselves.
	 */
	virtual ~ObservationTransitions();

	/**
	 * Set a observation transition from a particular observation-action-state triple to a probability.
	 * @param	previousAction	The action taken at the previous state which resulted in the current state.
	 * @param	state			The current state.
	 * @param	observation		The next observation to which we assign a probability.
	 * @param	probability		The probability of the observation given we took the action and landed in the state given.
	 */
	virtual void set(Action *previousAction, State *state, Observation *observation, double probability) = 0;

	/**
	 * The probability of a transition following the observation-action-state triple provided.
	 * @param	previousAction		The action taken at the previous state which resulted in the current state.
	 * @param	state				The current state.
	 * @param	observation			The next observation to which we assign a probability.
	 * @return	The probability of the observation given we took the action and landed in the state given.
	 */
	virtual double get(Action *previousAction, State *state, Observation *observation) = 0;

	/**
	 * Return a list of the observations available given a previous state and the action taken there.
	 * @param	Z					A set of observations.
	 * @param	previousAction		The action taken at the previous state which resulted in the current state.
	 * @param	state				The current state.
	 * @param	result				The list to overwrite and set to be the list of successor states.
	 */
	virtual void available(Observations *Z, Action *previousAction, State *state,
			std::vector<Observation *> &result) = 0;

};


#endif // OBSERVATION_TRANSITIONS_H
