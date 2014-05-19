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


#include "../../../include/core/states/belief_state.h"

/**
 * The default constructor of the BeliefState object.
 */
BeliefState::BeliefState()
{ }

/**
 * The copy constructor of the BeliefState object.
 * @param other The belief state to copy.
 */
BeliefState::BeliefState(const BeliefState &other)
{
	*this = other;
}

/**
 * The default deconstructor of the BeliefState object.
 */
BeliefState::~BeliefState()
{
	reset();
}

/**
 * Set the probability of a state.
 * @param state			The state to set a belief over.
 * @param probability	The probability of the state.
 */
void BeliefState::set(const State *state, double probability)
{
	belief[state] = probability;
}

/**
 * Get the probability of a state.
 * @param state The state to get a belief over.
 * @return The belief probability of the state.
 */
double BeliefState::get(const State *state) const
{
	std::map<const State *, double>::const_iterator result = belief.find(state);
	if (result == belief.end()) {
		return 0.0;
	} else {
		return result->second;
	}
}

/**
 * Overload the equals operator to set this belief state equal to the belief state provided.
 * @param other The belief state to copy.
 * @return The new version of this belief state.
 */
BeliefState &BeliefState::operator=(const BeliefState &other)
{
	belief = other.belief;
	return *this;
}

/**
 * Reset the belief state.
 */
void BeliefState::reset()
{
	belief.clear();
}
