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


#include "../../../include/core/state_transitions/state_transitions_map.h"
#include "../../../include/core/state_transitions/state_transition_exception.h"

#include "../../../include/core/states/states_map.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"

StateTransitionsMap::StateTransitionsMap()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");
}

StateTransitionsMap::~StateTransitionsMap()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
}

void StateTransitionsMap::set(State *state, Action *action, State *nextState, double probability)
{
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}
	if (nextState == nullptr) {
		nextState = stateWildcard;
	}

	stateTransitions[state][action][nextState] = std::max(0.0, std::min(1.0, probability));
}

double StateTransitionsMap::get(State *state, Action *action, State *nextState)
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		State *alpha = stateWildcard;
		if (i & (1 << 0)) {
			alpha = state;
		}

		Action *beta = actionWildcard;
		if (i & (1 << 1)) {
			beta = action;
		}

		State *gamma = stateWildcard;
		if (i & (1 << 2)) {
			gamma = nextState;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const StateTransitionException &err) { }
	}

	return 0.0;
}

void StateTransitionsMap::successors(States *S, State *state,
		Action *action, std::vector<State *> &result)
{
	// ToDo: Create a StatesArray object, and replace this cast with that instead.
	StatesMap *SMap = dynamic_cast<StatesMap *>(S);
	if (SMap == nullptr) {
		throw StateTransitionException();
	}

	result.clear();
	for (auto sp : *SMap) {
		State *nextState = resolve(sp);
		if (get(state, action, nextState) > 0.0) {
			result.push_back(nextState);
		}
	}
}

void StateTransitionsMap::reset()
{
	stateTransitions.clear();
}

double StateTransitionsMap::get_value(State *state, Action *action, State *nextState)
{
	std::unordered_map<State *,
		std::unordered_map<Action *,
		std::unordered_map<State *, double> > >::const_iterator alpha = stateTransitions.find(state);
	if (alpha == stateTransitions.end()) {
		throw StateTransitionException();
	}

	std::unordered_map<Action *,
		std::unordered_map<State *, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw StateTransitionException();
	}

	std::unordered_map<State *, double>::const_iterator gamma = beta->second.find(nextState);
	if (gamma == beta->second.end()) {
		throw StateTransitionException();
	}

	return gamma->second;
}
