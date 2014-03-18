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


#ifndef AGENTS_H
#define AGENTS_H


#include "agent.h"

#include <vector>
#include <algorithm>

/**
 * A class for finite sets of agents in an MDP-like object. The base assumption is that the set of
 * agents is always finite. Also, if this class is used, then it will be for the decentralized
 * MDP and POMDP classes.
 */
class Agents {
public:
	/**
	 * The default constructor for the Agents class.
	 */
	Agents();

	/**
	 * The default deconstructor for the Agents class.
	 */
	virtual ~Agents();

	/**
	 * Add an agent to the set of available agents.
	 * @param newAgent The new agent to include in the set of available agents.
	 */
	void add(const Agent *newAgent);

	/**
	 * Remove an agent to the set of available agents. This frees the memory.
	 * @param removeAgent 		The agent to remove from the set of available agents.
	 * @throws AgentException	The agent was not found in the agents list.
	 */
	void remove(const Agent *removeAgent);

	/**
	 * Set the internal actions list given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory.
	 * @param newActions The vector of new actions to use.
	 */
	void set(const std::vector<const Agent *> &newAgents);

	/**
	 * Get an agent at a particular index. The index is determined by the order in which
	 * agents were added.
	 * @param agentIndex The index of the agent.
	 * @return The agent at the corresponding index.
	 * @throws AgentException The index was invalid.
	 */
	const Agent *get(int agentIndex) const;

	/**
	 * Return a list of all the available agents.
	 * @return Return a list of available agents.
	 */
	const std::vector<const Agent *> &all() const;

	/**
	 * Return the number of agents.
	 * @return The number of agents.
	 */
	virtual int get_num_agents() const;

	/**
	 * Get a particular agent given the name.
	 * @param agentName The name of the agent.
	 * @return The agent with the corresponding name provided.
	 * @throws AgentException The name was invalid.
	 */
	virtual const Agent *find(std::string agentName) const;

	/**
	 * Reset the agents, clearing the internal list and freeing the memory.
	 */
	void reset();

	/**
	 * To facilitate easy iteration, return a constant beginning of the agents vector.
	 * @return The iterator which points to a constant beginning of the agents vector.
	 */
	std::vector<const Agent *>::const_iterator begin() const;

	/**
	 * To facilitate easy iteration, return a constant end of the agents vector.
	 * @return The iterator which points to a constant end of the agents vector.
	 */
	std::vector<const Agent *>::const_iterator end() const;

protected:
	/**
	 * The list of all agents.
	 */
	std::vector<const Agent *> agents;

};


#endif // AGENTS_H
