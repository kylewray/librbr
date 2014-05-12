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

#include "../../librbr/include/core/policy/policy_map.h"
#include "../../librbr/include/core/policy/policy_tree.h"
#include "../../librbr/include/core/policy/policy_alpha_vector.h"
#include "../../librbr/include/core/policy/policy_alpha_vectors.h"
#include "../../librbr/include/core/policy/policy_exception.h"

#include "../../librbr/include/core/states/named_state.h"
#include "../../librbr/include/core/states/belief_state.h"
#include "../../librbr/include/core/states/finite_states.h"
#include "../../librbr/include/core/actions/finite_actions.h"
#include "../../librbr/include/core/observations/finite_observations.h"
#include "../../librbr/include/core/horizon.h"

#include <math.h>

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
	if (!policyMap->load("resources/policy/test_01.policy_map", states, actions, horizon)) {
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
	if (policyMap->load("resources/policy/test_02.policy_map", states, actions, horizon)) {
		std::cout << "\tSuccess." << std::endl;
		numSuccesses++;
	} else {
		std::cout << "\tFailure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::load' file 'test_03.policy_map'... ";
	if (!policyMap->load("resources/policy/test_03.policy_map", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::save' file 'test_03.policy_map'... ";
	if (!policyMap->save("tmp/test_03.policy_map")) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::load' file 'test_03.policy_map' (Check Result)... ";
	if (!policyMap->load("tmp/test_03.policy_map", states, actions, horizon)) {
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

	if (!policyTree->load("resources/policy/test_04.policy_tree", actions, observations, horizon)) {
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

	std::cout << "Policy: Test 'PolicyAlphaVector::set' and 'PolicyAlphaVector::get'... ";

	PolicyAlphaVector *policyAlphaVector = new PolicyAlphaVector(a1);
	policyAlphaVector->set(s1, 13.37);
	policyAlphaVector->set(s2, 42.0);

	if (policyAlphaVector->get(s1) == 13.37 && policyAlphaVector->get(s2) == 42.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVector::get_dimension', 'PolicyAlphaVector::get_action', and 'PolicyAlphaVector::set_action'... ";

	if (policyAlphaVector->get_dimension() == 2 && policyAlphaVector->get_action() == a1) {
		policyAlphaVector->set_action(a2);
		if (policyAlphaVector->get_action() == a2) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVector::compute_value'... ";

	BeliefState *b = new BeliefState();
	b->set(s1, 0.36);
	b->set(s2, 0.64);

	if (fabs((float)(policyAlphaVector->compute_value(b) - 31.6932)) < 0.0001f) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVectors::get'... ";

	std::vector<PolicyAlphaVector *> alphaVectors;
	policyAlphaVector->set_action(a1);
	alphaVectors.push_back(policyAlphaVector);

	// Note: PolicyAlphaVectors now will manage these pointers.
	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a2);
	policyAlphaVector->set(s1, 100.0);
	policyAlphaVector->set(s2, 0.0);
	alphaVectors.push_back(policyAlphaVector);

	PolicyAlphaVectors *policyAlphaVectors = new PolicyAlphaVectors(alphaVectors);

	if (policyAlphaVectors->get(b) == a2) {
		b->set(s1, 0.15);
		b->set(s2, 0.85);

		if (policyAlphaVectors->get(b) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVectors::set'... ";

	delete policyAlphaVectors;
	policyAlphaVectors = new PolicyAlphaVectors(2);

	std::vector<std::vector<PolicyAlphaVector *> > setOfAlphaVectors;

	alphaVectors.clear();

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a1);
	policyAlphaVector->set(s1, 10.0);
	policyAlphaVector->set(s2, -10.0);
	alphaVectors.push_back(policyAlphaVector);

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a2);
	policyAlphaVector->set(s1, -10.0);
	policyAlphaVector->set(s2, 10.0);
	alphaVectors.push_back(policyAlphaVector);

	policyAlphaVectors->set(0, alphaVectors);

	setOfAlphaVectors.push_back(alphaVectors);

	std::vector<PolicyAlphaVector *> alphaVectors2;

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a1);
	policyAlphaVector->set(s1, -10.0);
	policyAlphaVector->set(s2, 10.0);
	alphaVectors2.push_back(policyAlphaVector);

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a2);
	policyAlphaVector->set(s1, 10.0);
	policyAlphaVector->set(s2, -10.0);
	alphaVectors2.push_back(policyAlphaVector);

	policyAlphaVectors->set(1, alphaVectors2);

	b->set(s1, 0.1);
	b->set(s2, 0.9);

	if (policyAlphaVectors->get(0, b) == a2 && policyAlphaVectors->get(1, b) == a1) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVectors::prune_dominated'... ";

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a1);
	policyAlphaVector->set(s1, -20.0);
	policyAlphaVector->set(s2, -10.0);
	alphaVectors.push_back(policyAlphaVector);

	policyAlphaVector = new PolicyAlphaVector();
	policyAlphaVector->set_action(a2);
	policyAlphaVector->set(s1, 0.0);
	policyAlphaVector->set(s2, 0.0);
	alphaVectors.push_back(policyAlphaVector);

	PolicyAlphaVectors::prune_dominated(states, alphaVectors);

	if (alphaVectors.size() == 3 &&
			((PolicyAlphaVector *)alphaVectors[0])->get_action() == a1 &&
			((PolicyAlphaVector *)alphaVectors[0])->get(s1) == 10.0 && ((PolicyAlphaVector *)alphaVectors[0])->get(s2) == -10.0 &&
			((PolicyAlphaVector *)alphaVectors[1])->get_action() == a2 &&
			((PolicyAlphaVector *)alphaVectors[1])->get(s1) == -10.0 && ((PolicyAlphaVector *)alphaVectors[1])->get(s2) == 10.0 &&
			((PolicyAlphaVector *)alphaVectors[2])->get_action() == a2 &&
			((PolicyAlphaVector *)alphaVectors[2])->get(s1) == 0.0 && ((PolicyAlphaVector *)alphaVectors[2])->get(s2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	// This last alpha vector was not dominated. Therefore, we need to free the memory, since the other one was and will
	// already be freed inside of 'prune_dominated'. Since this is not managed inside a PolicyAlphaVectors object, we
	// need to free the memory of this last one we made.
	delete policyAlphaVector;
	policyAlphaVector = nullptr;

	std::cout << "Policy: Test 'PolicyAlphaVectors::save'... ";

	if (!policyAlphaVectors->save("tmp/test_05.policy_alpha_vectors", states)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyAlphaVectors::load'... ";

	if (!policyAlphaVectors->load("tmp/test_05.policy_alpha_vectors", states, actions, observations, horizon)) {
		if (policyAlphaVectors->get(0, b) == a2 && policyAlphaVectors->get(1, b) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} else {
		std::cout << " Failure." << std::endl;
	}

	// Note: The state, action, and policy alpha vector variables are freed inside the deconstructors of states, actions,
	// and policy alpha vectors classes, respectively.
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

	delete policyAlphaVectors;
	policyAlphaVectors = nullptr;

	delete b;
	b = nullptr;

	return numSuccesses;
}
