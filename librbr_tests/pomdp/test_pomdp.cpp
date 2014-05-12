/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2013 Kyle Wray and Luis Pineda
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


#include "../perform_tests.h"

#include <iostream>

#include "../../librbr/include/file_loaders/unified_file.h"

#include "../../librbr/include/pomdp/pomdp.h"
#include "../../librbr/include/pomdp/pomdp_value_iteration.h"

#include "../../librbr/include/core/core_exception.h"
#include "../../librbr/include/core/states/state_exception.h"
#include "../../librbr/include/core/actions/action_exception.h"
#include "../../librbr/include/core/observations/observation_exception.h"
#include "../../librbr/include/core/state_transitions/state_transition_exception.h"
#include "../../librbr/include/core/observation_transitions/observation_transition_exception.h"
#include "../../librbr/include/core/rewards/reward_exception.h"
#include "../../librbr/include/core/policy/policy_exception.h"

/**
 * Test the POMDP solvers. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_pomdp()
{
	int numSuccesses = 0;
	UnifiedFile file;

	std::cout << "POMDP: Loading 'tiger.pomdp'...";
	if (!file.load("resources/pomdp/tiger.pomdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "POMDP: Solving 'tiger.pomdp' with POMDPValueIteration...";

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

	if (pomdp != nullptr) {
		policyAlphaVectors->save("tmp/test_pomdp_value_iteration_finite_horizon.pomdp_alpha_vectors",
				(const FiniteStates *)pomdp->get_states());
		delete pomdp;
	}
	pomdp = nullptr;

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	/*
	std::cout << "MDP: Loading 'grid_world_infinite_horizon.mdp'...";
//	if (!file.load("/Users/infinite/Downloads/hallway.POMDP")) {
	if (!file.load("tests/resources/mdp/grid_world_infinite_horizon.mdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPValueIteration...";

	mdp = file.get_mdp();

	try {
		mapPolicy = vi.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	mapPolicy->save("tests/tmp/test_mdp_value_iteration_infinite_horizon.policy_map");

	delete mapPolicy;
	mapPolicy = nullptr;

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPPolicyIteration (Exact)...";

	MDPPolicyIteration piExact;

	try {
		mapPolicy = piExact.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	mapPolicy->save("tests/tmp/test_mdp_policy_iteration_exact_infinite_horizon.policy_map");

	delete mapPolicy;
	mapPolicy = nullptr;

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPPolicyIteration (Modified with k=5)...";

	MDPPolicyIteration piModified(5);

	try {
		mapPolicy = piModified.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const StateTransitionException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const RewardException &err) {
		std::cout << " Failure." << std::endl;
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	mapPolicy->save("tests/tmp/test_mdp_policy_iteration_modified_infinite_horizon.policy_map");

	delete mdp;
	mdp = nullptr;

	delete mapPolicy;
	mapPolicy = nullptr;

	*/

	return numSuccesses;
}
