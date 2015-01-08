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


#include "../../include/perform_tests.h"

#include <iostream>

#include "../../../librbr/include/management/unified_file.h"

#include "../../../librbr/include/pomdp/pomdp.h"
#include "../../../librbr/include/pomdp/pomdp_value_iteration.h"
#include "../../../librbr/include/pomdp/pomdp_pbvi.h"

#include "../../../librbr/include/core/states/belief_state.h"
#include "../../../librbr/include/core/states/states_map.h"

#include "../../../librbr/include/core/core_exception.h"
#include "../../../librbr/include/core/states/state_exception.h"
#include "../../../librbr/include/core/actions/action_exception.h"
#include "../../../librbr/include/core/observations/observation_exception.h"
#include "../../../librbr/include/core/state_transitions/state_transition_exception.h"
#include "../../../librbr/include/core/observation_transitions/observation_transition_exception.h"
#include "../../../librbr/include/core/rewards/reward_exception.h"
#include "../../../librbr/include/core/policy/policy_exception.h"

int test_pomdp()
{
	int numSuccesses = 0;
	UnifiedFile file;

	std::cout << "POMDP: Loading 'tiger_finite.pomdp'...";
	if (!file.load("resources/pomdp/tiger_finite.pomdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "POMDP: Solving 'tiger_finite.pomdp' with POMDPValueIteration...";

	POMDP *pomdp = nullptr;

	POMDPValueIteration vi;
	PolicyAlphaVectors *policyAlphaVectors = nullptr;

	try {
		pomdp = file.get_pomdp();
		policyAlphaVectors = vi.solve(pomdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	// Save and destroy the alpha vectors.
	if (pomdp != nullptr) {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		policyAlphaVectors->save("tmp/test_pomdp_vi_finite_horizon.pomdp_alpha_vectors", states);
	}

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	std::cout << "POMDP: Solving 'tiger_finite.pomdp' with POMDPPBVI...";

	POMDPPBVI pbvi;

	try {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		std::vector<const State *> orderedStates;
		for (auto s : *states) {
			orderedStates.push_back(resolve(s));
		}

		BeliefState *b = new BeliefState();
		b->set(orderedStates[0], 1.0);
		b->set(orderedStates[1], 0.0);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.0);
		b->set(orderedStates[1], 1.0);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.25);
		b->set(orderedStates[1], 0.75);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.75);
		b->set(orderedStates[1], 0.25);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.5);
		b->set(orderedStates[1], 0.5);
		pbvi.add_initial_belief_state(b);

		policyAlphaVectors = pbvi.solve(pomdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	// Save and destroy the alpha vectors. Also, We are done with the tiger problem, so destroy it.
	if (pomdp != nullptr) {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		policyAlphaVectors->save("tmp/test_pomdp_pbvi_finite_horizon.pomdp_alpha_vectors", states);
		delete pomdp;
	}
	pomdp = nullptr;

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	std::cout << "POMDP: Loading 'tiger_infinite.pomdp'...";
	if (!file.load("resources/pomdp/tiger_infinite.pomdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "POMDP: Solving 'tiger_infinite.pomdp' with POMDPValueIteration...";

	// Anything above like 5 or 6 iterations produces too many belief points to terminate quickly.
	// Therefore, the resultant values (e.g., -98.2942 and 11.7058) are actually ***underestimates***!
	vi.set_num_iterations(5);

	try {
		pomdp = file.get_pomdp();
		policyAlphaVectors = vi.solve(pomdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	// Save and destroy the alpha vectors.
	if (pomdp != nullptr) {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		policyAlphaVectors->save("tmp/test_pomdp_vi_infinite_horizon.pomdp_alpha_vectors", states);
	}

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	std::cout << "POMDP: Solving 'tiger_infinite.pomdp' with POMDPPBVI...";

	int numExpansions = 4;
	pbvi.set_expansion_rule(POMDPPBVIExpansionRule::STOCHASTIC_SIMULATION_EXPLORATORY_ACTION);
	pbvi.set_num_expansion_iterations(numExpansions);
	pbvi.compute_num_update_iterations(pomdp, 0.01);
	pbvi.set_num_update_iterations(pbvi.get_num_update_iterations() / numExpansions);
	pbvi.reset();

	try {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		std::vector<const State *> orderedStates;
		for (auto s : *states) {
			orderedStates.push_back(resolve(s));
		}

		BeliefState *b = new BeliefState();
		b->set(orderedStates[0], 1.0);
		b->set(orderedStates[1], 0.0);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.0);
		b->set(orderedStates[1], 1.0);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.25);
		b->set(orderedStates[1], 0.75);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.75);
		b->set(orderedStates[1], 0.25);
		pbvi.add_initial_belief_state(b);

		b = new BeliefState();
		b->set(orderedStates[0], 0.5);
		b->set(orderedStates[1], 0.5);
		pbvi.add_initial_belief_state(b);

		policyAlphaVectors = pbvi.solve(pomdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ObservationTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	// Save and destroy the alpha vectors. Also, We are done with the tiger problem, so destroy it.
	if (pomdp != nullptr) {
		StatesMap *states = dynamic_cast<StatesMap *>(pomdp->get_states());
		policyAlphaVectors->save("tmp/test_pomdp_pbvi_infinite_horizon.pomdp_alpha_vectors", states);
		delete pomdp;
	}
	pomdp = nullptr;

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	return numSuccesses;
}
