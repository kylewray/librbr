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


#include <iostream>

#include "../../include/utilities/a_star.h"
#include "../../include/utilities/utility_exception.h"

/**
 * The default constructor for the AStar class. It requires the specification of all
 * relevant variables.
 * @param heuristic		The heuristic function estimating the distance from a node to the goal.
 * @param cost			The cost from the immediate transition from one node to another.
 * @param successor		Generate the list of successor nodes.
 */
template <typename T>
AStar<T>::AStar(double (*heuristic)(T node, T goal), double (*cost)(T n1, T n2), std::vector<T> (*successor)(T node))
{
	this->heuristic = heuristic;
	this->cost = cost;
	this->successor = successor;
}

/**
 * The deconstructor for the AStar class.
 */
template <typename T>
AStar<T>::~AStar()
{ }

/**
 * Solve the search problem given a starting node.
 * @param start	The initial node.
 * @param goal	The goal node.
 */
template <typename T>
std::vector<T> AStar<T>::solve(T start, T goal)
{
	// The open and closed sets used in A*.
	std::set<T> closed;
	std::set<T> open;
	open.insert(start);

	// The actual cost traversed.
	std::map<T, double> g;
	g[start] = 0.0;

	// The expected value to reach the goal.
	std::map<T, double> f;
	f[start] = g[start] + heuristic(start, goal);

	// Recall all parents of nodes.
	std::map<T, T> trace;

	// While there are nodes in the open set, we must explore them.
	while (open.size() > 0) {
		// TODO: Implement A*.
	}

	// The result follows the trace from the goal, back to the start.
	return reconstruct_path(start, goal, trace);
}

/**
 * Reconstruct the path given a collection of maps nodes to parents.
 * @param start	The initial node.
 * @param goal	The goal node.
 * @param trace	The trace of all expanded nodes to their parents.
 * @return The path from the start node to the goal node.
 * @throws UtilityException	The trace of the route was corrupt.
 */
template <typename T>
std::vector<T> AStar<T>::reconstruct_path(T start, T goal, std::map<T, T> &trace)
{
	// This variable will hold the actual path traversed.
	std::vector<T> path;
	path.push_back(goal);

	// Continue to iterate over the nodes until the start node is reached.
	T current = goal;
	while (current != start) {
		// Attempt to find the parent, throwing an exception if it cannot do so.
		typename std::map<T, T>::const_iterator result = trace.find(current);
		if (result == trace.end()) {
			throw UtilityException();
		}

		// Set the current node equal to the value found and push back the
		// current node on the path.
		current = result->second;
		path.push_back(current);
	}

	// Since we used the "push_back" method for speed, at the end, reverse the order.
	std::reverse(path.begin(), path.end());

	return path;
}
