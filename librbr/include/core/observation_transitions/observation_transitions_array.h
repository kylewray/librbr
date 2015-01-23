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


#ifndef OBSERVATION_TRANSITIONS_ARRAY_H
#define OBSERVATION_TRANSITIONS_ARRAY_H


#include "observation_transitions.h"

#include "../observations/observations.h"
#include "../observations/observations_map.h"

#include "../observations/observation.h"
#include "../observations/indexed_observation.h"

#include "../states/state.h"
#include "../states/indexed_state.h"

#include "../actions/action.h"
#include "../actions/indexed_action.h"

/**
 * A class for finite observation transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite observation transitions: a table lookup mapping observation-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite observation transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based ObservationTransitionsArray class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal observationTrantions vector variable here.
 *
 * This class requires that states and actions be of types IndexedState and IndexedAction, respectively.
 */
class ObservationTransitionsArray : virtual public ObservationTransitions {
public:
	/**
	 * The default constructor for the ObservationTransitionsArray class. This requires the
	 * number of states, actions, and observations to be specified.
	 * @param	numStates		The number of states.
	 * @param	numActions		The number of actions.
	 * @param	numObservations	The number of observations.
	 */
	ObservationTransitionsArray(unsigned int numStates, unsigned int numActions, unsigned int numObservations);

	/**
	 * The default deconstructor for the ObservationTransitionsArray class.
	 */
	virtual ~ObservationTransitionsArray();

	/**
	 * Set a observation transition from a particular observation-action-state triple to a probability.
	 * @param	previousAction					The action taken at the previous state which resulted in the current state.
	 * @param	state							The current state.
	 * @param	observation						The next observation to which we assign a probability.
	 * @param	probability						The probability of the observation given we took the action and landed
	 * 											in the state given.
	 * @throw	ObservationTransitionException	Either the state, action, or observation was invalid.
	 */
	virtual void set(Action *previousAction, State *state, Observation *observation, double probability);

	/**
	 * The probability of a transition following the observation-action-state triple provided.
	 * @param	previousAction					The action taken at the previous state which resulted in the current state.
	 * @param	state							The current state.
	 * @param	observation						The next observation to which we assign a probability.
	 * @throw	ObservationTransitionException	Either the state, action, or observation was invalid.
	 * @return	The probability of the observation given we took the action and landed in the state given.
	 */
	virtual double get(Action *previousAction, State *state, Observation *observation);

	/**
	 * Add an available observation.
	 * @param	previousAction					The previous action taken at the previous state.
	 * @param	state							The resultant state.
	 * @param	availableObservation			The available observation.
	 * @throw	ObservationTransitionException	The action-state pair could not be found.
	 */
	void add_available(Action *previousAction, State *state, Observation *availableObservation);

	/**
	 * Return a list of the observations available given a previous state and the action taken there.
	 * @param	Z					A set of observations.
	 * @param	previousAction		The action taken at the previous state which resulted in the current state.
	 * @param	state				The current state.
	 * @return	The list of available observations.
	 */
	virtual const std::vector<Observation *> &available(Observations *Z, Action *previousAction, State *state);

	/**
	 * Set the entire 3-dimensional array with the one provided. This only performs a copy.
	 * @param	O	A pointer to the new 3-d array of raw observations transitions data. This must be
	 * 				an array of size m x n x z.
	 */
	virtual void set_observation_transitions(const float *O);

	/**
	 * Get the memory location of the 3-dimensional array.
	 * @return	A pointer to the raw observation transitions data.
	 */
	virtual float *get_observation_transitions();

	/**
	 * Get the number of states used for the observation transitions array.
	 * @return	The number of states.
	 */
	virtual unsigned int get_num_states() const;

	/**
	 * Get the number of actions used for the observation transitions array.
	 * @return	The number of actions.
	 */
	virtual unsigned int get_num_actions() const;

	/**
	 * Get the number of observations used for the observation transitions array.
	 * @return	The number of observations.
	 */
	virtual unsigned int get_num_observations() const;

	/**
	 * Reset the observation transitions by assigning all probabilities to zero. This does not free memory.
	 */
	virtual void reset();

private:
	/**
	 * Compute the available observations for the action and state pair provided, then store the result
	 * in availableObservations.
	 * @param	Z							The finite set of observations.
	 * @param	a							The current action.
	 * @param	sp							The next state.
	 */
	virtual void compute_available(ObservationsMap *Z, IndexedAction *a, IndexedState *sp);

	/**
	 * The 3-dimensional array of all action-state-observation transitions. Internally,
	 * these are floats to improve speed.
	 */
	float *observationTransitions;

	/**
	 * The number of states in the state transitions second dimension.
	 */
	unsigned int states;

	/**
	 * The number of actions in the state transitions first dimension.
	 */
	unsigned int actions;

	/**
	 * The number of observations in the state transitions third dimension.
	 */
	unsigned int observations;

	/**
	 * A mapping from action-state pairs to a vector of available observations.
	 */
	std::vector<Observation *> *availableObservations;

};


#endif // OBSERVATION_TRANSITIONS_ARRAY_H
