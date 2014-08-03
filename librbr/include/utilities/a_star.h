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


#ifndef A_STAR_H
#define A_STAR_H


#include <vector>
#include <map>

/**
 * An implementation of the A* search algorithm.
 */
template <typename T>
class AStar {
public:
	/**
	 * The default constructor for the AStar class. It requires the specification of all
	 * relevant variables.
	 * @param	heuristic		The heuristic function estimating the distance from a node to the goal.
	 * @param	cost			The cost from the immediate transition from one node to another.
	 * @param	successors		Generate the list of successors nodes.
	 */
	AStar(double (*heuristic)(T node, T goal), double (*cost)(T n1, T n2), std::vector<T> (*successors)(T node));

	/**
	 * The deconstructor for the AStar class.
	 */
	virtual ~AStar();

	/**
	 * Solve the search problem given a starting node.
	 * @param	start				The initial node.
	 * @param	goal				The goal node.
	 * @throw	UtilityException	Either there is no path from the start to goal, or path reconstruction failed.
	 */
	void solve(T start, T goal);

	/**
	 * Get the solution path which was computed from the last call of solve.
	 * @return	The solution path from the start node to the goal node.
	 */
	const std::vector<T> &get_path();

	/**
	 * Get the number of nodes expanded from the last call of solve.
	 * @return	The number of nodes expanded.
	 */
	int get_num_nodes_expanded();

private:
	/**
	 * Reconstruct the path given a collection of maps nodes to parents and store it internally.
	 * @param	start				The initial node.
	 * @param	goal				The goal node.
	 * @param	trace				The trace of all expanded nodes to their parents.
	 * @throw	UtilityException	The trace of the route was corrupt.
	 */
	void reconstruct_path(T start, T goal, std::map<T, T> &trace);

	/**
	 * The heuristic function estimating the distance from a node to the goal.
	 */
	double (*heuristic)(T node, T goal);

	/**
	 * The cost from the immediate transition from one node to another.
	 */
	double (*cost)(T n1, T n2);

	/**
	 * Generate the list of successors nodes.
	 */
	std::vector<T> (*successors)(T node);

	/**
	 * The optimal path from the last call of solve.
	 */
	std::vector<T> path;

	/**
	 * The number of nodes expanded from the last call of solve.
	 */
	unsigned int numNodesExpanded;

};

#include "../../src/utilities/a_star.tpp"


#endif // A_STAR_H
