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


#ifndef FINITE_OBSERVATION_TRANSITIONS_H
#define FINITE_OBSERVATION_TRANSITIONS_H


#include <map>

#include "observation_transitions.h"

#include "../observations/observation.h"
#include "../actions/action.h"
#include "../states/state.h"


/**
 * A class for finite observation transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite observation transitions: a table lookup mapping observation-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite observation transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteObservationTransitions class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal observationTrantions vector variable here.
 */
class FiniteObservationTransitions : public ObservationTransitions {
public:
	/**
	 * The default constructor for the FiniteObservationTransitions class.
	 */
	FiniteObservationTransitions();

	/**
	 * The default deconstructor for the FiniteObservationTransitions class.
	 */
	virtual ~FiniteObservationTransitions();

	/**
	 * Set a observation transition from a particular observation-action-state triple to a probability.
	 * @param observation	The next observation to which we assign a probability.
	 * @param action		The action taken at the current state.
	 * @param state			The current state.
	 * @param probability	The probability of the observation given we were in the state and took the action.
	 */
	void set(Observation *observation, Action *action, State *nextState, double probability);

	/**
	 * The probability of a transition following the observation-action-state triple provided.
	 * @param observation	The next observation to which we assign a probability.
	 * @param action		The action taken at the current state.
	 * @param state			The current state.
	 * @return The probability of the observation given we were in the state and took the action.
	 */
	virtual double get(Observation *observation, Action *action, State *state) const;

	/**
	 * Reset the observation transitions, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The list of all state-action-state transitions.
	 */
	std::map<Observation *, std::map<Action *, std::map<State *, double> > > observationTransitions;

};


#endif // FINITE_OBSERVATION_TRANSITIONS_H
