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


#include "../../include/perform_tests.h"

#include <iostream>

#include "../../../librbr/include/management/unified_file.h"

#include "../../../librbr/include/mdp/mdp.h"
#include "../../../librbr/include/mdp/mdp_value_iteration.h"
#include "../../../librbr/include/mdp/mdp_policy_iteration.h"

#include "../../../librbr/include/core/core_exception.h"
#include "../../../librbr/include/core/states/state_exception.h"
#include "../../../librbr/include/core/actions/action_exception.h"
#include "../../../librbr/include/core/state_transitions/state_transition_exception.h"
#include "../../../librbr/include/core/rewards/reward_exception.h"
#include "../../../librbr/include/core/policy/policy_exception.h"

int test_mdp()
{
	int numSuccesses = 0;
	UnifiedFile file;

	std::cout << "MDP: Loading 'grid_world_finite_horizon.mdp'...";
	if (!file.load("resources/mdp/grid_world_finite_horizon.mdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "MDP: Solving 'grid_world_finite_horizon.mdp' with MDPValueIteration...";

	MDP *mdp = nullptr;
	MDPValueIteration vi;
	PolicyMap *policyMap = nullptr;

	try {
		mdp = file.get_mdp();
		policyMap = vi.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
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

	if (policyMap != nullptr) {
		policyMap->save("tmp/test_mdp_value_iteration_finite_horizon.policy_map");
		delete policyMap;
	}
	policyMap = nullptr;

	if (mdp != nullptr) {
		delete mdp;
	}
	mdp = nullptr;

	std::cout << "MDP: Loading 'grid_world_infinite_horizon.mdp'...";
	if (!file.load("resources/mdp/grid_world_infinite_horizon.mdp")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPValueIteration...";

	try {
		mdp = file.get_mdp();
		policyMap = vi.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
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

	if (policyMap != nullptr) {
		policyMap->save("tmp/test_mdp_value_iteration_infinite_horizon.policy_map");
		delete policyMap;
	}
	policyMap = nullptr;

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPPolicyIteration (Exact)...";

	MDPPolicyIteration piExact;

	try {
		policyMap = piExact.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
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

	if (policyMap != nullptr) {
		policyMap->save("tmp/test_mdp_policy_iteration_exact_infinite_horizon.policy_map");
		delete policyMap;
	}
	policyMap = nullptr;

	std::cout << "MDP: Solving 'grid_world_infinite_horizon.mdp' with MDPPolicyIteration (Modified with k=5)...";

	MDPPolicyIteration piModified(5);

	try {
		policyMap = piModified.solve(mdp);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const CoreException &err) {
		std::cout << " Failure." << std::endl;
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

	if (policyMap != nullptr) {
		policyMap->save("tmp/test_mdp_policy_iteration_modified_infinite_horizon.policy_map");
		delete policyMap;
	}
	policyMap = nullptr;

	delete mdp;
	mdp = nullptr;

	return numSuccesses;
}
