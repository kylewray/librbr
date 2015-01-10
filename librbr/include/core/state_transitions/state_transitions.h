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


#ifndef STATE_TRANSITIONS_H
#define STATE_TRANSITIONS_H


#include <vector>

#include "../states/state.h"
#include "../states/states.h"

#include "../actions/action.h"

/**
 * An abstract class which defines how to interact with a state transitions object.
 * This general structure should work for any implementation of state transitions,
 * such as a full table lookup, a partial table lookup with 'wildcard' capability,
 * a function, etc. This should be able to map any state-action-state triple to a
 * real value, or a distribution over possible next states for continuous states
 * and/or actions.
 *
 * This high level class allows for a single definition of each MDP-like object, instead
 * of one for every kind of combination of actions, states, etc.
 */
class StateTransitions {
public:
	/**
	 * The deconstructor for the StateTransitions class, which ensures that children
	 * classes properly deconstruct themselves.
	 */
	virtual ~StateTransitions();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the probability.
	 * @param	probability					The probability of going from the state, taking the action, then
	 * 										moving to the nextState.
	 * @throw	StateTransitionException	An error occurred. Please see child class definitions for specifics.
	 */
	virtual void set(State *state, Action *action, State *nextState, double probability) = 0;

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the probability.
	 * @throw	StateTransitionException	An error occurred. Please see child class definitions for specifics.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get(State *state, Action *action, State *nextState) = 0;

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * Note: This list may change (continual planning) if nested inside loops incorrectly. To be safe,
	 * just call it once unless you know what you are doing.
	 * @param	S							The set of states.
	 * @param	state						The previous state.
	 * @param	action						The action taken at the previous state.
	 * @throw	StateTransitionException	An error occurred. Please see child class definitions for specifics.
	 * @return	successors					A reference to the list of successor states.
	 */
	virtual const std::vector<State *> &successors(States *S, State *state, Action *action) = 0;

};


#endif // STATE_TRANSITIONS_H
