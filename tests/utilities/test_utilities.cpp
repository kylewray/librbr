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
#include <tuple>
#include <math.h>

#include "../../include/utilities/a_star.h"

/**
 * Test the utilities objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_utilities() {
	int numSuccesses = 0;

	// Setup the knight problem's functions.
	auto heuristic = [] (std::pair<int, int> node, std::pair<int, int> goal) {
		return sqrt(pow((float)(node.first - goal.first), 2) + pow((float)(node.second - goal.second), 2));
	};

	auto cost = [] (std::pair<int, int> n1, std::pair<int, int> n2) {
		return 3.0;
	};

	auto successors = [] (std::pair<int, int> node) {
		std::vector<std::pair<int, int> > nodes;
		for (int i = -2; i <= 2; i++) {
			for (int j = -2; j <= 2; j++) {
				if (i != 0 && j != 0 && abs(i) != abs(j)) {
					std::pair<int, int> newNode(node.first + i, node.second + j);
					nodes.push_back(newNode);
				}
			}
		}
		return nodes;
	};

	AStar<std::pair<int, int>> astar(heuristic, cost, successors);

	std::cout << "AStar: Solving Knight Problem #1...";
	std::cout.flush();

	std::pair<int, int> start(0, 0);
	std::pair<int, int> goal(8, 19);
	astar.solve(start, goal);
	std::vector<std::pair<int, int> > solution = astar.get_path();

	std::cout << "Number of Expansions: " << astar.get_num_nodes_expanded() << std::endl;
	std::cout << "Solution Size: " << solution.size() << std::endl;

	if (solution.size() == 12) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	return numSuccesses;
}


#endif // PERFORM_TESTS_H
