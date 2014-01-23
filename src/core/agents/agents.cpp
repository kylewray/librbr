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


#include "../../../include/core/agents/agents.h"


/**
 * The default constructor for the Agents class.
 */
Agents::Agents()
{ }

/**
 * The default deconstructor for the Agents class.
 */
Agents::~Agents()
{
	reset();
}

/**
 * Add an agent to the set of available agents.
 * @param newAgent The new agent to include in the set of available agents.
 */
void Agents::add(Agent *newAgent)
{
	agents.push_back(newAgent);
}

/**
 * Remove an agent to the set of available agents. This frees the memory.
 * @param removeAgent The agent to remove from the set of available agents.
 */
void Agents::remove(Agent *removeAgent)
{
	agents.erase(std::remove(agents.begin(), agents.end(), removeAgent), agents.end());
	delete removeAgent;
}

/**
 * Set the internal actions list given another list, performing a deep copy. This resets
 * the current list of states and frees the memory.
 * @param newActions The vector of new actions to use.
 */
void Agents::set(std::vector<Agent *> newAgents)
{
	reset();
	agents = newAgents;
}

/**
 * Return a list of all the available agents.
 * @return Return a list of available agents.
 */
std::vector<Agent *> Agents::all() const
{
	return agents;
}

/**
 * Return the number of agents.
 * @return The number of agents.
 */
int Agents::get_num_agents() const
{
	return agents.size();
}

/**
 * Get a particular agent given the name.
 * @param agentName The name of the agent.
 * @return The agent with the corresponding name provided and @code{nullptr}
 * 		if the agent was not found.
 */
Agent *Agents::find(std::string agentName)
{
	for (Agent *agent : agents) {
		if (agent->get_name().compare(agentName) == 0) {
			return agent;
		}
	}
	return nullptr;
}

/**
 * Reset the agents, clearing the internal list and freeing the memory.
 */
void Agents::reset()
{
	for (Agent *agent : agents) {
		delete agent;
	}
	agents.clear();
}
