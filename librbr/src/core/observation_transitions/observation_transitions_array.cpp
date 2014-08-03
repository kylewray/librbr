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


#include "../../../include/core/observation_transitions/observation_transitions_array.h"
#include "../../../include/core/observation_transitions/observation_transition_exception.h"

#include "../../../include/core/observations/observations_map.h"

#include "../../../include/core/states/indexed_state.h"
#include "../../../include/core/actions/indexed_action.h"
#include "../../../include/core/observations/indexed_observation.h"

ObservationTransitionsArray::ObservationTransitionsArray(unsigned int numStates,
		unsigned int numActions, unsigned int numObservations)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	observations = numObservations;
	if (observations == 0) {
		observations = 1;
	}

	observationTransitions = new float[actions * states * observations];

	reset();
}

ObservationTransitionsArray::~ObservationTransitionsArray()
{
	delete [] observationTransitions;
}

void ObservationTransitionsArray::set(const Action *previousAction, const State *state,
		const Observation *observation, double probability)
{
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(previousAction);
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedObservation *z = dynamic_cast<const IndexedObservation *>(observation);

	if (a == nullptr || s == nullptr || z == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || s->get_index() >= states || z->get_index() >= observations) {
		throw ObservationTransitionException();
	}

	observationTransitions[a->get_index() * states * observations +
	                       s->get_index() * observations +
	                       z->get_index()] =
			(float)std::max(0.0, std::min(1.0, probability));
}

double ObservationTransitionsArray::get(const Action *previousAction, const State *state,
		const Observation *observation) const
{
	const IndexedAction *a = dynamic_cast<const IndexedAction *>(previousAction);
	const IndexedState *s = dynamic_cast<const IndexedState *>(state);
	const IndexedState *z = dynamic_cast<const IndexedState *>(observation);

	if (a == nullptr || s == nullptr || z == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || s->get_index() >= states || z->get_index() >= observations) {
		throw ObservationTransitionException();
	}

	return observationTransitions[a->get_index() * states * observations +
	   	                       s->get_index() * observations +
	   	                       z->get_index()];
}

void ObservationTransitionsArray::available(const Observations *Z, const Action *previousAction, const State *state,
		std::vector<const Observation *> &result) const
{
	// ToDo: Create an ObservationsArray object, and replace this cast with that instead.
	const ObservationsMap *ZMap = static_cast<const ObservationsMap *>(Z);
	if (ZMap == nullptr) {
		throw ObservationTransitionException();
	}

	result.clear();
	for (auto z : *ZMap) {
		const Observation *observation = resolve(z);
		if (get(previousAction, state, observation) > 0.0) {
			result.push_back(observation);
		}
	}
}

void ObservationTransitionsArray::set_observation_transitions(const float *O)
{
	for (int a = 0; a < actions; a++) {
		for (int sp = 0; sp < states; sp++) {
			for (int z = 0; z < observations; z++) {
				observationTransitions[a * states * observations + sp * observations + z] =
						O[a * states * observations + sp * observations + z];
			}
		}
	}
}

const float *ObservationTransitionsArray::get_observation_transitions() const
{
	return (const float *)observationTransitions;
}

unsigned int ObservationTransitionsArray::get_num_states() const
{
	return states;
}

unsigned int ObservationTransitionsArray::get_num_actions() const
{
	return actions;
}

unsigned int ObservationTransitionsArray::get_num_observations() const
{
	return observations;
}

void ObservationTransitionsArray::reset()
{
	for (int a = 0; a < actions; a++) {
		for (int s = 0; s < states; s++) {
			for (int z = 0; z < observations; z++) {
				observationTransitions[a * states * observations +
				                       s * observations +
				                       z] = 0.0f;
			}
		}
	}
}
