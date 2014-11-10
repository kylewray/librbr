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


#ifndef OBSERVATION_TRANSITIONS_MAP_H
#define OBSERVATION_TRANSITIONS_MAP_H


#include <unordered_map>

#include "observation_transitions.h"

#include "../observations/observation.h"
#include "../observations/observations.h"

#include "../states/state.h"
#include "../actions/action.h"

/**
 * A class for finite observation transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite observation transitions: a table lookup mapping observation-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite observation transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based ObservationTransitionsMap class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal observationTrantions vector variable here.
 */
class ObservationTransitionsMap : virtual public ObservationTransitions {
public:
	/**
	 * The default constructor for the ObservationTransitionsMap class.
	 */
	ObservationTransitionsMap();

	/**
	 * The default deconstructor for the ObservationTransitionsMap class.
	 */
	virtual ~ObservationTransitionsMap();

	/**
	 * Set a observation transition from a particular observation-action-state triple to a probability.
	 * @param	previousAction	The action taken at the previous state which resulted in the current state.
	 * @param	state			The current state.
	 * @param	observation		The next observation to which we assign a probability.
	 * @param	probability		The probability of the observation given we took the action and landed in the state given.
	 */
	virtual void set(Action *previousAction, State *state, Observation *observation, double probability);

	/**
	 * The probability of a transition following the observation-action-state triple provided.
	 * @param	previousAction		The action taken at the previous state which resulted in the current state.
	 * @param	state				The current state.
	 * @param	observation			The next observation to which we assign a probability.
	 * @return	The probability of the observation given we took the action and landed in the state given.
	 */
	virtual double get(Action *previousAction, State *state, Observation *observation);

	/**
	 * Return a list of the observations available given a previous state and the action taken there.
	 * @param	Z					A set of observations.
	 * @param	previousAction		The action taken at the previous state which resulted in the current state.
	 * @param	state				The current state.
	 * @param	result				The list to overwrite and set to be the list of successor states.
	 */
	virtual void available(Observations *Z, Action *previousAction, State *state,
			std::vector<Observation *> &result);

	/**
	 * Reset the observation transitions, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The actual get function which returns a value. This will throw an error if the value is undefined.
	 * It is used as a helper function for the public get function.
	 * @param	previousAction					The action taken at the previous state which resulted in the current state.
	 * @param	state							The current state.
	 * @param	observation						The next observation to which we assign a probability.
	 * @throw	ObservationTransitionException 	The observation transition was not defined.
	 * @return	The probability of the observation given we took the action and landed in the state given.
	 */
	virtual double get_value(Action *previousAction, State *state, Observation *observation);

	/**
	 * The list of all state-action-state transitions.
	 */
	std::unordered_map<Action *,
		std::unordered_map<State *,
		std::unordered_map<Observation *, double> > > observationTransitions;

	/**
	 * A special action (implicitly constant) referring to an action wildcard.
	 */
	Action *actionWildcard;

	/**
	 * A special state (implicitly constant) referring to a state wildcard.
	 */
	State *stateWildcard;

	/**
	 * A special observation (implicitly constant) referring to an observation wildcard.
	 */
	Observation *observationWildcard;

};


#endif // OBSERVATION_TRANSITIONS_MAP_H
