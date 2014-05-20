/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#include "../../include/perform_tests.h"

#include <iostream>

#include "../../../librbr/include/core/states/named_state.h"
#include "../../../librbr/include/core/states/finite_states.h"
#include "../../../librbr/include/core/states/factored_state.h"
#include "../../../librbr/include/core/states/finite_factored_states.h"
#include "../../../librbr/include/core/states/state_exception.h"
#include "../../../librbr/include/core/states/state_utilities.h"

/**
 * Test the states objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_states()
{
	int numSuccesses = 0;

	State *s1 = new NamedState("s1");
	State *s2 = new NamedState("s2");
	State *s3 = new NamedState("s3");

	FiniteStates *finiteStates = new FiniteStates();

	std::cout << "States: Test 'FiniteStates::add' and 'FiniteStates::get'... ";

	finiteStates->add(s1);
	finiteStates->add(s2);
	finiteStates->add(s3);

	try {
		if (finiteStates->get_num_states() == 3 && finiteStates->get(0) == s1 &&
				finiteStates->get(1) == s2 && finiteStates->get(2) == s3) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::remove'... ";
	try {
		finiteStates->remove(s2);
		s2 = new NamedState("s2");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::remove' (Check Result)... ";
	if (finiteStates->get_num_states() == 2 && finiteStates->get(0) == s1 &&
			finiteStates->get(1) == s3) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::add' and 'FiniteStates::remove'... ";
	try {
		finiteStates->remove(s1);
		s1 = new NamedState("s1");

		finiteStates->add(s2);
		finiteStates->add(s1);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::add', 'FiniteStates::remove' (Check Result), and 'FiniteStates::all'... ";
	if (finiteStates->get_num_states() == 3 && finiteStates->get(0) == s3 &&
			finiteStates->get(1) == s2 && finiteStates->get(2) == s1) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::remove' (Expecting Error)... ";
	State *doesNotExist = new NamedState("doesNotExist");
	try {
		finiteStates->remove(doesNotExist);
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}
	delete doesNotExist;

	std::cout << "States: Test 'FiniteStates::set'... ";

	State *s1New = new NamedState("s1");
	State *s2New = new NamedState("s2");

	std::vector<const State *> testStatesList;
	testStatesList.push_back(s1New);
	testStatesList.push_back(s2New);
	finiteStates->set(testStatesList);

	s1 = s1New;
	s2 = s2New;
	s3 = new NamedState("s3");

	if (finiteStates->get_num_states() == 2 && finiteStates->get(0) == s1 &&
			finiteStates->get(1) == s2) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	const State *testFindState = nullptr;

	std::cout << "States: Test 'FiniteStates::find'... ";
	try {
		testFindState = find_state(finiteStates, s1->to_string());
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteStates::find' (Expecting Error)... ";
	try {
		testFindState = find_state(finiteStates, s3->to_string());
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteStates;

	s1 = new NamedState("s1");
	s2 = new NamedState("s2");
	s3 = new NamedState("s3");
	State *s4 = new NamedState("s4");

	FiniteFactoredStates *finiteFactoredStates = new FiniteFactoredStates(2);

	std::cout << "States: Test 'FiniteFactoredStates::add'... ";
	try {
		finiteFactoredStates->add(0, s1);
		finiteFactoredStates->add(0, s2);
		finiteFactoredStates->add(1, s3);
		finiteFactoredStates->add(1, s4);
		finiteFactoredStates->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	FactoredState *s13 = nullptr;
	FactoredState *s14 = nullptr;
	FactoredState *s23 = nullptr;
	FactoredState *s24 = nullptr;

	std::cout << "States: Test 'FiniteFactoredStates::get'... ";
	try {
		s13 = (FactoredState *)finiteFactoredStates->get(0);
		s14 = (FactoredState *)finiteFactoredStates->get(1);
		s23 = (FactoredState *)finiteFactoredStates->get(2);
		s24 = (FactoredState *)finiteFactoredStates->get(3);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::add' (Check Result)... ";
	try {
		if (finiteFactoredStates->get_num_states() == 4 &&
				finiteFactoredStates->get(0, 0) == s1 &&
				finiteFactoredStates->get(0, 1) == s2 &&
				finiteFactoredStates->get(1, 0) == s3 &&
				finiteFactoredStates->get(1, 1) == s4 &&
				s13->get(0) == s1 &&
				s13->get(1) == s3 &&
				s14->get(0) == s1 &&
				s14->get(1) == s4 &&
				s23->get(0) == s2 &&
				s23->get(1) == s3 &&
				s24->get(0) == s2 &&
				s24->get(1) == s4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::remove'... ";
	try {
		finiteFactoredStates->remove(0, s2);
		s2 = new NamedState("s2");

		finiteFactoredStates->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::remove' (Check Result)... ";
	try {
		if (finiteFactoredStates->get_num_states() == 2 &&
				finiteFactoredStates->get(0, 0) == s1 &&
				finiteFactoredStates->get(1, 0) == s3 &&
				finiteFactoredStates->get(1, 1) == s4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::add' and 'FiniteFactoredStates::remove'... ";
	try {
		finiteFactoredStates->remove(1, s3);
		s3 = new NamedState("s3");

		finiteFactoredStates->add(0, s3);
		finiteFactoredStates->add(0, s2);
		finiteFactoredStates->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::add' and 'FiniteFactoredStates::remove' (Check Result)... ";
	try {
		if (finiteFactoredStates->get_num_states() == 3 &&
				finiteFactoredStates->get(0, 0) == s1 &&
				finiteFactoredStates->get(0, 1) == s3 &&
				finiteFactoredStates->get(0, 2) == s2 &&
				finiteFactoredStates->get(1, 0) == s4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'FiniteFactoredStates::remove' (Expecting Error)... ";
	State *a7 = new NamedState("a7");
	State *a8 = new NamedState("a8");
	FactoredState *doesNotExist2 = new FactoredState({a7, a8});
	try {
		finiteFactoredStates->remove(0, doesNotExist2);
		finiteFactoredStates->update();
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}
	delete a7;
	delete a8;
	delete doesNotExist2;

	std::cout << "States: Test 'FiniteFactoredStates::set'... ";
	s1New = new NamedState("s1");
	s2New = new NamedState("s2");

	testStatesList.clear();
	testStatesList.push_back(s2New);
	testStatesList.push_back(s1New);

	try {
		finiteFactoredStates->set(0, testStatesList);
		finiteFactoredStates->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	s1 = s1New;
	s2 = s2New;
	s3 = new NamedState("s3");

	std::cout << "States: Test 'FiniteFactoredStates::set' (Check Result)... ";
	try {
		if (finiteFactoredStates->get_num_states() == 2 &&
				finiteFactoredStates->get(0, 0) == s2 &&
				finiteFactoredStates->get(0, 1) == s1 &&
				finiteFactoredStates->get(1, 0) == s4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	testFindState = nullptr;

	std::cout << "States: Test 'FiniteFactoredStates::find'... ";
	try {
		testFindState = find_state(finiteFactoredStates, "s1 s4");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const StateException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "States: Test 'finiteFactoredStates::find' (Expecting Error)... ";
	try {
		testFindState = find_state(finiteFactoredStates, "s3 s2");
		std::cout << " Failure." << std::endl;
	} catch (const StateException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteFactoredStates;

	return numSuccesses;
}
