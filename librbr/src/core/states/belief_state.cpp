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


#include "../../../include/core/states/belief_state.h"

BeliefState::BeliefState()
{ }

BeliefState::BeliefState(const BeliefState &other)
{
	*this = other;
}

BeliefState::~BeliefState()
{
	reset();
}

void BeliefState::set(State *state, double probability)
{
	belief[state] = std::min(1.0, std::max(0.0, probability));

	states.push_back(state);
}

double BeliefState::get(State *state) const
{
	std::map<State *, double>::const_iterator result = belief.find(state);
	if (result == belief.end()) {
		return 0.0;
	} else {
		return result->second;
	}
}

const std::vector<State *> &BeliefState::get_states()
{
	return states;
}

BeliefState &BeliefState::operator=(const BeliefState &other)
{
	belief = other.belief;
	states = other.states;
	return *this;
}

void BeliefState::reset()
{
	belief.clear();
	states.clear();
}

