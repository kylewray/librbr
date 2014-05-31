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

#include "../../../librbr/include/core/actions/named_action.h"
#include "../../../librbr/include/core/actions/finite_actions.h"
#include "../../../librbr/include/core/actions/joint_action.h"
#include "../../../librbr/include/core/actions/finite_joint_actions.h"
#include "../../../librbr/include/core/actions/action_exception.h"
#include "../../../librbr/include/core/actions/action_utilities.h"

int test_actions()
{
	int numSuccesses = 0;

	Action *a1 = new NamedAction("a1");
	Action *a2 = new NamedAction("a2");
	Action *a3 = new NamedAction("a3");

	FiniteActions *finiteActions = new FiniteActions();

	std::cout << "Actions: Test 'FiniteActions::add' and 'FiniteActions::get'... ";

	finiteActions->add(a1);
	finiteActions->add(a2);
	finiteActions->add(a3);

	bool a1Found = false;
	bool a2Found = false;
	bool a3Found = false;

	try {
		for (auto a : *finiteActions) {
			const Action *action = resolve(a);
			if (action == a1) {
				a1Found = true;
			} else if (action == a2) {
				a2Found = true;
			} else if (action == a3) {
				a3Found = true;
			}
		}

		if (finiteActions->get_num_actions() == 3 && a1Found && a2Found && a3Found) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::remove'... ";
	try {
		finiteActions->remove(a2);
		a2 = new NamedAction("a2");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::remove' (Check Result)... ";

	a1Found = false;
	a2Found = false;
	a3Found = false;

	for (auto a : *finiteActions) {
		const Action *action = resolve(a);
		if (action == a1) {
			a1Found = true;
		} else if (action == a2) {
			a2Found = true;
		} else if (action == a3) {
			a3Found = true;
		}
	}

	if (finiteActions->get_num_actions() == 2 && a1Found && !a2Found && a3Found) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::add' and 'FiniteActions::remove'... ";
	try {
		finiteActions->remove(a1);
		a1 = new NamedAction("a1");

		finiteActions->add(a2);
		finiteActions->add(a1);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::add' and 'FiniteActions::remove' (Check Result)... ";

	a1Found = false;
	a2Found = false;
	a3Found = false;

	for (auto a : *finiteActions) {
		const Action *action = resolve(a);
		if (action == a1) {
			a1Found = true;
		} else if (action == a2) {
			a2Found = true;
		} else if (action == a3) {
			a3Found = true;
		}
	}

	if (finiteActions->get_num_actions() == 3 && a1Found && a2Found && a3Found) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::remove' (Expecting Error)... ";
	NamedAction *doesNotExist = new NamedAction("doesNotExist");

	try {
		finiteActions->remove(doesNotExist);
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete doesNotExist;
	doesNotExist = nullptr;

	std::cout << "Actions: Test 'FiniteActions::set'... ";

	Action *a1New = new NamedAction("a1");
	Action *a2New = new NamedAction("a2");

	std::vector<const Action *> testActionsList;
	testActionsList.push_back(a1New);
	testActionsList.push_back(a2New);
	finiteActions->set(testActionsList);

	a1 = a1New;
	a2 = a2New;
	a3 = new NamedAction("a3");

	a1Found = false;
	a2Found = false;
	a3Found = false;

	for (auto a : *finiteActions) {
		const Action *action = resolve(a);
		if (action == a1) {
			a1Found = true;
		} else if (action == a2) {
			a2Found = true;
		} else if (action == a3) {
			a3Found = true;
		}
	}

	if (finiteActions->get_num_actions() == 2 && a1Found && a2Found && !a3Found) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	const Action *testFindAction = nullptr;

	std::cout << "Actions: Test 'FiniteActions::find'... ";
	try {
		testFindAction = find_action(finiteActions, ((const NamedAction *)a1)->get_name());
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteActions::find' (Expecting Error)... ";
	try {
		testFindAction = find_action(finiteActions, ((const NamedAction *)a3)->get_name());
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteActions;
	finiteActions = nullptr;

	a1 = new NamedAction("a1");
	a2 = new NamedAction("a2");
	a3 = new NamedAction("a3");
	Action *a4 = new NamedAction("a4");

	FiniteJointActions *finiteJointActions = new FiniteJointActions(2);

	std::cout << "Actions: Test 'FiniteJointActions::add'... ";
	try {
		finiteJointActions->add(0, a1);
		finiteJointActions->add(0, a2);
		finiteJointActions->add(1, a3);
		finiteJointActions->add(1, a4);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::get'... ";

	bool a13Found = false;
	bool a14Found = false;
	bool a23Found = false;
	bool a24Found = false;

	try {
		for (auto a : *finiteJointActions) {
			const Action *action = resolve(a);
			const JointAction *ja = static_cast<const JointAction *>(action);
			if (ja == nullptr) {
				throw ActionException();
			}

			if (ja->get(0) == a1 && ja->get(1) == a3) {
				a13Found = true;
			} else if (ja->get(0) == a1 && ja->get(1) == a4) {
				a14Found = true;
			} else if (ja->get(0) == a2 && ja->get(1) == a3) {
				a23Found = true;
			} else if (ja->get(0) == a2 && ja->get(1) == a4) {
				a24Found = true;
			}
		}

		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 4 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(0, 1) == a2 &&
				finiteJointActions->get(1, 0) == a3 &&
				finiteJointActions->get(1, 1) == a4 &&
				a13Found && a14Found && a23Found && a24Found) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove'... ";
	try {
		finiteJointActions->remove(0, a2);
		a2 = new NamedAction("a2");

		finiteJointActions->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 2 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(1, 0) == a3 &&
				finiteJointActions->get(1, 1) == a4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' and 'FiniteJointActions::remove'... ";
	try {
		finiteJointActions->remove(1, a3);
		a3 = new NamedAction("a3");

		finiteJointActions->add(0, a3);
		finiteJointActions->add(0, a2);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::add' and 'FiniteJointActions::remove' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 3 &&
				finiteJointActions->get(0, 0) == a1 &&
				finiteJointActions->get(0, 1) == a3 &&
				finiteJointActions->get(0, 2) == a2 &&
				finiteJointActions->get(1, 0) == a4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'FiniteJointActions::remove' (Expecting Error)... ";
	Action *a7 = new NamedAction("a7");
	Action *a8 = new NamedAction("a8");
	JointAction *doesNotExist2 = new JointAction({a7, a8});
	try {
		finiteJointActions->remove(0, doesNotExist2);
		finiteJointActions->update();
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}
	delete a7;
	a7 = nullptr;

	delete a8;
	a8 = nullptr;

	delete doesNotExist2;
	doesNotExist2 = nullptr;

	std::cout << "Actions: Test 'FiniteJointActions::set'... ";
	a1New = new NamedAction("a1");
	a2New = new NamedAction("a2");

	testActionsList.clear();
	testActionsList.push_back(a2New);
	testActionsList.push_back(a1New);

	try {
		finiteJointActions->set(0, testActionsList);
		finiteJointActions->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	a1 = a1New;
	a2 = a2New;
	a3 = new NamedAction("a3");

	std::cout << "Actions: Test 'FiniteJointActions::set' (Check Result)... ";
	try {
		if (finiteJointActions->get_num_actions() == 2 &&
				finiteJointActions->get(0, 0) == a2 &&
				finiteJointActions->get(0, 1) == a1 &&
				finiteJointActions->get(1, 0) == a4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	testFindAction = nullptr;

	std::cout << "Actions: Test 'FiniteJointActions::find'... ";
	try {
		testFindAction = find_action(finiteJointActions, "a1 a4");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ActionException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Actions: Test 'finiteJointActions::find' (Expecting Error)... ";
	try {
		testFindAction = find_action(finiteJointActions, "a3 a2");
		std::cout << " Failure." << std::endl;
	} catch (const ActionException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteJointActions;
	finiteJointActions = nullptr;

	return numSuccesses;
}
