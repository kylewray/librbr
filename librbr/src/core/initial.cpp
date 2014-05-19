/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#include "../../include/core/initial.h"

/**
 * The constructor for the Initial class.
 */
Initial::Initial()
{
	initialState = nullptr;
}

/**
 * The constructor for the Initial class, specifying the true initial state.
 * @param state The true initial state.
 */
Initial::Initial(const State *state)
{
	initialState = state;
}

/**
 * The constructor for the Initial class, specifying the belief over the states.
 * @param belief The belief over the states.
 */
Initial::Initial(const BeliefState &belief)
{
	initialState = nullptr;
	initialBelief = belief;
}

/**
 * The deconstructor for the Initial class.
 */
Initial::~Initial()
{
	reset();
}

/**
 * Set the true initial state.
 * @param state The true initial state.
 */
void Initial::set_initial_state(const State *state)
{
	initialState = nullptr;
	initialBelief.reset();
}

/**
 * Set the initial belief over states.
 * @param belief The belief over the states.
 */
void Initial::set_initial_belief(const BeliefState &belief)
{
	initialState = nullptr;
	initialBelief = belief;
}

/**
 * Set the probability of a particular state, assuming the belief has been created.
 * @param state 		The state to which the probability will be assigned.
 * @param probability	The probability of that the initial state is the state given.
 */
void Initial::set_initial_belief(const State *state, double probability)
{
	initialState = nullptr;
	initialBelief.set(state, probability);
}

/**
 * Get the true initial state.
 * @return The true initial state.
 */
const State *Initial::get_initial_state() const
{
	return initialState;
}

/**
 * Get the probability of a particular state.
 * @param state The state to which the probability will be assigned.
 * @return The probability of that the initial state is the state given.
 */
const BeliefState &Initial::get_initial_belief() const
{
	return initialBelief;
}

/**
 * Reset the initial state and the initial belief.
 */
void Initial::reset()
{
	initialState = nullptr;
	initialBelief.reset();
}
