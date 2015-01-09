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

#include "../../../librbr/include/core/observations/named_observation.h"
#include "../../../librbr/include/core/observations/observations_map.h"
#include "../../../librbr/include/core/observations/joint_observation.h"
#include "../../../librbr/include/core/observations/joint_observations_map.h"
#include "../../../librbr/include/core/observations/observation_exception.h"
#include "../../../librbr/include/core/observations/observation_utilities.h"

int test_observations()
{
	int numSuccesses = 0;

	NamedObservation *o1 = new NamedObservation("o1");
	NamedObservation *o2 = new NamedObservation("o2");
	NamedObservation *o3 = new NamedObservation("o3");

	ObservationsMap *finiteObservations = new ObservationsMap();

	std::cout << "Observations: Test 'FiniteObservations::add' and 'FiniteObservations::get'... ";

	finiteObservations->add(o1);
	finiteObservations->add(o2);
	finiteObservations->add(o3);

	bool o1Found = false;
	bool o2Found = false;
	bool o3Found = false;

	try {
		for (auto o : *finiteObservations) {
			Observation *observation = resolve(o);
			if (observation == o1) {
				o1Found = true;
			} else if (observation == o2) {
				o2Found = true;
			} else if (observation == o3) {
				o3Found = true;
			}
		}

		if (finiteObservations->get_num_observations() == 3 && o1Found && o2Found && o3Found &&
				finiteObservations->exists(o1) && finiteObservations->exists(o2) && finiteObservations->exists(o3)) {
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
		o2 = new NamedObservation("o2");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::remove' (Check Result)... ";

	o1Found = false;
	o2Found = false;
	o3Found = false;

	for (auto o : *finiteObservations) {
		Observation *observation = resolve(o);
		if (observation == o1) {
			o1Found = true;
		} else if (observation == o2) {
			o2Found = true;
		} else if (observation == o3) {
			o3Found = true;
		}
	}

	if (finiteObservations->get_num_observations() == 2 && o1Found && !o2Found && o3Found &&
			finiteObservations->exists(o1) && !finiteObservations->exists(o2) && finiteObservations->exists(o3)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::get'... ";
	if (finiteObservations->get(o1->hash_value()) == o1 &&
			finiteObservations->get(o3->hash_value()) == o3) {
		try {
			finiteObservations->get(o2->hash_value());
			std::cout << " Failure." << std::endl;
        } catch (const ObservationException &err) {
            std::cout << " Success." << std::endl;
            numSuccesses++;
        }
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::add' and 'FiniteObservations::remove'... ";
	try {
		finiteObservations->remove(o1);
		o1 = new NamedObservation("o1");

		finiteObservations->add(o2);
		finiteObservations->add(o1);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::add' and 'FiniteObservations::remove' (Check Result)... ";

	o1Found = false;
	o2Found = false;
	o3Found = false;

	for (auto o : *finiteObservations) {
		Observation *observation = resolve(o);
		if (observation == o1) {
			o1Found = true;
		} else if (observation == o2) {
			o2Found = true;
		} else if (observation == o3) {
			o3Found = true;
		}
	}

	if (finiteObservations->get_num_observations() == 3 && o1Found && o2Found && o3Found &&
			finiteObservations->exists(o1) && finiteObservations->exists(o2) && finiteObservations->exists(o3)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::remove' (Expecting Error)... ";
	NamedObservation *doesNotExist = new NamedObservation("doesNotExist");

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

	NamedObservation *o1New = new NamedObservation("o1");
	NamedObservation *o2New = new NamedObservation("o2");

	std::vector<Observation *> testObservationsList;
	testObservationsList.push_back(o1New);
	testObservationsList.push_back(o2New);
	finiteObservations->set(testObservationsList);

	o1 = o1New;
	o2 = o2New;
	o3 = new NamedObservation("o3");

	o1Found = false;
	o2Found = false;
	o3Found = false;

	for (auto o : *finiteObservations) {
		Observation *observation = resolve(o);
		if (observation == o1) {
			o1Found = true;
		} else if (observation == o2) {
			o2Found = true;
		} else if (observation == o3) {
			o3Found = true;
		}
	}

	if (finiteObservations->get_num_observations() == 2 && o1Found && o2Found && !o3Found &&
			finiteObservations->exists(o1) && finiteObservations->exists(o2) && !finiteObservations->exists(o3)) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	Observation *testFindObservation = nullptr;

	std::cout << "Observations: Test 'FiniteObservations::find'... ";
	try {
		testFindObservation = find_observation(finiteObservations, o1->get_name());
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'FiniteObservations::find' (Expecting Error)... ";
	try {
		testFindObservation = find_observation(finiteObservations, o3->get_name());
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteObservations;
	finiteObservations = nullptr;

	o1 = new NamedObservation("o1");
	o2 = new NamedObservation("o2");
	o3 = new NamedObservation("o3");
	Observation *o4 = new NamedObservation("o4");

	JointObservationsMap *finiteJointObservations = new JointObservationsMap(2);

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

	std::cout << "Observations: Test 'FiniteJointObservations::get'... ";

	bool o13Found = false;
	bool o14Found = false;
	bool o23Found = false;
	bool o24Found = false;

	try {
		for (auto o : *finiteJointObservations) {
			Observation *observation = resolve(o);
			JointObservation *jo = static_cast<JointObservation *>(observation);
			if (jo == nullptr) {
				throw ObservationException();
			}

			if (jo->get(0) == o1 && jo->get(1) == o3) {
				o13Found = true;
			} else if (jo->get(0) == o1 && jo->get(1) == o4) {
				o14Found = true;
			} else if (jo->get(0) == o2 && jo->get(1) == o3) {
				o23Found = true;
			} else if (jo->get(0) == o2 && jo->get(1) == o4) {
				o24Found = true;
			}
		}

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
				o13Found && o14Found && o23Found && o24Found) {
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
		o2 = new NamedObservation("o2");

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
		o3 = new NamedObservation("o3");

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
	NamedObservation *a7 = new NamedObservation("a7");
	NamedObservation *a8 = new NamedObservation("a8");
	std::vector<Observation *> finiteJointObservationsRemove = { a7, a8 };
	JointObservation *doesNotExist2 = new JointObservation(finiteJointObservationsRemove);
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
	o1New = new NamedObservation("o1");
	o2New = new NamedObservation("o2");

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
	o3 = new NamedObservation("o3");

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
		testFindObservation = find_observation(finiteJointObservations, "o1 o4");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const ObservationException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Observations: Test 'finiteJointObservations::find' (Expecting Error)... ";
	try {
		testFindObservation = find_observation(finiteJointObservations, "o3 o2");
		std::cout << " Failure." << std::endl;
	} catch (const ObservationException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete finiteJointObservations;
	finiteJointObservations = nullptr;

	return numSuccesses;
}
