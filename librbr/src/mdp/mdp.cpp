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


#include "../../include/mdp/mdp.h"

MDP::MDP()
{
	states = nullptr;
	actions = nullptr;
	stateTransitions = nullptr;
	rewards = nullptr;
	initialState = nullptr;
	horizon = nullptr;
}

MDP::MDP(States *S, Actions *A, StateTransitions *T, Rewards *R, Initial *s, Horizon *h)
{
	states = S;
	actions = A;
	stateTransitions = T;
	rewards = R;
	initialState = s;
	horizon = h;
}

MDP::~MDP()
{
	if (states != nullptr) {
		delete states;
	}
	if (actions != nullptr) {
		delete actions;
	}
	if (stateTransitions != nullptr) {
		delete stateTransitions;
	}
	if (rewards != nullptr) {
		delete rewards;
	}
	if (initialState != nullptr) {
		delete initialState;
	}
	if (horizon != nullptr) {
		delete horizon;
	}
}

const States *MDP::get_states() const
{
	return states;
}

const Actions *MDP::get_actions() const
{
	return actions;
}

const StateTransitions *MDP::get_state_transitions() const
{
	return stateTransitions;
}

const Rewards *MDP::get_rewards() const
{
	return rewards;
}

const Initial *MDP::get_initial_state() const
{
	return initialState;
}

const Horizon *MDP::get_horizon() const
{
	return horizon;
}
