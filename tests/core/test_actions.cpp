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

#include "../../include/core/actions/action.h"
#include "../../include/core/actions/finite_actions.h"
#include "../../include/core/actions/joint_action.h"
#include "../../include/core/actions/finite_joint_actions.h"
#include "../../include/core/actions/action_exception.h"

/**
 * Test the actions objects. Output the success or failure for each test.
 * @return The number of errors encountered during execution.
 */
int test_actions()
{
	int numErrors = 0;

	Action *a1 = new Action("a1");
	Action *a2 = new Action("a2");
	Action *a3 = new Action("a3");

	FiniteActions *finiteActions = new FiniteActions();

	std::cout << "Actions: Test 'FiniteActions::add' and 'FiniteActions::get'... ";

	finiteActions->add(a1);
	finiteActions->add(a2);
	finiteActions->add(a3);

	try {
		if (finiteActions->get_num_actions() == 3 && finiteActions->get(0) == a1 &&
				finiteActions->get(1) == a2 && finiteActions->get(2) == a3) {
			std::cout << " Success." << std::endl;
		} else {
			std::cout << " Failure." << std::endl;
			numErrors++;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::remove'... ";
	try {
		finiteActions->remove(a2);
		a2 = new Action("a2");
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::remove' (Check Result)... ";
	if (finiteActions->get_num_actions() == 2 && finiteActions->get(0) == a1 &&
			finiteActions->get(1) == a3) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::add' and 'FiniteActions::remove'... ";
	try {
		finiteActions->remove(a1);
		a1 = new Action("a1");

		finiteActions->add(a2);
		finiteActions->add(a1);
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::add', 'FiniteActions::remove' (Check Result), and 'FiniteActions::all'... ";
	if (finiteActions->get_num_actions() == 3 && finiteActions->get(0) == a3 &&
			finiteActions->get(1) == a2 && finiteActions->get(2) == a1 &&
			finiteActions->all().size() == finiteActions->get_num_actions()) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::remove' (Expecting Error)... ";
	Action *doesNotExist = new Action("doesNotExist");
	try {
		finiteActions->remove(doesNotExist);
		std::cout << " Failure." << std::endl;
		numErrors++;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
	}
	delete doesNotExist;

	std::cout << "Actions: Test 'FiniteActions::set'... ";

	Action *a1New = new Action("a1");
	Action *a2New = new Action("a2");

	std::vector<Action *> testActionsList;
	testActionsList.push_back(a1New);
	testActionsList.push_back(a2New);
	finiteActions->set(testActionsList);

	a1 = a1New;
	a2 = a2New;
	a3 = new Action("a3");

	if (finiteActions->get_num_actions() == 2 && finiteActions->get(0) == a1 &&
			finiteActions->get(1) == a2) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	Action *testFindAction = nullptr;

	std::cout << "Actions: Test 'FiniteActions::find'... ";
	try {
		testFindAction = finiteActions->find(a1->get_name());
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteActions::find' (Expecting Error)... ";
	try {
		testFindAction = finiteActions->find(a3->get_name());
		std::cout << " Failure." << std::endl;
		numErrors++;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
	}

	Action *a4 = new Action("a4");

	FiniteJointActions *finiteJointActions = new FiniteJointActions(2);

	std::cout << "Actions: Test 'FiniteJointActions::add'... ";
	try {
		finiteJointActions->add(0, a1);
		finiteJointActions->add(0, a2);
		finiteJointActions->add(1, a3);
		finiteJointActions->add(1, a4);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	JointAction *a13 = nullptr;
	JointAction *a14 = nullptr;
	JointAction *a23 = nullptr;
	JointAction *a24 = nullptr;

	std::cout << "Actions: Test 'FiniteJointActions::get'... ";
	try {
		a13 = (JointAction *)finiteJointActions->get(0);
		a14 = (JointAction *)finiteJointActions->get(1);
		a23 = (JointAction *)finiteJointActions->get(2);
		a24 = (JointAction *)finiteJointActions->get(3);
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 4 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(0, 1) == a2 &&
				finiteJointActions->get(1, 0) == a3 &&
				finiteJointActions->get(1, 1) == a4 &&
				a13->get(0) == a1 &&
				a13->get(1) == a3 &&
				a14->get(0) == a1 &&
				a14->get(1) == a4 &&
				a23->get(0) == a2 &&
				a23->get(1) == a3 &&
				a24->get(0) == a2 &&
				a24->get(1) == a4) {
			std::cout << " Success." << std::endl;
		} else {
			std::cout << " Failure." << std::endl;
			numErrors++;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove'... ";
	try {
		finiteJointActions->remove(0, a2);
		a2 = new Action("a2");

		finiteJointActions->update();
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 2 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(1, 0) == a3 &&
				finiteJointActions->get(1, 1) == a4) {
			std::cout << " Success." << std::endl;
		} else {
			std::cout << " Failure." << std::endl;
			numErrors++;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' and 'FiniteJointActions::remove'... ";
	try {
		finiteJointActions->remove(1, a3);
		a3 = new Action("a3");

		finiteJointActions->add(0, a3);
		finiteJointActions->add(0, a2);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' and 'FiniteJointActions::remove' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 3 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(0, 1) == a3 &&
				finiteJointActions->get(0, 2) == a2 &&
				finiteJointActions->get(1, 0) == a4) {
			std::cout << " Success." << std::endl;
		} else {
			std::cout << " Failure." << std::endl;
			numErrors++;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove' (Expecting Error)... ";
	Action *a7 = new Action("a7");
	Action *a8 = new Action("a8");
	JointAction *doesNotExist2 = new JointAction({a7, a8});
	try {
		finiteJointActions->remove(0, doesNotExist2);
		finiteJointActions->update();
		std::cout << " Failure." << std::endl;
		numErrors++;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
	}
	delete a7;
	delete a8;
	delete doesNotExist2;

	std::cout << "Actions: Test 'FiniteJointActions::set'... ";
	a1New = new Action("a1");
	a2New = new Action("a2");

	testActionsList.clear();
	testActionsList.push_back(a2New);
	testActionsList.push_back(a1New);

	try {
		finiteJointActions->set(0, testActionsList);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	a1 = a1New;
	a2 = a2New;
	a3 = new Action("a3");

	std::cout << "Actions: Test 'FiniteJointActions::set' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 2 &&
				finiteJointActions->get(0, 0) == a2 &&
				finiteJointActions->get(0, 1) == a1 &&
				finiteJointActions->get(1, 0) == a4) {
			std::cout << " Success." << std::endl;
		} else {
			std::cout << " Failure." << std::endl;
			numErrors++;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	testFindAction = nullptr;

	std::cout << "Actions: Test 'FiniteJointActions::find'... ";
	try {
		testFindAction = finiteJointActions->find("a1 a4");
		std::cout << " Success." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "Actions: Test 'finiteJointActions::find' (Expecting Error)... ";
	try {
		testFindAction = finiteJointActions->find("a3 a2");
		std::cout << " Failure." << std::endl;
		numErrors++;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
	}

	return numErrors;
}


#endif // PERFORM_TESTS_H
