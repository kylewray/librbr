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


#include <vector>

#include "agent.h"


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
	void add(Agent *newAgent);

	/**
	 * Remove an agent to the set of available agents. This frees the memory.
	 * @param removeAgent The agent to remove from the set of available agents.
	 */
	void remove(Agent *removeAgent);

	/**
	 * Set the internal actions list given another list, performing a deep copy. This resets
	 * the current list of states and frees the memory.
	 * @param newActions The vector of new actions to use.
	 */
	void set(std::vector<Agent *> newAgents);

	/**
	 * Return a list of all the available agents.
	 * @return Return a list of available agents.
	 */
	std::vector<Agent *> all() const;

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
	virtual Agent *find(std::string agentName);

	/**
	 * Reset the agents, clearing the internal list and freeing the memory.
	 */
	void reset();

protected:
	/**
	 * The list of all agents.
	 */
	std::vector<Agent *> agents;

};


#endif // AGENTS_H
