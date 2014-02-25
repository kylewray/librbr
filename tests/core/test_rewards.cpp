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

#include "../../include/core/rewards/sas_rewards.h"

/**
 * Test the reward objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_rewards()
{
	int numSuccesses = 0;

	State *s1 = new State("s1");
	State *s2 = new State("s2");

	Action *a1 = new Action("a1");
	Action *a2 = new Action("a2");

	SASRewards *rewards = new SASRewards();

	std::cout << "Rewards: Test 'SASRewards::set' and 'SASRewards::get' (All Values Mapped)... ";

	rewards->set(s1, a1, s1, 3.0);
	rewards->set(s1, a1, s2, -2.9);
	rewards->set(s1, a2, s1, 2.5);
	rewards->set(s1, a2, s2, 1.9);
	rewards->set(s2, a1, s1, 2.2);
	rewards->set(s2, a1, s2, -6.0);
	rewards->set(s2, a2, s1, 4.0);
	rewards->set(s2, a2, s2, 9.4);

	if (rewards->get(s1, a1, s1) == 3.0 && rewards->get(s1, a1, s2) == -2.9 &&
			rewards->get(s1, a2, s1) == 2.5 && rewards->get(s1, a2, s2) == 1.9 &&
			rewards->get(s2, a1, s1) == 2.2 && rewards->get(s2, a1, s2) == -6.0 &&
			rewards->get(s2, a2, s1) == 4.0 && rewards->get(s2, a2, s2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete rewards;
	rewards = new SASRewards();

	std::cout << "Rewards: Test 'SASRewards::set' and 'SASRewards::get' (Default Zero Values)... ";

//	rewards->set(s1, a1, s1, 3.0); // Leave undefined to test the default zero value for the next state s1.
	rewards->set(s1, a1, s2, -2.9);
//	rewards->set(s1, a2, s1, 2.5);
//	rewards->set(s1, a2, s2, 1.9); // Leave undefined to test the default zero value for the action a2.
//	rewards->set(s2, a1, s1, 2.2);  // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a1, s2, -6.0); // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s1, 4.0);	// Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s2, 9.4);	// Leave undefined to test the default zero value for the previous state s2.

	if (rewards->get(s1, a1, s1) == 0.0 && rewards->get(s1, a1, s2) == -2.9 &&
			rewards->get(s1, a2, s1) == 0.0 && rewards->get(s1, a2, s2) == 0.0 &&
			rewards->get(s2, a1, s1) == 0.0 && rewards->get(s2, a1, s2) == 0.0 &&
			rewards->get(s2, a2, s1) == 0.0 && rewards->get(s2, a2, s2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete rewards;
	rewards = new SASRewards();

	std::cout << "Rewards: Test 'SASRewards::set' and 'SASRewards::get' (Wildcard Next State)... ";

	rewards->set(s1, a1, s1, 3.0);
	rewards->set(s1, a1, s2, -2.9);
	rewards->set(s1, a2, s1, 2.5);
	rewards->set(s1, a2, s2, 1.9);
	rewards->set(s2, a1, s1, 2.2);
	rewards->set(s2, a1, s2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard next state.
//	rewards->set(s2, a2, s2, 9.4); // Leave undefined to test wildcard next state.
	rewards->set(s2, a2, nullptr, -42.0);

	if (rewards->get(s1, a1, s1) == 3.0 && rewards->get(s1, a1, s2) == -2.9 &&
			rewards->get(s1, a2, s1) == 2.5 && rewards->get(s1, a2, s2) == 1.9 &&
			rewards->get(s2, a1, s1) == 2.2 && rewards->get(s2, a1, s2) == -6.0 &&
			rewards->get(s2, a2, s1) == -42.0 && rewards->get(s2, a2, s2) == -42.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete rewards;
	rewards = new SASRewards();

	std::cout << "Rewards: Test 'SASRewards::set' and 'SASRewards::get' (Wildcard Action)... ";

	rewards->set(s1, a1, s1, 3.0);
	rewards->set(s1, a1, s2, -2.9);
	rewards->set(s1, a2, s1, 2.5);
	rewards->set(s1, a2, s2, 1.9);
//	rewards->set(s2, a1, s1, 2.2); // Leave undefined to test wildcard action.
	rewards->set(s2, a1, s2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard action.
	rewards->set(s2, a2, s2, 9.4);
	rewards->set(s2, nullptr, s1, -42.0);
//	rewards->set(s2, nullptr, s2, -121.0);

	if (rewards->get(s1, a1, s1) == 3.0 && rewards->get(s1, a1, s2) == -2.9 &&
			rewards->get(s1, a2, s1) == 2.5 && rewards->get(s1, a2, s2) == 1.9 &&
			rewards->get(s2, a1, s1) == -42.0 && rewards->get(s2, a1, s2) == -6.0 &&
			rewards->get(s2, a2, s1) == -42.0 && rewards->get(s2, a2, s2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete rewards;
	rewards = new SASRewards();

	std::cout << "Rewards: Test 'SASRewards::set' and 'SASRewards::get' (Wildcard Previous State)... ";

	rewards->set(s1, a1, s1, 3.0);
	rewards->set(s1, a1, s2, -2.9);
//	rewards->set(s1, a2, s1, 2.5); // Leave undefined to test wildcard previous state.
	rewards->set(s1, a2, s2, 1.9);
	rewards->set(s2, a1, s1, 2.2);
	rewards->set(s2, a1, s2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard previous state.
	rewards->set(s2, a2, s2, 9.4);
	rewards->set(nullptr, a2, s1, -42.0);

	if (rewards->get(s1, a1, s1) == 3.0 && rewards->get(s1, a1, s2) == -2.9 &&
			rewards->get(s1, a2, s1) == -42.0 && rewards->get(s1, a2, s2) == 1.9 &&
			rewards->get(s2, a1, s1) == 2.2 && rewards->get(s2, a1, s2) == -6.0 &&
			rewards->get(s2, a2, s1) == -42.0 && rewards->get(s2, a2, s2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete rewards;
	rewards = nullptr;

	return numSuccesses;
}


#endif // PERFORM_TESTS_H
