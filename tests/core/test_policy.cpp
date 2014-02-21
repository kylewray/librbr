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
#include "../../include/core/policy/policy_fsc.h"
#include "../../include/core/policy/policy_exception.h"

#include "../../include/core/states/finite_states.h"
#include "../../include/core/actions/finite_actions.h"
#include "../../include/core/horizon.h"

/**
 * Test the policy objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_policy()
{
	int numSuccesses = 0;

	State *s1 = new State("s1");
	State *s2 = new State("s2");

	Action *a1 = new Action("a1");
	Action *a2 = new Action("a2");

	FiniteStates *states = new FiniteStates({s1, s2});
	FiniteActions *actions = new FiniteActions({a1, a2});
	Horizon *horizon = new Horizon((unsigned int)3);

	PolicyMap *policy = new PolicyMap();

	std::cout << "Policy: 'PolicyMap::load' file 'test_01.map_policy'... ";
	if (!policy->load("tests/resources/policy/test_01.map_policy", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::get' (Check Result)... ";
	try {
		if (policy->get(s1) == a2 && policy->get(s2) == a1) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::set'... ";

	policy->set(s1, a1);
	policy->set(s2, a2);

	try {
		if (policy->get(s1) == a1 && policy->get(s2) == a2) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const PolicyException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: 'test_02.map_policy' (Expecting Error)...\n\t";
	if (policy->load("tests/resources/policy/test_02.map_policy", states, actions, horizon)) {
		std::cout << "\tSuccess." << std::endl;
		numSuccesses++;
	} else {
		std::cout << "\tFailure." << std::endl;
	}

	std::cout << "Policy: 'PolicyMap::load' file 'test_03.map_policy'... ";
	if (!policy->load("tests/resources/policy/test_03.map_policy", states, actions, horizon)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Policy: Test 'PolicyMap::get' (Check Result)... ";
	try {
		if (policy->get(0, s1) == a2 && policy->get(0, s2) == a1 &&
				policy->get(1, s1) == a1 && policy->get(1, s2) == a2 &&
				policy->get(2, s1) == a1 && policy->get(2, s2) == a1) {
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
	delete actions;
	delete horizon;
	delete policy;

	return numSuccesses;
}


#endif // PERFORM_TESTS_H
