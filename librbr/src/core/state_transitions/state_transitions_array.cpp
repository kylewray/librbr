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


#include "../../../include/core/state_transitions/state_transitions_array.h"
#include "../../../include/core/state_transitions/state_transition_exception.h"

#include "../../../include/core/states/states_map.h"

#include "../../../include/core/states/indexed_state.h"
#include "../../../include/core/actions/indexed_action.h"

StateTransitionsArray::StateTransitionsArray(unsigned int numStates, unsigned int numActions)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	stateTransitions = new float[states * actions * states];

	reset();
}

StateTransitionsArray::~StateTransitionsArray()
{
	delete [] stateTransitions;
}

void StateTransitionsArray::set(const State *state, const Action *action, const State *nextState, double probability)
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);
	const IndexedState *sp = dynamic_cast<const IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions || sp->get_index() >= states) {
		throw StateTransitionException();
	}

	stateTransitions[s->get_index() * actions * states +
	                 a->get_index() * states +
	                 sp->get_index()] = (float)std::max(0.0, std::min(1.0, probability));
}

double StateTransitionsArray::get(const State *state, const Action *action, const State *nextState) const
{
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(action);
	const IndexedState *sp = dynamic_cast<const IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions || sp->get_index() >= states) {
		throw StateTransitionException();
	}

	return stateTransitions[s->get_index() * actions * states +
	                        a->get_index() * states +
	                        sp->get_index()];
}

void StateTransitionsArray::successors(const States *S, const State *state,
		const Action *action, std::vector<const State *> &result) const
{
	// ToDo: Create a StatesArray object, and replace this cast with that instead.
	const StatesMap *SMap = static_cast<const StatesMap *>(S);
	if (SMap == nullptr) {
		throw StateTransitionException();
	}

	result.clear();
	for (auto sp : *SMap) {
		const State *nextState = resolve(sp);
		if (get(state, action, nextState) > 0.0) {
			result.push_back(nextState);
		}
	}
}

void StateTransitionsArray::set_state_transitions(const float *T)
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			for (int sp = 0; sp < states; sp++) {
				stateTransitions[s * actions * states + a * states + sp] =
						T[s * actions * states + a * states + sp];
			}
		}
	}
}

const float *StateTransitionsArray::get_state_transitions() const
{
	return (const float *)stateTransitions;
}

unsigned int StateTransitionsArray::get_num_states() const
{
	return states;
}

unsigned int StateTransitionsArray::get_num_actions() const
{
	return actions;
}

void StateTransitionsArray::reset()
{
	for (int s = 0; s < states; s++) {
		for (int a = 0; a < actions; a++) {
			for (int sp = 0; sp < states; sp++) {
				stateTransitions[s * actions * states +
				                 a * states +
				                 sp] = 0.0f;
			}
		}
	}
}
