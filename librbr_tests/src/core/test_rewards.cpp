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

#include "../../../librbr/include/core/rewards/sa_rewards_map.h"
#include "../../../librbr/include/core/rewards/sas_rewards_map.h"
#include "../../../librbr/include/core/rewards/saso_rewards_map.h"
#include "../../../librbr/include/core/rewards/factored_rewards.h"

#include "../../../librbr/include/core/states/named_state.h"
#include "../../../librbr/include/core/actions/named_action.h"
#include "../../../librbr/include/core/observations/named_observation.h"

int test_rewards()
{
	int numSuccesses = 0;

	State *s1 = new NamedState("s1");
	State *s2 = new NamedState("s2");

	Action *a1 = new NamedAction("a1");
	Action *a2 = new NamedAction("a2");

	NamedObservation *o1 = new NamedObservation("o1");
	NamedObservation *o2 = new NamedObservation("o2");

	// ------------------------------------------------------------------------------------------------------------

	SARewardsMap *saRewardsMap = new SARewardsMap();

	std::cout << "Rewards: Test 'SARewardsMap::set' and 'SARewardsMap::get' (All Values Mapped)... ";

	saRewardsMap->set(s1, a1, 3.0);
	saRewardsMap->set(s1, a2, 2.5);
	saRewardsMap->set(s2, a1, s2, 2.2);
	saRewardsMap->set(s2, a2, s1, o2, 4.0);

	if (saRewardsMap->get(s1, a1, s1) == 3.0 && saRewardsMap->get(s1, a2) == 2.5 &&
			saRewardsMap->get(s2, a1, s1, o1) == 2.2 && saRewardsMap->get(s2, a2, s2) == 4.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete saRewardsMap;
	saRewardsMap = new SARewardsMap();

	std::cout << "Rewards: Test 'SARewardsMap::set' and 'SARewardsMap::get' (Default Zero Values)... ";

//	rewards->set(s1, a1, s1, 3.0); // Leave undefined to test the default zero value for the next state s1.
	saRewardsMap->set(s1, a2, -2.9);
//	rewards->set(s1, a2, s1, 2.5);
//	rewards->set(s1, a2, s2, 1.9); // Leave undefined to test the default zero value for the action a2.
//	rewards->set(s2, a1, s1, 2.2);  // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a1, s2, -6.0); // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s1, 4.0);	// Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s2, 9.4);	// Leave undefined to test the default zero value for the previous state s2.

	if (saRewardsMap->get(s1, a1, s1) == 0.0 && saRewardsMap->get(s1, a2) == -2.9 &&
			saRewardsMap->get(s2, a1) == 0.0 && saRewardsMap->get(s2, a2, s2, o1) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete saRewardsMap;
	saRewardsMap = new SARewardsMap();

	std::cout << "Rewards: Test 'SARewardsMap::set' and 'SARewardsMap::get' (Wildcard Action)... ";

	saRewardsMap->set(s1, a1, 3.0);
	saRewardsMap->set(s1, a2, s1, 2.5);
//	rewards->set(s2, a1, s1, 2.2); // Leave undefined to test wildcard action.
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard action.
	saRewardsMap->set(s2, nullptr, s2, -42.0);
//	rewards->set(s2, nullptr, s2, -121.0);

	if (saRewardsMap->get(s1, a1) == 3.0 && saRewardsMap->get(s1, a2, s2, o1) == 2.5 &&
			saRewardsMap->get(s2, a1) == -42.0 && saRewardsMap->get(s2, a2, s2) == -42.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete saRewardsMap;
	saRewardsMap = new SARewardsMap();

	std::cout << "Rewards: Test 'SARewardsMap::set' and 'SARewardsMap::get' (Wildcard Previous State)... ";

	saRewardsMap->set(s1, a1, s1, 3.0);
//	rewards->set(s1, a2, s1, 2.5); // Leave undefined to test wildcard previous state.
	saRewardsMap->set(s2, a1, 2.2);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard previous state.
	saRewardsMap->set(nullptr, a2, s1, -42.0);

	if (saRewardsMap->get(s1, a1) == 3.0 && saRewardsMap->get(s1, a2, s2) == -42.0 &&
			saRewardsMap->get(s2, a1, s1, o1) == 2.2 && saRewardsMap->get(s2, a2) == -42.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete saRewardsMap;
	saRewardsMap = nullptr;

	// ------------------------------------------------------------------------------------------------------------

	SASRewardsMap *sasRewardsMap = new SASRewardsMap();

	std::cout << "Rewards: Test 'SASRewardsMap::set' and 'SASRewardsMap::get' (All Values Mapped)... ";

	sasRewardsMap->set(s1, a1, s1, 3.0);
	sasRewardsMap->set(s1, a1, s2, -2.9);
	sasRewardsMap->set(s1, a2, s1, o2, 2.5);
	sasRewardsMap->set(s1, a2, s2, 1.9);
	sasRewardsMap->set(s2, a1, s1, o1, 2.2);
	sasRewardsMap->set(s2, a1, s2, o2, -6.0);
	sasRewardsMap->set(s2, a2, s1, 4.0);
	sasRewardsMap->set(s2, a2, s2, 9.4);

	if (sasRewardsMap->get(s1, a1, s1) == 3.0 && sasRewardsMap->get(s1, a1, s2, o2) == -2.9 &&
			sasRewardsMap->get(s1, a2, s1) == 2.5 && sasRewardsMap->get(s1, a2, s2) == 1.9 &&
			sasRewardsMap->get(s2, a1, s1, o1) == 2.2 && sasRewardsMap->get(s2, a1, s2) == -6.0 &&
			sasRewardsMap->get(s2, a2, s1) == 4.0 && sasRewardsMap->get(s2, a2, s2, o2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasRewardsMap;
	sasRewardsMap = new SASRewardsMap();

	std::cout << "Rewards: Test 'SASRewardsMap::set' and 'SASRewardsMap::get' (Default Zero Values)... ";

//	rewards->set(s1, a1, s1, 3.0); // Leave undefined to test the default zero value for the next state s1.
	sasRewardsMap->set(s1, a1, s2, -2.9);
//	rewards->set(s1, a2, s1, 2.5);
//	rewards->set(s1, a2, s2, 1.9); // Leave undefined to test the default zero value for the action a2.
//	rewards->set(s2, a1, s1, 2.2);  // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a1, s2, -6.0); // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s1, 4.0);	// Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s2, 9.4);	// Leave undefined to test the default zero value for the previous state s2.

	if (sasRewardsMap->get(s1, a1, s1, o2) == 0.0 && sasRewardsMap->get(s1, a1, s2, o2) == -2.9 &&
			sasRewardsMap->get(s1, a2, s1) == 0.0 && sasRewardsMap->get(s1, a2, s2) == 0.0 &&
			sasRewardsMap->get(s2, a1, s1, o1) == 0.0 && sasRewardsMap->get(s2, a1, s2) == 0.0 &&
			sasRewardsMap->get(s2, a2, s1) == 0.0 && sasRewardsMap->get(s2, a2, s2, o1) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasRewardsMap;
	sasRewardsMap = new SASRewardsMap();

	std::cout << "Rewards: Test 'SASRewardsMap::set' and 'SASRewardsMap::get' (Wildcard Next State)... ";

	sasRewardsMap->set(s1, a1, s1, 3.0);
	sasRewardsMap->set(s1, a1, s2, o1, -2.9);
	sasRewardsMap->set(s1, a2, s1, 2.5);
	sasRewardsMap->set(s1, a2, s2, 1.9);
	sasRewardsMap->set(s2, a1, s1, o2, 2.2);
	sasRewardsMap->set(s2, a1, s2, o2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard next state.
//	rewards->set(s2, a2, s2, 9.4); // Leave undefined to test wildcard next state.
	sasRewardsMap->set(s2, a2, nullptr, -42.0);

	if (sasRewardsMap->get(s1, a1, s1) == 3.0 && sasRewardsMap->get(s1, a1, s2) == -2.9 &&
			sasRewardsMap->get(s1, a2, s1) == 2.5 && sasRewardsMap->get(s1, a2, s2) == 1.9 &&
			sasRewardsMap->get(s2, a1, s1) == 2.2 && sasRewardsMap->get(s2, a1, s2) == -6.0 &&
			sasRewardsMap->get(s2, a2, s1) == -42.0 && sasRewardsMap->get(s2, a2, s2) == -42.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasRewardsMap;
	sasRewardsMap = new SASRewardsMap();

	std::cout << "Rewards: Test 'SASRewardsMap::set' and 'SASRewardsMap::get' (Wildcard Action)... ";

	sasRewardsMap->set(s1, a1, s1, o1, 3.0);
	sasRewardsMap->set(s1, a1, s2, -2.9);
	sasRewardsMap->set(s1, a2, s1, 2.5);
	sasRewardsMap->set(s1, a2, s2, o2, 1.9);
//	rewards->set(s2, a1, s1, 2.2); // Leave undefined to test wildcard action.
	sasRewardsMap->set(s2, a1, s2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard action.
	sasRewardsMap->set(s2, a2, s2, 9.4);
	sasRewardsMap->set(s2, nullptr, s1, o1, -42.0);
//	rewards->set(s2, nullptr, s2, -121.0);

	if (sasRewardsMap->get(s1, a1, s1) == 3.0 && sasRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasRewardsMap->get(s1, a2, s1) == 2.5 && sasRewardsMap->get(s1, a2, s2) == 1.9 &&
			sasRewardsMap->get(s2, a1, s1, o2) == -42.0 && sasRewardsMap->get(s2, a1, s2) == -6.0 &&
			sasRewardsMap->get(s2, a2, s1) == -42.0 && sasRewardsMap->get(s2, a2, s2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasRewardsMap;
	sasRewardsMap = new SASRewardsMap();

	std::cout << "Rewards: Test 'SASRewardsMap::set' and 'SASRewardsMap::get' (Wildcard Previous State)... ";

	sasRewardsMap->set(s1, a1, s1, 3.0);
	sasRewardsMap->set(s1, a1, s2, -2.9);
//	rewards->set(s1, a2, s1, 2.5); // Leave undefined to test wildcard previous state.
	sasRewardsMap->set(s1, a2, s2, o1, 1.9);
	sasRewardsMap->set(s2, a1, s1, 2.2);
	sasRewardsMap->set(s2, a1, s2, o2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard previous state.
	sasRewardsMap->set(s2, a2, s2, 9.4);
	sasRewardsMap->set(nullptr, a2, s1, o1, -42.0);

	if (sasRewardsMap->get(s1, a1, s1) == 3.0 && sasRewardsMap->get(s1, a1, s2) == -2.9 &&
			sasRewardsMap->get(s1, a2, s1, o1) == -42.0 && sasRewardsMap->get(s1, a2, s2, o1) == 1.9 &&
			sasRewardsMap->get(s2, a1, s1) == 2.2 && sasRewardsMap->get(s2, a1, s2) == -6.0 &&
			sasRewardsMap->get(s2, a2, s1, o2) == -42.0 && sasRewardsMap->get(s2, a2, s2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasRewardsMap;
	sasRewardsMap = nullptr;

	// ------------------------------------------------------------------------------------------------------------

	SASORewardsMap *sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (All Values Mapped)... ";

	sasoRewardsMap->set(s1, a1, s1, o1, 3.0);
	sasoRewardsMap->set(s1, a1, s2, o1, -2.9);
	sasoRewardsMap->set(s1, a2, s1, o1, 2.5);
	sasoRewardsMap->set(s1, a2, s2, o2, 1.9);
	sasoRewardsMap->set(s2, a1, s1, o2, 2.2);
	sasoRewardsMap->set(s2, a1, s2, o2, -6.0);
	sasoRewardsMap->set(s2, a2, s1, o1, 4.0);
	sasoRewardsMap->set(s2, a2, s2, o1, 9.4);

	if (sasoRewardsMap->get(s1, a1, s1, o1) == 3.0 && sasoRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o1) == 2.5 && sasoRewardsMap->get(s1, a2, s2, o2) == 1.9 &&
			sasoRewardsMap->get(s2, a1, s1, o2) == 2.2 && sasoRewardsMap->get(s2, a1, s2, o2) == -6.0 &&
			sasoRewardsMap->get(s2, a2, s1, o1) == 4.0 && sasoRewardsMap->get(s2, a2, s2, o1) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (Default Zero Values)... ";

//	rewards->set(s1, a1, s1, 3.0); // Leave undefined to test the default zero value for the next state s1.
	sasoRewardsMap->set(s1, a1, s2, o1, -2.9);
//	rewards->set(s1, a2, s1, 2.5);
//	rewards->set(s1, a2, s2, 1.9); // Leave undefined to test the default zero value for the action a2.
//	rewards->set(s2, a1, s1, 2.2);  // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a1, s2, -6.0); // Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s1, 4.0);	// Leave undefined to test the default zero value for the previous state s2.
//	rewards->set(s2, a2, s2, 9.4);	// Leave undefined to test the default zero value for the previous state s2.

	if (sasoRewardsMap->get(s1, a1, s1, o2) == 0.0 && sasoRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o1) == 0.0 && sasoRewardsMap->get(s1, a2, s2, o1) == 0.0 &&
			sasoRewardsMap->get(s2, a1, s1, o2) == 0.0 && sasoRewardsMap->get(s2, a1, s2, o2) == 0.0 &&
			sasoRewardsMap->get(s2, a2, s1, o1) == 0.0 && sasoRewardsMap->get(s2, a2, s2, o1) == 0.0 &&
			sasoRewardsMap->get(s1, a1, s2, o2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (Wildcard Next State)... ";

	sasoRewardsMap->set(s1, a1, s1, o1, 3.0);
	sasoRewardsMap->set(s1, a1, s2, o2, -2.9);
	sasoRewardsMap->set(s1, a2, s1, o1, 2.5);
	sasoRewardsMap->set(s1, a2, s2, o2, 1.9);
	sasoRewardsMap->set(s2, a1, s1, o1, 2.2);
	sasoRewardsMap->set(s2, a1, s2, o2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard next state.
//	rewards->set(s2, a2, s2, 9.4); // Leave undefined to test wildcard next state.
	sasoRewardsMap->set(s2, a2, nullptr, o1, -42.0);

	if (sasoRewardsMap->get(s1, a1, s1, o1) == 3.0 && sasoRewardsMap->get(s1, a1, s2, o2) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o1) == 2.5 && sasoRewardsMap->get(s1, a2, s2, o2) == 1.9 &&
			sasoRewardsMap->get(s2, a1, s1, o1) == 2.2 && sasoRewardsMap->get(s2, a1, s2, o2) == -6.0 &&
			sasoRewardsMap->get(s2, a2, s1, o1) == -42.0 && sasoRewardsMap->get(s2, a2, s2, o2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (Wildcard Action)... ";

	sasoRewardsMap->set(s1, a1, s1, o2, 3.0);
	sasoRewardsMap->set(s1, a1, s2, o1, -2.9);
	sasoRewardsMap->set(s1, a2, s1, o2, 2.5);
	sasoRewardsMap->set(s1, a2, s2, o1, 1.9);
//	rewards->set(s2, a1, s1, 2.2); // Leave undefined to test wildcard action.
	sasoRewardsMap->set(s2, a1, s2, o1, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard action.
	sasoRewardsMap->set(s2, a2, s2, o1, 9.4);
	sasoRewardsMap->set(s2, nullptr, s1, o2, -42.0);
//	rewards->set(s2, nullptr, s2, -121.0);

	if (sasoRewardsMap->get(s1, a1, s1, o2) == 3.0 && sasoRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o2) == 2.5 && sasoRewardsMap->get(s1, a2, s2, o1) == 1.9 &&
			sasoRewardsMap->get(s2, a1, s1, o2) == -42.0 && sasoRewardsMap->get(s2, a1, s2, o1) == -6.0 &&
			sasoRewardsMap->get(s2, a2, s1, o2) == -42.0 && sasoRewardsMap->get(s2, a2, s2, o1) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (Wildcard Previous State)... ";

	sasoRewardsMap->set(s1, a1, s1, o1, 3.0);
	sasoRewardsMap->set(s1, a1, s2, o1, -2.9);
//	rewards->set(s1, a2, s1, 2.5); // Leave undefined to test wildcard previous state.
	sasoRewardsMap->set(s1, a2, s2, o2, 1.9);
	sasoRewardsMap->set(s2, a1, s1, o2, 2.2);
	sasoRewardsMap->set(s2, a1, s2, o2, -6.0);
//	rewards->set(s2, a2, s1, 4.0); // Leave undefined to test wildcard previous state.
	sasoRewardsMap->set(s2, a2, s2, o1, 9.4);
	sasoRewardsMap->set(nullptr, a2, s1, o1, -42.0);

	if (sasoRewardsMap->get(s1, a1, s1, o1) == 3.0 && sasoRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o1) == -42.0 && sasoRewardsMap->get(s1, a2, s2, o2) == 1.9 &&
			sasoRewardsMap->get(s2, a1, s1, o2) == 2.2 && sasoRewardsMap->get(s2, a1, s2, o2) == -6.0 &&
			sasoRewardsMap->get(s2, a2, s1, o1) == -42.0 && sasoRewardsMap->get(s2, a2, s2, o1) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = new SASORewardsMap();

	std::cout << "Rewards: Test 'SASORewardsMap::set' and 'SASORewardsMap::get' (Wildcard Observation)... ";

	sasoRewardsMap->set(s1, a1, s1, o1, 3.0);
	sasoRewardsMap->set(s1, a1, s2, o1, -2.9);
	sasoRewardsMap->set(s1, a2, s1, o1, 2.5);
	sasoRewardsMap->set(s1, a2, s2, o2, 1.9);
	sasoRewardsMap->set(s2, a1, s1, o2, 2.2);
	sasoRewardsMap->set(s2, a1, s2, o2, -6.0);
	sasoRewardsMap->set(s2, a2, s1, o1, 4.0);
	sasoRewardsMap->set(s2, a2, s2, nullptr, 9.4); // Leave undefined to test wildcard previous state.

	if (sasoRewardsMap->get(s1, a1, s1, o1) == 3.0 && sasoRewardsMap->get(s1, a1, s2, o1) == -2.9 &&
			sasoRewardsMap->get(s1, a2, s1, o1) == 2.5 && sasoRewardsMap->get(s1, a2, s2, o2) == 1.9 &&
			sasoRewardsMap->get(s2, a1, s1, o2) == 2.2 && sasoRewardsMap->get(s2, a1, s2, o2) == -6.0 &&
			sasoRewardsMap->get(s2, a2, s1, o1) == 4.0 && sasoRewardsMap->get(s2, a2, s2, o1) == 9.4 &&
			sasoRewardsMap->get(s2, a2, s2, o2) == 9.4) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete sasoRewardsMap;
	sasoRewardsMap = nullptr;

	// ------------------------------------------------------------------------------------------------------------

	std::cout << "Rewards: Test 'FactoredRewards::add_factor' and 'FactoredRewards::get'... ";

	FactoredRewards *factoredRewards = new FactoredRewards();

	saRewardsMap = new SARewardsMap();
	saRewardsMap->set(s1, a1, 42.0);
	saRewardsMap->set(s1, a2, 10.0);
	saRewardsMap->set(s2, a1, -1.0);
	saRewardsMap->set(s2, a2, -100.0);

	factoredRewards->add_factor(saRewardsMap);

	saRewardsMap = new SARewardsMap();
	saRewardsMap->set(s1, a1, -1.0);
	saRewardsMap->set(s1, a2, 0.0);
	saRewardsMap->set(s2, a1, 1.0);
	saRewardsMap->set(s2, a2, 2.0);

	factoredRewards->add_factor(saRewardsMap);

	/*
	std::cout << factoredRewards->get_num_rewards() << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(0)))->get(s1, a1) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(0)))->get(s1, a2) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(0)))->get(s2, a1) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(0)))->get(s2, a2) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(1)))->get(s1, a1) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(1)))->get(s1, a2) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(1)))->get(s2, a1) << std::endl;
	std::cout << ((SARewards *)(factoredRewards->get(1)))->get(s2, a2) << std::endl;
	 */

	const SARewards *R0 = dynamic_cast<const SARewards *>(factoredRewards->get(0));
	const SARewards *R1 = dynamic_cast<const SARewards *>(factoredRewards->get(1));

	if (R0 != nullptr && R1 != nullptr && factoredRewards->get_num_rewards() == 2 &&
			R0->get(s1, a1) == 42.0 &&
			R0->get(s1, a2) == 10.0 &&
			R0->get(s2, a1) == -1.0 &&
			R0->get(s2, a2) == -100.0 &&
			R1->get(s1, a1) == -1.0 &&
			R1->get(s1, a2) == 0.0 &&
			R1->get(s2, a1) == 1.0 &&
			R1->get(s2, a2) == 2.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Rewards: Test 'FactoredRewards::set'... ";

	saRewardsMap = new SARewardsMap();
	saRewardsMap->set(s1, a1, -1.0);
	saRewardsMap->set(s1, a2, 0.0);
	saRewardsMap->set(s2, a1, 1.0);
	saRewardsMap->set(s2, a2, 2.0);

	factoredRewards->set(0, saRewardsMap);

	saRewardsMap = new SARewardsMap();
	saRewardsMap->set(s1, a1, 42.0);
	saRewardsMap->set(s1, a2, 10.0);
	saRewardsMap->set(s2, a1, -1.0);
	saRewardsMap->set(s2, a2, -100.0);

	factoredRewards->set(1, saRewardsMap);

	R0 = dynamic_cast<const SARewards *>(factoredRewards->get(0));
	R1 = dynamic_cast<const SARewards *>(factoredRewards->get(1));

	if (R0 != nullptr && R1 != nullptr && factoredRewards->get_num_rewards() == 2 &&
		R0->get(s1, a1) == -1.0 &&
		R0->get(s1, a2) == 0.0 &&
		R0->get(s2, a1) == 1.0 &&
		R0->get(s2, a2) == 2.0 &&
		R1->get(s1, a1) == 42.0 &&
		R1->get(s1, a2) == 10.0 &&
		R1->get(s2, a1) == -1.0 &&
		R1->get(s2, a2) == -100.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete factoredRewards;

	// ------------------------------------------------------------------------------------------------------------

	delete s1;
	s1 = nullptr;
	delete s2;
	s2 = nullptr;
	delete a1;
	a1 = nullptr;
	delete a2;
	a2 = nullptr;
	delete o1;
	o1 = nullptr;
	delete o2;
	o2 = nullptr;

	return numSuccesses;
}
