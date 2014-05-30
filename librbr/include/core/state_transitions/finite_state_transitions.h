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


#ifndef FINITE_STATE_TRANSITIONS_H
#define FINITE_STATE_TRANSITIONS_H


#include <unordered_map>

#include "state_transitions.h"

#include "../states/state.h"
#include "../states/finite_states.h"

#include "../actions/action.h"

/**
 * A class for finite state transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite state transitions: a table lookup mapping state-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite state transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteStateTransitions class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal stateTrantions vector variable here.
 */
class FiniteStateTransitions : public StateTransitions {
public:
	/**
	 * The default constructor for the FiniteStateTransitions class.
	 */
	FiniteStateTransitions();

	/**
	 * The default deconstructor for the FiniteStateTransitions class.
	 */
	virtual ~FiniteStateTransitions();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken at the current state.
	 * @param	nextState		The next state with which we assign the probability.
	 * @param	probability		The probability of going from the state, taking the action, then
	 * 							moving to the nextState.
	 */
	void set(const State *state, const Action *action, const State *nextState, double probability);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param	state		The current state of the system.
	 * @param	action		The action taken at the current state.
	 * @param	nextState	The next state with which we assign the probability.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get(const State *state, const Action *action, const State *nextState) const;

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * @param	S				The finite set of states.
	 * @param	state			The previous state.
	 * @param	action			The action taken at the previous state.
	 * @param	successors		The list to overwrite and set to be the list of successor states.
	 */
	virtual void successors(const FiniteStates *S, const State *state, const Action *action, std::vector<const State *> &result) const;

	/**
	 * Reset the state transitions, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The actual get function which returns a value. This will throw an error if the value is undefined.
	 * It is used as a helper function for the public get function.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the reward.
	 * @throw	StateTransitionException	The state transition was not defined.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get_value(const State *state, const Action *action, const State *nextState) const;

	/**
	 * The list of all state-action-state transitions.
	 */
	std::unordered_map<const State *,
		std::unordered_map<const Action *,
		std::unordered_map<const State *, double> > > stateTransitions;

	/**
	 * A special state (implicitly constant) referring to a state wildcard.
	 */
	State *stateWildcard;

	/**
	 * A special action (implicitly constant) referring to an action wildcard.
	 */
	Action *actionWildcard;

};


#endif // FINITE_STATE_TRANSITIONS_H
