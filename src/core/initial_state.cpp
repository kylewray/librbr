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


#include "../../include/core/initial_state.h"


/**
 * The constructor for the InitialState class.
 */
InitialState::InitialState()
{
	initialState = nullptr;
}

/**
 * The constructor for the InitialState class, specifying the true initial state.
 * @param state The true initial state.
 */
InitialState::InitialState(State *state)
{
	set_initial_state(state);
	set_initial_belief(state, 1.0);
}

/**
 * The deconstructor for the InitialState class.
 */
InitialState::~InitialState()
{
	reset();
}

/**
 * Set the true initial state.
 * @param state The true initial state.
 */
void InitialState::set_initial_state(State *state)
{
	initialState = state;
}

/**
 * Set the probability of a particular state.
 * @param state 		The state to which the probability will be assigned.
 * @param probability	The probability of that the initial state is the state given.
 */
void InitialState::set_initial_belief(State *state, double probability)
{
	initialBelief[state] = 1.0;
}

/**
 * Get the true initial state.
 * @return The true initial state.
 */
State *InitialState::get_initial_state()
{
	return initialState;
}

/**
 * Get the probability of a particular state.
 * @param state The state to which the probability will be assigned.
 * @return The probability of that the initial state is the state given.
 */
double InitialState::get_initial_belief(State *state) const
{
	std::map<State *, double>::const_iterator belief = initialBelief.find(state);
	if (belief == initialBelief.end()) {
		return 0.0;
	} else {
		return belief->second;
	}
}

/**
 * Reset the initial state and the initial belief.
 */
void InitialState::reset()
{
	initialState = nullptr;
	initialBelief.clear();
}

