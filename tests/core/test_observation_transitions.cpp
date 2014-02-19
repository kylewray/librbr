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

#include "../../include/core/observation_transitions/finite_observation_transitions.h"

/**
 * Test the observation transition objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_observation_transitions()
{
	int numSuccesses = 0;

	Observation *o1 = new Observation("o1");
	Observation *o2 = new Observation("o2");

	State *s1 = new State("s1");
	State *s2 = new State("s2");

	Action *a1 = new Action("a1");
	Action *a2 = new Action("a2");

	FiniteObservationTransitions *finiteObservationTransitions = new FiniteObservationTransitions();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (All Values Mapped)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
	finiteObservationTransitions->set(a2, s1, o2, 0.7);
	finiteObservationTransitions->set(a2, s2, o2, 0.8);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.7 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new FiniteObservationTransitions();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Default Zero Values)... ";

//	finiteObservationTransitions->set(a1, s1, o1, 0.1); // Leave undefined to test the default zero value for the next state s1.
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
//	finiteObservationTransitions->set(a2, s1, o1, 0.3);
//	finiteObservationTransitions->set(a2, s2, o1, 0.4); // Leave undefined to test the default zero value for the action a2.
//	finiteObservationTransitions->set(a1, s1, o2, 0.5); // Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a1, s2, o2, 0.6); // Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a2, s1, o2, 0.7);	// Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a2, s2, o2, 0.8);	// Leave undefined to test the default zero value for the observation o2.

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.0 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.0 && finiteObservationTransitions->get(a1, s2, o2) == 0.0 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.0 && finiteObservationTransitions->get(a2, s2, o1) == 0.0 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.0 && finiteObservationTransitions->get(a2, s2, o2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new FiniteObservationTransitions();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Next State)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard next state.
//	finiteObservationTransitions->set(a2, s2, o2, 0.8); // Leave undefined to test wildcard next state.
	finiteObservationTransitions->set(a2, nullptr, o2, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.9) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new FiniteObservationTransitions();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Action)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
//	finiteObservationTransitions->set(a1, s1, o2, 0.5); // Leave undefined to test wildcard action.
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard action.
	finiteObservationTransitions->set(a2, s2, o2, 0.8);
	finiteObservationTransitions->set(nullptr, s1, o2, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.9 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new FiniteObservationTransitions();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Previous State)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
//	finiteObservationTransitions->set(a2, s1, o1, 0.3); // Leave undefined to test wildcard previous state.
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard previous state.
	finiteObservationTransitions->set(a2, s2, o2, 0.8);
	finiteObservationTransitions->set(a2, s1, nullptr, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.9 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;

	return numSuccesses;
}


#endif // PERFORM_TESTS_H
