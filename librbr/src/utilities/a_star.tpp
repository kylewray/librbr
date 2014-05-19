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


//#include "../../include/utilities/a_star.h"
#include "../../include/utilities/utility_exception.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <map>

/**
 * The default constructor for the AStar class. It requires the specification of all
 * relevant variables.
 * @param heuristic		The heuristic function estimating the distance from a node to the goal.
 * @param cost			The cost from the immediate transition from one node to another.
 * @param successors	Generate the list of successors nodes.
 */
template <typename T>
AStar<T>::AStar(double (*heuristic)(T node, T goal), double (*cost)(T n1, T n2), std::vector<T> (*successors)(T node))
{
	this->heuristic = heuristic;
	this->cost = cost;
	this->successors = successors;
	numNodesExpanded = 0;
}

/**
 * The deconstructor for the AStar class.
 */
template <typename T>
AStar<T>::~AStar()
{ }

/**
 * Solve the search problem given a starting node.
 * @param start				The initial node.
 * @param goal				The goal node.
 * @throws UtilityException	Either there is no path from the start to goal, or path reconstruction failed.
 */
template <typename T>
void AStar<T>::solve(T start, T goal)
{
	// The closed set and open priority queue (vector w/ heap functions) used in A*.
	std::set<T> closed;
	std::vector<T> open;
	open.push_back(start);

	// The actual cost traversed.
	std::map<T, double> g;
	g[start] = 0.0;

	// The expected value to reach the goal.
	std::map<T, double> f;
	f[start] = g[start] + heuristic(start, goal);

	// Recall all parents of nodes.
	std::map<T, T> trace;

	// We will keep track of how many nodes were expanded.
	numNodesExpanded = 0;

	// Create a lambda comparison function for two nodes, using their f values.
	auto node_compare = [&f] (T n1, T n2) { return f[n1] < f[n2]; };

	// While there are nodes in the open set, we must explore them.
	while (open.size() > 0) {
		// Get the node with the highest cost in the open priority queue.
		T current = open.back();

		// Check if we are at the goal state, and return if we are.
		if (current == goal) {
			reconstruct_path(start, goal, trace);
			return;
		}

		// Now the current node is expanded.
		closed.insert(current);

//		std::pop_heap(open.begin(), open.end());
		open.pop_back();

		numNodesExpanded++;

		// Iterate over the successors (i.e., neighbors) of this node.
		for (T successor : successors(current)) {
			bool update = false;
			bool add = false;

			// First, compute the g value for the successor.
			double gSuccessor = g[current] + cost(current, successor);

			// If successor is has not been explored, then add it to the open list.
			if (std::find(closed.begin(), closed.end(), successor) == closed.end() &&
					std::find(open.begin(), open.end(), successor) == open.end()) {
				update = true;
				add = true;
			} else {
				// Check if this has no g (or f) value yet, or if it needs an update. This
				// has a separate find result variable to avoid two searches through g.
				typename std::map<T, double>::const_iterator gSuccessorResult = g.find(successor);
				if (gSuccessorResult == g.end() || gSuccessor < gSuccessorResult->second) {
					update = true;
				}
			}

			// As we have determined, if successor is new or the g value of next is old, then store
			// it and all other important data about it.
			if (update) {
				trace[successor] = current;
				g[successor] = gSuccessor;
				f[successor] = gSuccessor + heuristic(successor, goal);
			}

			// If needed, add the node. This must be called after we have defined the f and g
			// functions for successor due to the node compare function.
			if (add) {
				open.push_back(successor);
				std::push_heap(open.begin(), open.end(), node_compare);
			}
		}
	}

	// If we reached the end without a goal, then raise an exception.
	throw UtilityException();
}

/**
 * Get the solution path which was computed from the last call of solve.
 * @return The solution path from the start node to the goal node.
 */
template <typename T>
const std::vector<T> &AStar<T>::get_path()
{
	return path;
}

/**
 * Get the number of nodes expanded from the last call of solve.
 * @return The number of nodes expanded.
 */
template <typename T>
int AStar<T>::get_num_nodes_expanded()
{
	return numNodesExpanded;
}

/**
 * Reconstruct the path given a collection of maps nodes to parents and store it internally.
 * @param start				The initial node.
 * @param goal				The goal node.
 * @param trace				The trace of all expanded nodes to their parents.
 * @throws UtilityException	The trace of the route was corrupt.
 */
template <typename T>
void AStar<T>::reconstruct_path(T start, T goal, std::map<T, T> &trace)
{
	// This variable will hold the actual path traversed.
	path.clear();
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
}
