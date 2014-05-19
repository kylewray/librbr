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

#include "../../../librbr/include/core/agents/agent.h"
#include "../../../librbr/include/core/agents/agents.h"
#include "../../../librbr/include/core/agents/agent_exception.h"

/**
 * Test the agents objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_agents()
{
	int numSuccesses = 0;

	Agent *alice = new Agent("Alice");
	Agent *bob = new Agent("Bob");

	Agents *agents = new Agents();
	agents->add(alice);
	agents->add(bob);

	std::cout << "Agents: Test 'Agents::add' and 'Agents::get'... ";
	try {
		if (agents->get_num_agents() == 2 && agents->get(0) == alice &&
				agents->get(1) == bob) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const AgentException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::remove'... ";
	try {
		agents->remove(alice);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const AgentException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::remove' (Check Result) and 'Agents::all'... ";
	if (agents->get_num_agents() == 1 && agents->get(0) == bob &&
			agents->all().size() == agents->get_num_agents()) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::remove' (Expecting Error)... ";
	Agent *charlie = new Agent("Charlie");
	try {
		agents->remove(charlie);
		std::cout << " Failure." << std::endl;
	} catch (const AgentException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	std::cout << "Agents: Test 'Agents::set'... ";

	alice = new Agent("Alice");

	std::vector<const Agent *> testAgentsList;
	testAgentsList.push_back(alice);
	testAgentsList.push_back(charlie);

	try {
		agents->set(testAgentsList);
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const AgentException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::set' (Check Result)... ";

	try {
		if (agents->get_num_agents() == 2 && agents->get(0) == alice &&
				agents->get(1) == charlie) {
			std::cout << " Success." << std::endl;
			numSuccesses++;
		} else {
			std::cout << " Failure." << std::endl;
		}
	} catch (const AgentException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::find'... ";

	const Agent *testFindAgent = nullptr;

	try {
		testFindAgent = agents->find("Charlie");
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} catch (const AgentException &err) {
		std::cout << " Failure." << std::endl;
	}

	std::cout << "Agents: Test 'Agents::find' (Expecting Error)... ";
	try {
		testFindAgent = agents->find("Dave");
		std::cout << " Failure." << std::endl;
	} catch (const AgentException &err) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	}

	delete agents;
	agents = nullptr;

	return numSuccesses;
}
