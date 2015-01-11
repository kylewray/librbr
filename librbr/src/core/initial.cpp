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


#include "../../include/core/initial.h"

Initial::Initial()
{
	initialState = nullptr;
}

Initial::Initial(State *state)
{
	initialState = state;
}

Initial::Initial(BeliefState &belief)
{
	initialState = nullptr;
	initialBelief = belief;
}

Initial::~Initial()
{
	reset();
}

void Initial::set_initial_state(State *state)
{
	initialState = nullptr;
	initialBelief.reset();
}

void Initial::set_initial_belief(BeliefState &belief)
{
	initialState = nullptr;
	initialBelief = belief;
}

void Initial::set_initial_belief(State *state, double probability)
{
	initialState = nullptr;
	initialBelief.set(state, probability);
}

State *Initial::get_initial_state()
{
	return initialState;
}

BeliefState &Initial::get_initial_belief()
{
	return initialBelief;
}

void Initial::reset()
{
	initialState = nullptr;
	initialBelief.reset();
}
