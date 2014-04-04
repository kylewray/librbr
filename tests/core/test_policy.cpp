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


// Only perform the tests if the flag is set during compilation.
#ifdef PERFORM_TESTS_H


#include <iostream>

#include "../../include/core/policy/policy_map.h"
#include "../../include/core/policy/policy_tree.h"
#include "../../include/core/policy/policy_exception.h"

#include "../../include/core/states/named_state.h"
#include "../../include/core/states/finite_states.h"
#include "../../include/core/actions/finite_actions.h"
#include "../../include/core/observations/finite_observations.h"
#include "../../include/core/horizon.h"

/**
 * Test the policy objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_policy()
{
	int numSuccesses = 0;

	State *s1 = new NamedState("s1");
	State *s2 = new NamedState("s2");

	Action *a1 = new Action("a1");
	Action *a2 = new Action("a2");

	FiniteStates *states = new FiniteStates({s1, s2});
	FiniteActions *actions = new FiniteActions({a1, a2});
	Horizon *horizon = new Horizon((unsigned int)4);

	PolicyMap *policyMap = new PolicyMap();

	std::cout << "Policy: 'PolicyMap::load' file 'test_01.policy_map'... ";
	if (!policyMap->load("tests/resources/policy/test_01.policy_map", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::get' (Check Result)... ";
	try {
		if (policyMap->get(s1) == a2 && policyMap->get(s2) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::set'... ";

	try {
		policyMap->set(s1, a1);
		policyMap->set(s2, a2);

		if (policyMap->get(s1) == a1 && policyMap->get(s2) == a2) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'test_02.policy_map' (Expecting Error)...\n\t";
	if (policyMap->load("tests/resources/policy/test_02.policy_map", states, actions, horizon)) {
		std::cout << "\tSuccess." << std::endl;
		numSuccesses++;
	} else {
		std::cout << "\tFailure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::load' file 'test_03.policy_map'... ";
	if (!policyMap->load("tests/resources/policy/test_03.policy_map", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::save' file 'test_03.policy_map'... ";
	if (!policyMap->save("tests/tmp/test_03.policy_map")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::load' file 'test_03.policy_map' (Check Result)... ";
	if (!policyMap->load("tests/tmp/test_03.policy_map", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::get' (Check Result)... ";
	try {
		if (policyMap->get(0, s1) == a2 && policyMap->get(0, s2) == a1 &&
				policyMap->get(1, s1) == a1 && policyMap->get(1, s2) == a2 &&
				policyMap->get(2, s1) == a1 && policyMap->get(2, s2) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'PolicyTree::load' file 'test_04.policy_tree'... ";

	Observation *o1 = new Observation("o1");
	Observation *o2 = new Observation("o2");

	FiniteObservations *observations = new FiniteObservations({o1, o2});

	PolicyTree *policyTree = new PolicyTree(observations, horizon);

	if (!policyTree->load("tests/resources/policy/test_04.policy_tree", actions, observations, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyTree::get' (Check Result)... ";

	const std::vector<const Observation *> history;
	const std::vector<const Observation *> history1 = {o1};
	const std::vector<const Observation *> history2 = {o2};
	const std::vector<const Observation *> history11 = {o1, o1};
	const std::vector<const Observation *> history12 = {o1, o2};
	const std::vector<const Observation *> history21 = {o2, o1};
	const std::vector<const Observation *> history22 = {o2, o2};
	const std::vector<const Observation *> history111 = {o1, o1, o1};
	const std::vector<const Observation *> history112 = {o1, o1, o2};
	const std::vector<const Observation *> history121 = {o1, o2, o1};
	const std::vector<const Observation *> history122 = {o1, o2, o2};
	const std::vector<const Observation *> history211 = {o2, o1, o1};
	const std::vector<const Observation *> history212 = {o2, o1, o2};
	const std::vector<const Observation *> history221 = {o2, o2, o1};
	const std::vector<const Observation *> history222 = {o2, o2, o2};
	const std::vector<const Observation *> history1111 = {o1, o1, o1, o1};

	try {
		if (policyTree->get(history) == a1 &&
				policyTree->get(history1) == a2 && policyTree->get(history2) == a1 &&
				policyTree->get(history11) == a1 && policyTree->get(history12) == a2 &&
				policyTree->get(history21) == a2 && policyTree->get(history22) == a1 &&
				policyTree->get(history111) == a1 && policyTree->get(history112) == a2 &&
				policyTree->get(history121) == a1 && policyTree->get(history122) == a2 &&
				policyTree->get(history211) == a2 && policyTree->get(history212) == a1 &&
				policyTree->get(history221) == a2 && policyTree->get(history222) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyTree::set'... ";

	try {
		policyTree->set(history2, a1);
		policyTree->set(history12, a2);
		policyTree->set(history21, a1);
		policyTree->set(history221, a1);
		policyTree->set(history212, a2);

		if (policyTree->get(history2) == a1 && policyTree->get(history12) == a2 &&
				policyTree->get(history21) == a1 && policyTree->get(history221) == a1 &&
				policyTree->get(history212) == a2) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	// Note: The state and action variables are freed inside the deconstructors of states and actions.
	delete states;
	states = nullptr;

	delete actions;
	actions = nullptr;

	delete observations;
	observations = nullptr;

	delete horizon;
	horizon = nullptr;

	delete policyMap;
	policyMap = nullptr;

	delete policyTree;
	policyTree = nullptr;

	return numSuccesses;
}


#endif // PERFORM_TESTS_H
