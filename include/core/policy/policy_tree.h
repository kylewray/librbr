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


#ifndef MAP_POLICY_H
#define MAP_POLICY_H


#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "policy.h"

#include "../states/state.h"
#include "../states/finite_states.h"
#include "../actions/action.h"
#include "../actions/finite_actions.h"
#include "../observations/observation.h"
#include "../observations/finite_observations.h"
#include "../horizon.h"

/**
 * A small structure for a policy tree's node.
 */
struct PolicyTreeNode {
	/**
	 * The default constructor for the PolicyTreeNode class. It defaults to a null action.
	 */
	PolicyTreeNode();

	/**
	 * A constructor for the PolicyTreeNode class which allows the specification of the action.
	 * @param a The corresponding action at this node.
	 */
	PolicyTreeNode(const Action *a);

	/**
	 * The current node's action.
	 */
	const Action *action;

	/**
	 * A pointer to each of the children following a given observation.
	 * If next is empty, then we are at a leaf.
	 */
	std::map<const Observation *, PolicyTreeNode *> next;

};

/**
 * A policy tree for POMDPs (and possibly Dec-POMDPs). Each node represents an
 * action to take, and each edge represents an observation made at the time step.
 * It is likely that this will only be used for finite horizon scenarios.
 *
 * This requires a finite observations object. The reason being that the set of
 * observations is the branching factor for the tree.
 */
class PolicyTree : public Policy {
public:
	/**
	 * The default constructor for a PolicyTree object. It defaults to a horizon of 0, which
	 * is a single action.
	 */
	PolicyTree();

	/**
	 * A constructor for a PolicyTree object which specifies the horizon. If the horizon
	 * is zero, then it will create an empty tree (no nodes).
	 * @param observations	The finite set of observations.
	 * @param horizon 		The horizon of the problem.
	 */
	PolicyTree(const FiniteObservations *observations, unsigned int horizon);

	/**
	 * A constructor for a MapPolicy object which specifies the horizon.
	 * @param observations	The finite set of observations.
	 * @param horizon 		The horizon object from the MDP-like object.
	 */
	PolicyTree(const FiniteObservations *observations, const Horizon *horizon);

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~PolicyTree();

	/**
	 * Set the action given a history of observations.
	 * @param history			The history of observations.
	 * @param action			The action to take after this history of observations.
	 * @throws PolicyException	The policy was not defined for this history.
	 */
	void set(const std::vector<const Observation *> &history, const Action *action);

	/**
	 * Get the action given a history of observations.
	 * @param history The history of observations.
	 * @return The action to take given the history of observations provided.
	 * @throws PolicyException The policy was not defined for this history.
	 */
	const Action *get(const std::vector<const Observation *> &history) const;

	/**
	 * A function which must load a policy file.
	 * @param filename		The name and path of the file to load.
	 * @param actions		The actions object which contains the actual action objects to be mapped.
	 * @param observations	The observations object which contains the actual observation objects to be mapped.
	 * @param horizon		The horizons object to ensure valid policy creation.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename, const FiniteActions *actions, const FiniteObservations *observations, const Horizon *horizon);

	/**
	 * A function which must save a policy file.
	 * @param filename The name and path of the file to save.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename) const;

	/**
	 * A function which follows the defined policy, having the current state stored internally,
	 * and returns the action to select next.
	 * @throws PolicyException The horizon has already been reached; no more planning has been done.
	 */
	virtual const Action *next(const Observation *observation);

	/**
	 * Reset the policy, clearing the entire tree.
	 */
	virtual void reset();

private:
	/**
	 * Generate the policy tree recursively.
	 * @param observations	The finite set of observations (branching factor).
	 * @param horizon		The remaining horizon value; once zero, recursion terminates.
	 * @returns The root of this node's subtree.
	 */
	PolicyTreeNode *generate_tree(const FiniteObservations *observations, unsigned int horizon);

	/**
	 * Traverse the tree following the history provided.
	 * @param history The history of observations.
	 * @return The final node after following the history.
	 * @throws PolicyException The policy was not defined for this history.
	 */
	PolicyTreeNode *traverse(const std::vector<const Observation *> &history) const;

	/**
	 * Save the policy tree recursively.
	 * @param file		The handle to the output file.
	 * @param node 		The current node.
	 * @param history	The history of observations so far.
	 */
	void save_tree(std::ofstream &file, PolicyTreeNode *node, std::vector<const Observation *> history) const;

	/**
	 * Defines the policy itself; it's the internal mapping from states to actions. There is
	 * one of these mappings for each level.
	 */
	std::vector<PolicyTreeNode *> nodes;

	/**
	 * The root node of the policy tree.
	 */
	PolicyTreeNode *root;

	/**
	 * The current node in the tree.
	 */
	PolicyTreeNode *current;

};


#endif // POLICY_TREE_H
