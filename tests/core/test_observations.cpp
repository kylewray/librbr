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

#include "../../include/core/observations/observation.h"
#include "../../include/core/observations/finite_observations.h"
#include "../../include/core/observations/joint_observation.h"
#include "../../include/core/observations/finite_joint_observations.h"
#include "../../include/core/observations/observation_exception.h"

/**
 * Test the observation objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_observations()
{
	int numSuccesses = 0;

	Observation *o1 = new Observation("o1");
	Observation *o2 = new Observation("o2");
	Observation *o3 = new Observation("o3");

	FiniteObservations *finiteObservations = new FiniteObservations();

	std::cout << "Observations: Test 'FiniteObservations::add' and 'FiniteObservations::get'... ";

	finiteObservations->add(o1);
	finiteObservations->add(o2);
	finiteObservations->add(o3);

	try {
		if (finiteObservations->get_num_observations() == 3 && finiteObservations->get(0) == o1 &&
				finiteObservations->get(1) == o2 && finiteObservations->get(2) == o3) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::remove'... ";
	try {
		finiteObservations->remove(o2);
		o2 = new Observation("o2");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::remove' (Check Result)... ";
	if (finiteObservations->get_num_observations() == 2 && finiteObservations->get(0) == o1 &&
			finiteObservations->get(1) == o3) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::add' and 'FiniteObservations::remove'... ";
	try {
		finiteObservations->remove(o1);
		o1 = new Observation("o1");

		finiteObservations->add(o2);
		finiteObservations->add(o1);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::add', 'FiniteObservations::remove' (Check Result), and 'FiniteObservations::all'... ";
	if (finiteObservations->get_num_observations() == 3 && finiteObservations->get(0) == o3 &&
			finiteObservations->get(1) == o2 && finiteObservations->get(2) == o1 &&
			finiteObservations->all().size() == finiteObservations->get_num_observations()) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::remove' (Expecting Error)... ";
	Observation *doesNotExist = new Observation("doesNotExist");

	try {
		finiteObservations->remove(doesNotExist);
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete doesNotExist;
	doesNotExist = nullptr;

	std::cout << "Observations: Test 'FiniteObservations::set'... ";

	Observation *o1New = new Observation("o1");
	Observation *o2New = new Observation("o2");

	std::vector<const Observation *> testObservationsList;
	testObservationsList.push_back(o1New);
	testObservationsList.push_back(o2New);
	finiteObservations->set(testObservationsList);

	o1 = o1New;
	o2 = o2New;
	o3 = new Observation("o3");

	if (finiteObservations->get_num_observations() == 2 && finiteObservations->get(0) == o1 &&
			finiteObservations->get(1) == o2) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	const Observation *testFindObservation = nullptr;

	std::cout << "Observations: Test 'FiniteObservations::find'... ";
	try {
		testFindObservation = finiteObservations->find(o1->get_name());
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::find' (Expecting Error)... ";
	try {
		testFindObservation = finiteObservations->find(o3->get_name());
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteObservations;
	finiteObservations = nullptr;

	o1 = new Observation("o1");
	o2 = new Observation("o2");
	o3 = new Observation("o3");
	Observation *o4 = new Observation("o4");

	FiniteJointObservations *finiteJointObservations = new FiniteJointObservations(2);

	std::cout << "Observations: Test 'FiniteJointObservations::add'... ";
	try {
		finiteJointObservations->add(0, o1);
		finiteJointObservations->add(0, o2);
		finiteJointObservations->add(1, o3);
		finiteJointObservations->add(1, o4);
		finiteJointObservations->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	JointObservation *o13 = nullptr;
	JointObservation *o14 = nullptr;
	JointObservation *o23 = nullptr;
	JointObservation *o24 = nullptr;

	std::cout << "Observations: Test 'FiniteJointObservations::get'... ";
	try {
		o13 = (JointObservation *)finiteJointObservations->get(0);
		o14 = (JointObservation *)finiteJointObservations->get(1);
		o23 = (JointObservation *)finiteJointObservations->get(2);
		o24 = (JointObservation *)finiteJointObservations->get(3);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::add' (Check Result)... ";
	try {
		if (finiteJointObservations->get_num_observations() == 4 &&
				finiteJointObservations->get(0, 0) == o1 &&
				finiteJointObservations->get(0, 1) == o2 &&
				finiteJointObservations->get(1, 0) == o3 &&
				finiteJointObservations->get(1, 1) == o4 &&
				o13->get(0) == o1 &&
				o13->get(1) == o3 &&
				o14->get(0) == o1 &&
				o14->get(1) == o4 &&
				o23->get(0) == o2 &&
				o23->get(1) == o3 &&
				o24->get(0) == o2 &&
				o24->get(1) == o4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::remove'... ";
	try {
		finiteJointObservations->remove(0, o2);
		o2 = new Observation("o2");

		finiteJointObservations->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::remove' (Check Result)... ";
	try {
		if (finiteJointObservations->get_num_observations() == 2 &&
				finiteJointObservations->get(0, 0) == o1 &&
				finiteJointObservations->get(1, 0) == o3 &&
				finiteJointObservations->get(1, 1) == o4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::add' and 'FiniteJointObservations::remove'... ";
	try {
		finiteJointObservations->remove(1, o3);
		o3 = new Observation("o3");

		finiteJointObservations->add(0, o3);
		finiteJointObservations->add(0, o2);
		finiteJointObservations->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::add' and 'FiniteJointObservations::remove' (Check Result)... ";
	try {
		if (finiteJointObservations->get_num_observations() == 3 &&
				finiteJointObservations->get(0, 0) == o1 &&
				finiteJointObservations->get(0, 1) == o3 &&
				finiteJointObservations->get(0, 2) == o2 &&
				finiteJointObservations->get(1, 0) == o4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteJointObservations::remove' (Expecting Error)... ";
	Observation *a7 = new Observation("a7");
	Observation *a8 = new Observation("a8");
	JointObservation *doesNotExist2 = new JointObservation({a7, a8});
	try {
		finiteJointObservations->remove(0, doesNotExist2);
		finiteJointObservations->update();
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete a7;
	a7 = nullptr;

	delete a8;
	a8 = nullptr;

	delete doesNotExist2;
	doesNotExist2 = nullptr;

	std::cout << "Observations: Test 'FiniteJointObservations::set'... ";
	o1New = new Observation("o1");
	o2New = new Observation("o2");

	testObservationsList.clear();
	testObservationsList.push_back(o2New);
	testObservationsList.push_back(o1New);

	try {
		finiteJointObservations->set(0, testObservationsList);
		finiteJointObservations->update();
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	o1 = o1New;
	o2 = o2New;
	o3 = new Observation("o3");

	std::cout << "Observations: Test 'FiniteJointObservations::set' (Check Result)... ";
	try {
		if (finiteJointObservations->get_num_observations() == 2 &&
				finiteJointObservations->get(0, 0) == o2 &&
				finiteJointObservations->get(0, 1) == o1 &&
				finiteJointObservations->get(1, 0) == o4) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	testFindObservation = nullptr;

	std::cout << "Observations: Test 'FiniteJointObservations::find'... ";
	try {
		testFindObservation = finiteJointObservations->find("o1 o4");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'finiteJointObservations::find' (Expecting Error)... ";
	try {
		testFindObservation = finiteJointObservations->find("o3 o2");
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteJointObservations;
	finiteJointObservations = nullptr;

	return numSuccesses;
}
