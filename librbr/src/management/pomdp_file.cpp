/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2015 Kyle Hollins Wray, University of Massachusetts
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


#include "../../include/management/pomdp_file.h"

#include "../../include/utilities/log.h"
#include "../../include/utilities/string_manipulation.h"

#include "../../include/core/core_exception.h"

#include "../../include/core/states/states_map.h"
#include "../../include/core/states/indexed_state.h"

#include "../../include/core/actions/actions_map.h"
#include "../../include/core/actions/indexed_action.h"

#include "../../include/core/observations/observations_map.h"
#include "../../include/core/observations/indexed_observation.h"

#include "../../include/core/state_transitions/state_transitions_array.h"

#include "../../include/core/observation_transitions/observation_transitions_array.h"

#include "../../include/core/rewards/rewards.h"
#include "../../include/core/rewards/factored_rewards.h"
#include "../../include/core/rewards/sa_rewards.h"
#include "../../include/core/rewards/sa_rewards_array.h"
#include "../../include/core/rewards/reward_exception.h"

#include "../../include/core/initial.h"
#include "../../include/core/horizon.h"

POMDPFile::POMDPFile()
{ }

POMDPFile::~POMDPFile()
{ }


void POMDPFile::save_pomdp(POMDP *pomdp, std::string filename, BeliefState *start)
{
	// Load the file and throw an error if this cannot be done.
	std::ofstream file(filename);

	if (!file.is_open()) {
		log_message("POMDPFile::save_pomdp", "Failed to create the file '" + filename + "'.");
		throw CoreException();
	}

	// Note: FactoredStates, and even JointActions, will automatically work, because the
	// entire set of all possible state tuples should be generated. State transitions will
	// also already be defined for this. It is different for FactoredRewards, which do *not*
	// have a 'flattened' representation. They must be treated for each possible case.

	// First, obtain all the objects for easy reference. Also, check to make sure this
	// is a valid MDP.
	StatesMap *S = dynamic_cast<StatesMap *>(pomdp->get_states());
	ActionsMap *A = dynamic_cast<ActionsMap *>(pomdp->get_actions());
	ObservationsMap *Z = dynamic_cast<ObservationsMap *>(pomdp->get_observations());
	StateTransitions *T = pomdp->get_state_transitions();
	ObservationTransitions *O = pomdp->get_observation_transitions();
	Rewards *R = nullptr;
	SARewards *Ri = nullptr;
	Horizon *h = pomdp->get_horizon();

	R = dynamic_cast<FactoredRewards *>(pomdp->get_rewards());
	if (R == nullptr) {
		Ri = dynamic_cast<SARewards *>(pomdp->get_rewards());
		if (Ri == nullptr) {
			throw CoreException();
		}
	} else {
		FactoredRewards *RF = dynamic_cast<FactoredRewards *>(R);
		if (RF == nullptr) {
			throw CoreException();
		}

		// Note: We attempt to cast the first one, and assume that the rest are of the same class.
		Rewards *R0 = nullptr;
		try {
			R0 = RF->get(0);
		} catch (const RewardException &err) {
			log_message("POMDPFile::save_pomdp",
					"Found null reward, and thus failed to save the file '" + filename + "'.");
			throw CoreException();
		}

		// We only handle SARewards and only take the first reward factor.
		Ri = dynamic_cast<SARewards *>(R0);
		if (Ri == nullptr) {
			throw CoreException();
		}
	}

	if (S == nullptr || S->get_num_states() == 0 || A == nullptr || A->get_num_actions() == 0 ||
			T == nullptr || O == nullptr || R == nullptr || h == nullptr) {
		log_message("POMDPFile::save_pomdp",
				"Failed to parse the MDP provided and save the file '" + filename + "'.");
		throw CoreException();
	}

	// Create the header which contains state, action, and horizon information.
	file << "discount: " << h->get_discount_factor() << std::endl;
	file << "values: reward" << std::endl;
	file << "states: " << S->get_num_states() << std::endl;
	file << "actions: " << A->get_num_actions() << std::endl;
	file << "observations: " << Z->get_num_observations() << std::endl;

	// Note: Assume uniform belief over these starting states.
	file << "start include:" << std::endl;
	for (State *s : start->get_states()) {
		file << s->hash_value() << " ";
	}

	file << std::endl;

	// Write all of the state transition information.
	for (auto action : *A) {
		Action *a = resolve(action);

		for (auto state : *S) {
			State *s = resolve(state);

			for (State *sp : T->successors(S, s, a)) {
				file << "T: " << a->hash_value() << " : " << s->hash_value() << " : " << sp->hash_value() << " " << T->get(s, a, sp) << std::endl;
			}
		}
	}

	file << std::endl;

	// Write all of the state transition information.
	for (auto action : *A) {
		Action *a = resolve(action);

		for (auto nextState : *S) {
			State *sp = resolve(nextState);

			file << "O: " << a->hash_value() << " : " << sp->hash_value() << std::endl;

			for (auto observation : *Z) {
				Observation *z = resolve(observation);

				 file << O->get(a, sp, z) << " ";
			}

			file << std::endl;
		}
	}

	file << std::endl;

	// Write all of the reward information.
	for (auto action : *A) {
		Action *a = resolve(action);

		for (auto state : *S) {
			State *s = resolve(state);

			file << "R: " << a->hash_value() << " : " << s->hash_value() << " : * : * " << Ri->get(s, a) << std::endl;
		}
	}

	file.close();
}
