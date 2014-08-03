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
#include <tuple>
#include <math.h>

#include "../../../librbr/include/utilities/a_star.h"

int test_utilities() {
	int numSuccesses = 0;

	// Setup the knight problem's functions.
	auto knightHeuristic = [] (std::pair<int, int> node, std::pair<int, int> goal) {
		return sqrt(pow((float)(node.first - goal.first), 2) + pow((float)(node.second - goal.second), 2));
	};

	auto knightCost = [] (std::pair<int, int> n1, std::pair<int, int> n2) {
		return 3.0;
	};

	auto knightSuccessors = [] (std::pair<int, int> node) {
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

	AStar<std::pair<int, int>> astar(knightHeuristic, knightCost, knightSuccessors);

	std::cout << "AStar: Solving Knight Problem #1...";
	std::cout.flush();

	std::pair<int, int> start(0, 0);
	std::pair<int, int> goal(8, 19);
	astar.solve(start, goal);
	std::vector<std::pair<int, int> > solution = astar.get_path();

	if (solution.size() == 12 &&
			solution[0].first == 0 && solution[0].second == 0 &&
			solution[1].first == 2 && solution[1].second == 1 &&
			solution[2].first == 1 && solution[2].second == 3 &&
			solution[3].first == 2 && solution[3].second == 5 &&
			solution[4].first == 3 && solution[4].second == 7 &&
			solution[5].first == 4 && solution[5].second == 9 &&
			solution[6].first == 5 && solution[6].second == 11 &&
			solution[7].first == 6 && solution[7].second == 13 &&
			solution[8].first == 7 && solution[8].second == 15 &&
			solution[9].first == 8 && solution[9].second == 17 &&
			solution[10].first == 6 && solution[10].second == 18 &&
			solution[11].first == 8 && solution[11].second == 19) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	return numSuccesses;
}
