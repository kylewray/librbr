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


#include "../../../include/core/states/finite_factored_states.h"
#include "../../../include/core/states/factored_state.h"
#include "../../../include/core/states/state_exception.h"

#include <algorithm>

FiniteFactoredStates::FiniteFactoredStates()
{ }

FiniteFactoredStates::FiniteFactoredStates(int numFactors)
{
	if (numFactors < 1) {
		numFactors = 1;
	}
	factoredStates.resize(numFactors);
}

FiniteFactoredStates::~FiniteFactoredStates()
{
	reset();
}

void FiniteFactoredStates::add_factor(const std::vector<const State *> &newStates)
{
	if (newStates.size() == 0) {
		throw StateException();
	}

	for (const State *state : newStates) {
		if (state == nullptr) {
			throw StateException();
		}
	}

	factoredStates.push_back(newStates);
}

void FiniteFactoredStates::add(int factorIndex, const State *newState)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size()) {
		throw StateException();
	}

	factoredStates[factorIndex].push_back(newState);
}

void FiniteFactoredStates::remove(int factorIndex, const State *removeState)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size()) {
		throw StateException();
	} else if (std::find(factoredStates[factorIndex].begin(), factoredStates[factorIndex].end(),
			removeState) == factoredStates[factorIndex].end()) {
		throw StateException();
	}

	factoredStates[factorIndex].erase(
			std::remove(factoredStates[factorIndex].begin(), factoredStates[factorIndex].end(), removeState),
			factoredStates[factorIndex].end());
	delete removeState;
}

void FiniteFactoredStates::set(int factorIndex, const std::vector<const State *> &newStates)
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size() || newStates.size() == 0) {
		throw StateException();
	}

	// Delete the current factor's states list.
	for (const State *state : factoredStates[factorIndex]) {
		delete state;
	}
	factoredStates[factorIndex].clear();

	factoredStates[factorIndex] = newStates;
}

const State *FiniteFactoredStates::get(int factorIndex, int stateIndex) const
{
	if (factorIndex < 0 || factorIndex >= factoredStates.size() ||
			stateIndex < 0 || stateIndex >= factoredStates[factorIndex].size()) {
		throw StateException();
	}

	return factoredStates[factorIndex][stateIndex];
}

void FiniteFactoredStates::update()
{
	// Throw an error if one factor is not defined.
	for (std::vector<const State *> &factor : factoredStates) {
		if (factor.size() == 0) {
			throw StateException();
		}
	}

	states.clear();

	std::vector<const State *> create;
	update_step(create, 0);
}

int FiniteFactoredStates::get_num_factors()
{
	return factoredStates.size();
}

void FiniteFactoredStates::reset()
{
	for (std::vector<const State *> &factor : factoredStates) {
		for (const State *state : factor) {
			delete state;
		}
		factor.clear();
	}

	states.clear();
}

void FiniteFactoredStates::update_step(std::vector<const State *> currentFactoredState, int currentFactorIndex)
{
	// At the final factor index, we need to create a bunch of factored states.
	for (const State *state : factoredStates[currentFactorIndex]) {
		// Begin by pushing a current factor's state on the vector (tuple).
		currentFactoredState.push_back(state);

		// If this is the final index, then create a factored state object and append it to the list of states.
		// Otherwise, recurse to the next index, using the new currentFactoredState object.
		if (currentFactorIndex == factoredStates.size() - 1) {
			const FactoredState *newState = new FactoredState(currentFactoredState);
			states[newState->hash_value()] = newState;
		} else {
			update_step(currentFactoredState, currentFactorIndex + 1);
		}

		// Pop off the state that was just appended, to prepare for the next loop.
		currentFactoredState.pop_back();
	}
}
