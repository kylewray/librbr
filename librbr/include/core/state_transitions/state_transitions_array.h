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


#ifndef STATE_TRANSITIONS_ARRAY_H
#define STATE_TRANSITIONS_ARRAY_H


#include "state_transitions.h"

#include "../states/state.h"
#include "../states/states.h"

#include "../actions/action.h"

/**
 * A class for finite state transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite state transitions: a table lookup mapping state-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite state transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based StateTransitionsArray class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal stateTrantions vector variable here.
 *
 * This class requires that states and actions be of types IndexedState and IndexedAction, respectively.
 */
class StateTransitionsArray : virtual public StateTransitions {
public:
	/**
	 * The default constructor for the StateTransitionsArray class. This requires the
	 * number of states and actions to be specified.
	 * @param	numStates		The number of states.
	 * @param	numActions		The number of actions.
	 */
	StateTransitionsArray(unsigned int numStates, unsigned int numActions);

	/**
	 * The default deconstructor for the StateTransitionsArray class.
	 */
	virtual ~StateTransitionsArray();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the probability.
	 * @param	probability					The probability of going from the state, taking the action, then
	 * 										moving to the nextState.
	 * @throw	StateTransitionException	Either one of the states or the action was invalid.
	 */
	virtual void set(State *state, Action *action, State *nextState, double probability);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the probability.
	 * @throw	StateTransitionException	Either one of the states or the action was invalid.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get(State *state, Action *action, State *nextState);

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * @param	S							The set of states.
	 * @param	state						The previous state.
	 * @param	action						The action taken at the previous state.
	 * @param	successors					The list to overwrite and set to be the list of successor states.
	 * @throw	StateTransitionException	Either the states array, state, or action was invalid.
	 */
	virtual void successors(States *S, State *state, Action *action,
			std::vector<State *> &result);

	/**
	 * Set the entire 3-dimensional array with the one provided. This only performs a copy.
	 * @param	T	A pointer to the new 3-d array of raw state transitions data. This must be
	 * 				an array of size n x m x n.
	 */
	virtual void set_state_transitions(const float *T);

	/**
	 * Get the memory location of the 3-dimensional array.
	 * @return	A pointer to the raw state transitions data.
	 */
	virtual float *get_state_transitions();

	/**
	 * Get the number of states used for the state transitions array.
	 * @return	The number of states.
	 */
	virtual unsigned int get_num_states() const;

	/**
	 * Get the number of actions used for the state transitions array.
	 * @return	The number of actions.
	 */
	virtual unsigned int get_num_actions() const;

	/**
	 * Reset the state transitions by assigning all probabilities to zero. This does not free memory.
	 */
	virtual void reset();

private:
	/**
	 * The 3-dimensional array of all state-action-state transitions. Internally,
	 * these are floats to improve speed.
	 */
	float *stateTransitions;

	/**
	 * The number of states in the state transitions first and third dimensions.
	 */
	unsigned int states;

	/**
	 * The number of actions in the state transitions second dimension.
	 */
	unsigned int actions;

};


#endif // STATE_TRANSITIONS_ARRAY_H
