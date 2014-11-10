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


#include "../../include/management/conversion.h"

#include "../../include/core/initial.h"
#include "../../include/core/horizon.h"
#include "../../include/core/core_exception.h"

#include "../../include/core/states/states_map.h"
//#include "../../include/core/states/states_array.h"

#include "../../include/core/actions/actions_map.h"
//#include "../../include/core/actions/actions_array.h"

#include "../../include/core/state_transitions/state_transitions.h"
#include "../../include/core/state_transitions/state_transitions_map.h"
#include "../../include/core/state_transitions/state_transitions_array.h"

#include "../../include/core/rewards/sas_rewards.h"
#include "../../include/core/rewards/sas_rewards_map.h"
#include "../../include/core/rewards/sas_rewards_array.h"

#include "../../include/core/states/named_state.h"
#include "../../include/core/states/indexed_state.h"

#include "../../include/core/actions/named_action.h"
#include "../../include/core/actions/indexed_action.h"

#include <unordered_map>

//#include <iostream>   // ToDo: REMOVE ME!

MDP *convert_map_to_array(MDP *mdp)
{
	if (mdp == nullptr) {
		throw CoreException();
	}

	// Check the validity of the MDP's components by attempting to cast the
	// states, actions, state transitions, rewards, and initial state.
	StatesMap *states = dynamic_cast<StatesMap *>(mdp->get_states());
	ActionsMap *actions = dynamic_cast<ActionsMap *>(mdp->get_actions());

	StateTransitionsMap *stateTransitions =
			dynamic_cast<StateTransitionsMap *>(mdp->get_state_transitions());
	SASRewardsMap *rewards = dynamic_cast<SASRewardsMap *>(mdp->get_rewards());

	Horizon *horizon = dynamic_cast<Horizon *>(mdp->get_horizon());

	if (states == nullptr || actions == nullptr || stateTransitions == nullptr ||
			rewards == nullptr) {
		throw CoreException();
	}

	return convert_map_to_array(states, actions, stateTransitions, rewards, horizon);
}

MDP *convert_map_to_array(StatesMap *states, ActionsMap *actions,
		StateTransitions *stateTransitions, SASRewards *rewards,
		Horizon *horizon)
{
	// Next, create the states object. As part of this, we must create a mapping from the
	// indexed states to the original states. Also create the initial state in the process.
	StatesMap *S = new StatesMap();
	std::unordered_map<unsigned int, State *> convertStates;

	IndexedState::reset_indexer();
	for (auto state : *states) {
		State *indexedState = new IndexedState();
		S->add(indexedState);
		convertStates[indexedState->hash_value()] = resolve(state);
	}

	// Create the actions object in a similar manner, with its own mapping.
	ActionsMap *A = new ActionsMap();
	std::unordered_map<unsigned int, Action *> convertActions;

	IndexedAction::reset_indexer();
	for (auto action : *actions) {
		Action *indexedAction = new IndexedAction();
		A->add(indexedAction);
		convertActions[indexedAction->hash_value()] = resolve(action);
	}

	// With our states and actions maps, we may now create the state transitions and the rewards.
	StateTransitionsArray *T = new StateTransitionsArray(S->get_num_states(), A->get_num_actions());
	SASRewardsArray *R = new SASRewardsArray(S->get_num_states(), A->get_num_actions());

	for (auto state : *S) {
		State *s = resolve(state);
		State *sOriginal = convertStates[s->hash_value()];

		for (auto action : *A) {
			Action *a = resolve(action);
			Action *aOriginal = convertActions[a->hash_value()];

			for (auto nextState : *S) {
				State *sp = resolve(nextState);
				State *spOriginal = convertStates[sp->hash_value()];

//				std::cout << "(" << sOriginal->to_string() << ", " << aOriginal->to_string() << ", " << spOriginal->to_string() << ")\n";
//				std::cout.flush();

				T->set(s, a, sp, stateTransitions->get(sOriginal, aOriginal, spOriginal));
				R->set(s, a, sp, rewards->get(sOriginal, aOriginal, spOriginal));
			}
		}
	}

	// Finally, create a copy of the horizon.
	Horizon *h = nullptr;
	if (horizon->is_finite()) {
		h = new Horizon(horizon->get_horizon());
	} else {
		h = new Horizon(horizon->get_discount_factor());
	}

	return new MDP(S, A, T, R, h);
}

POMDP *convert_map_to_array(const POMDP *pomdp)
{
	// ToDo: Implement.
	return nullptr;
}
