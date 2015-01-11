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


#ifndef INITIAL_STATE_H
#define INITIAL_STATE_H


#include <map>

#include "states/state.h"
#include "states/belief_state.h"

/**
 * A class which defines the initial state of an MDP-like object. In fully observable models,
 * the initial state is simply a state. In partially observable models, the initial state
 * consists of a true initial state and an initial belief over states.
 */
class Initial {
public:
	/**
	 * The constructor for the Initial class.
	 */
	Initial();

	/**
	 * The constructor for the Initial class, specifying the true initial state.
	 * @param	state		The true initial state.
	 */
	Initial(State *state);

	/**
	 * The constructor for the Initial class, specifying the belief over the states.
	 * @param	belief		The belief over the states.
	 */
	Initial(BeliefState &belief);

	/**
	 * The deconstructor for the Initial class.
	 */
	virtual ~Initial();

	/**
	 * Set the true initial state.
	 * @param	state		The true initial state.
	 */
	void set_initial_state(State *state);

	/**
	 * Set the initial belief over states.
	 * @param	belief		The belief over the states.
	 */
	void set_initial_belief(BeliefState &belief);

	/**
	 * Set the probability of a particular state, assuming the belief has been created.
	 * @param	state 			The state to which the probability will be assigned.
	 * @param	probability		The probability of that the initial state is the state given.
	 */
	void set_initial_belief(State *state, double probability);

	/**
	 * Get the true initial state.
	 * @return	The true initial state.
	 */
	State *get_initial_state();

	/**
	 * Get the probability of a particular state.
	 * @param	state		The state to which the probability will be assigned.
	 * @return	The probability of that the initial state is the state given.
	 */
	BeliefState &get_initial_belief();

	/**
	 * Reset the initial state and the initial belief.
	 */
	void reset();

private:
	/**
	 * The true initial state.
	 */
	State *initialState;

	/**
	 * The belief over the states.
	 */
	BeliefState initialBelief;

};


#endif // INITIAL_STATE_H
