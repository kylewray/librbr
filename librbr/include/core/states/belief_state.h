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


#ifndef BELIEF_STATE_H
#define BELIEF_STATE_H


#include <map>

#include "state.h"

/**
 * A belief over a set of states.
 */
class BeliefState {
public:
	/**
	 * The default constructor of the BeliefState object.
	 */
	BeliefState();

	/**
	 * The copy constructor of the BeliefState object.
	 * @param other The belief state to copy.
	 */
	BeliefState(const BeliefState &other);

	/**
	 * The default deconstructor of the BeliefState object.
	 */
	virtual ~BeliefState();

	/**
	 * Set the probability of a state.
	 * @param state			The state to set a belief over.
	 * @param probability	The probability of the state.
	 */
	void set(const State *state, double probability);

	/**
	 * Get the probability of a state.
	 * @param state The state to get a belief over.
	 * @return The belief probability of the state.
	 */
	double get(const State *state) const;

	/**
	 * Overload the equals operator to set this belief state equal to the belief state provided.
	 * @param other The belief state to copy.
	 * @return The new version of this belief state.
	 */
	BeliefState &operator=(const BeliefState &other);

	/**
	 * Reset the belief state.
	 */
	void reset();

private:
	/**
	 * The belief over the states. If a state is not mapped, then it
	 * is assumed that the probability for that state is 0.
	 */
	std::map<const State *, double> belief;

};


#endif // BELIEF_STATE_H
