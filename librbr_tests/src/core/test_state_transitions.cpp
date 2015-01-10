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

#include "../../../librbr/include/core/state_transitions/state_transitions_map.h"
#include "../../../librbr/include/core/state_transitions/state_transition_exception.h"

#include "../../../librbr/include/core/states/named_state.h"
#include "../../../librbr/include/core/actions/named_action.h"

#include "../../../librbr/include/core/states/states_map.h"

int test_state_transitions()
{
	int numSuccesses = 0;

	State *s1 = new NamedState("s1");
	State *s2 = new NamedState("s2");

	Action *a1 = new NamedAction("a1");
	Action *a2 = new NamedAction("a2");

	StateTransitionsMap *finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::set' and ";
	std::cout << "'FiniteStateTransitions::get' (All Values Mapped)... ";

	finiteStateTransitions->set(s1, a1, s1, 0.1);
	finiteStateTransitions->set(s1, a1, s2, 0.2);
	finiteStateTransitions->set(s1, a2, s1, 0.3);
	finiteStateTransitions->set(s1, a2, s2, 0.4);
	finiteStateTransitions->set(s2, a1, s1, 0.5);
	finiteStateTransitions->set(s2, a1, s2, 0.6);
	finiteStateTransitions->set(s2, a2, s1, 0.7);
	finiteStateTransitions->set(s2, a2, s2, 0.8);

	if (finiteStateTransitions->get(s1, a1, s1) == 0.1 && finiteStateTransitions->get(s1, a1, s2) == 0.2 &&
			finiteStateTransitions->get(s1, a2, s1) == 0.3 && finiteStateTransitions->get(s1, a2, s2) == 0.4 &&
			finiteStateTransitions->get(s2, a1, s1) == 0.5 && finiteStateTransitions->get(s2, a1, s2) == 0.6 &&
			finiteStateTransitions->get(s2, a2, s1) == 0.7 && finiteStateTransitions->get(s2, a2, s2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteStateTransitions;
	finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::set' and ";
	std::cout << "'FiniteStateTransitions::get' (Default Zero Values)... ";

//	finiteStateTransitions->set(s1, a1, s1, 0.1); // Leave undefined to test the default zero value for the next state s1.
	finiteStateTransitions->set(s1, a1, s2, 0.2);
//	finiteStateTransitions->set(s1, a2, s1, 0.3);
//	finiteStateTransitions->set(s1, a2, s2, 0.4); // Leave undefined to test the default zero value for the action a2.
//	finiteStateTransitions->set(s2, a1, s1, 0.5); // Leave undefined to test the default zero value for the observation o2.
//	finiteStateTransitions->set(s2, a1, s2, 0.6); // Leave undefined to test the default zero value for the observation o2.
//	finiteStateTransitions->set(s2, a2, s1, 0.7);	// Leave undefined to test the default zero value for the observation o2.
//	finiteStateTransitions->set(s2, a2, s2, 0.8);	// Leave undefined to test the default zero value for the observation o2.

	if (finiteStateTransitions->get(s1, a1, s1) == 0.0 && finiteStateTransitions->get(s1, a1, s2) == 0.2 &&
			finiteStateTransitions->get(s2, a1, s1) == 0.0 && finiteStateTransitions->get(s2, a1, s2) == 0.0 &&
			finiteStateTransitions->get(s1, a2, s1) == 0.0 && finiteStateTransitions->get(s1, a2, s2) == 0.0 &&
			finiteStateTransitions->get(s2, a2, s1) == 0.0 && finiteStateTransitions->get(s2, a2, s2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteStateTransitions;
	finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::set' and ";
	std::cout << "'FiniteStateTransitions::get' (Wildcard Next State)... ";

	finiteStateTransitions->set(s1, a1, s1, 0.1);
	finiteStateTransitions->set(s1, a1, s2, 0.2);
	finiteStateTransitions->set(s1, a2, s1, 0.3);
	finiteStateTransitions->set(s1, a2, s2, 0.4);
	finiteStateTransitions->set(s2, a1, s1, 0.5);
	finiteStateTransitions->set(s2, a1, s2, 0.6);
//	finiteStateTransitions->set(s2, a2, s1, 0.7); // Leave undefined to test wildcard next state.
//	finiteStateTransitions->set(s2, a2, s2, 0.8); // Leave undefined to test wildcard next state.
	finiteStateTransitions->set(s2, a2, nullptr, 0.9);

	if (finiteStateTransitions->get(s1, a1, s1) == 0.1 && finiteStateTransitions->get(s1, a1, s2) == 0.2 &&
			finiteStateTransitions->get(s1, a2, s1) == 0.3 && finiteStateTransitions->get(s1, a2, s2) == 0.4 &&
			finiteStateTransitions->get(s2, a1, s1) == 0.5 && finiteStateTransitions->get(s2, a1, s2) == 0.6 &&
			finiteStateTransitions->get(s2, a2, s1) == 0.9 && finiteStateTransitions->get(s2, a2, s2) == 0.9) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteStateTransitions;
	finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::set' and ";
	std::cout << "'FiniteStateTransitions::get' (Wildcard Action)... ";

	finiteStateTransitions->set(s1, a1, s1, 0.1);
	finiteStateTransitions->set(s1, a1, s2, 0.2);
	finiteStateTransitions->set(s1, a2, s1, 0.3);
	finiteStateTransitions->set(s1, a2, s2, 0.4);
//	finiteStateTransitions->set(s2, a1, s1, 0.5); // Leave undefined to test wildcard action.
	finiteStateTransitions->set(s2, a1, s2, 0.6);
//	finiteStateTransitions->set(s2, a2, s1, 0.7); // Leave undefined to test wildcard action.
	finiteStateTransitions->set(s2, a2, s2, 0.8);
	finiteStateTransitions->set(s2, nullptr, s1, 0.9);

	if (finiteStateTransitions->get(s1, a1, s1) == 0.1 && finiteStateTransitions->get(s1, a1, s2) == 0.2 &&
			finiteStateTransitions->get(s1, a2, s1) == 0.3 && finiteStateTransitions->get(s1, a2, s2) == 0.4 &&
			finiteStateTransitions->get(s2, a1, s1) == 0.9 && finiteStateTransitions->get(s2, a1, s2) == 0.6 &&
			finiteStateTransitions->get(s2, a2, s1) == 0.9 && finiteStateTransitions->get(s2, a2, s2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteStateTransitions;
	finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::set' and ";
	std::cout << "'FiniteStateTransitions::get' (Wildcard Previous State)... ";

	finiteStateTransitions->set(s1, a1, s1, 0.1);
	finiteStateTransitions->set(s1, a1, s2, 0.2);
//	finiteStateTransitions->set(s1, a2, s1, 0.3); // Leave undefined to test wildcard previous state.
	finiteStateTransitions->set(s1, a2, s2, 0.4);
	finiteStateTransitions->set(s2, a1, s1, 0.5);
	finiteStateTransitions->set(s2, a1, s2, 0.6);
//	finiteStateTransitions->set(s2, a2, s1, 0.7); // Leave undefined to test wildcard previous state.
	finiteStateTransitions->set(s2, a2, s2, 0.8);
	finiteStateTransitions->set(nullptr, a2, s1, 0.9);

	if (finiteStateTransitions->get(s1, a1, s1) == 0.1 && finiteStateTransitions->get(s1, a1, s2) == 0.2 &&
			finiteStateTransitions->get(s1, a2, s1) == 0.9 && finiteStateTransitions->get(s1, a2, s2) == 0.4 &&
			finiteStateTransitions->get(s2, a1, s1) == 0.5 && finiteStateTransitions->get(s2, a1, s2) == 0.6 &&
			finiteStateTransitions->get(s2, a2, s1) == 0.9 && finiteStateTransitions->get(s2, a2, s2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteStateTransitions;
	finiteStateTransitions = new StateTransitionsMap();

	std::cout << "StateTransitions: Test 'FiniteStateTransitions::add_successor' and 'FiniteStateTransitions::successors'... ";

	finiteStateTransitions->set(s1, a1, s1, 0.0);
	finiteStateTransitions->set(s1, a1, s2, 0.0);
	finiteStateTransitions->set(s1, a2, s1, 1.0);
	finiteStateTransitions->set(s1, a2, s2, 0.0);
	finiteStateTransitions->set(s2, a1, s1, 0.0);
	finiteStateTransitions->set(s2, a1, s2, 1.0);
	finiteStateTransitions->set(s2, a2, s1, 1.0);
	finiteStateTransitions->set(s2, a2, s2, 1.0);

	StatesMap *S = new StatesMap();
	S->add(s1);
	S->add(s2);

	finiteStateTransitions->add_successor(s1, a2, s1);
	finiteStateTransitions->add_successor(s2, a1, s2);
	finiteStateTransitions->add_successor(s2, a2, s1);
	finiteStateTransitions->add_successor(s2, a2, s2);

	try {
		const std::vector<State *> &tmp1 = finiteStateTransitions->successors(S, s1, a1);
		std::cout << " Failure." << std::endl;
	} catch (StateTransitionException &err) {
		std::cout << "1"; std::cout.flush();
		const std::vector<State *> &tmp2 = finiteStateTransitions->successors(S, s1, a2);
		if (tmp2.size() == 1 && tmp2[0] == s1) {
			std::cout << "2"; std::cout.flush();
			const std::vector<State *> &tmp3 = finiteStateTransitions->successors(S, s2, a1);
			if (tmp3.size() == 1 && tmp3[0] == s2) {
				std::cout << "3"; std::cout.flush();
				const std::vector<State *> &tmp4 = finiteStateTransitions->successors(S, s2, a2);
				if (tmp4.size() == 2) {
					std::cout << " Success." << std::endl;
					numSuccesses++;
				} else {
					std::cout << " Failure." << std::endl;
				}
			} else {
				std::cout << " Failure." << std::endl;
			}
		} else {
			std::cout << " Failure." << std::endl;
		}
	}

	delete finiteStateTransitions;
	finiteStateTransitions = nullptr;

	delete S;

//	delete s1; // Taken care of by S.
//	delete s2; // Taken care of by S.
	delete a1;
	delete a2;

	return numSuccesses;
}
