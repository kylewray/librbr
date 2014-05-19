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


#include "../../../include/core/agents/agents.h"
#include "../../../include/core/agents/agent_exception.h"

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
void Agents::add(const Agent *newAgent)
{
	agents.push_back(newAgent);
}

/**
 * Remove an agent to the set of available agents. This frees the memory.
 * @param removeAgent 		The agent to remove from the set of available agents.
 * @throws AgentException	The agent was not found in the agents list.
 */
void Agents::remove(const Agent *removeAgent)
{
	if (std::find(agents.begin(), agents.end(), removeAgent) == agents.end()) {
		throw AgentException();
	}

	agents.erase(std::remove(agents.begin(), agents.end(), removeAgent), agents.end());
	delete removeAgent;
}

/**
 * Set the internal actions list given another list, performing a deep copy. This resets
 * the current list of states and frees the memory.
 * @param newActions The vector of new actions to use.
 */
void Agents::set(const std::vector<const Agent *> &newAgents)
{
	reset();
	agents = newAgents;
}

/**
 * Get an agent at a particular index. The index is determined by the order in which
 * agents were added.
 * @param agentIndex The index of the agent.
 * @return The agent at the corresponding index.
 * @throws AgentException The index was invalid.
 */
const Agent *Agents::get(int agentIndex) const
{
	if (agentIndex < 0 || agentIndex >= agents.size()) {
		throw AgentException();
	}

	return agents[agentIndex];
}

/**
 * Return a list of all the available agents.
 * @return Return a list of available agents.
 */
const std::vector<const Agent *> &Agents::all() const
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
 * @return The agent with the corresponding name provided
 * @throws AgentException The name was invalid.
 */
const Agent *Agents::find(std::string agentName) const
{
	for (const Agent *agent : agents) {
		if (agent->get_name().compare(agentName) == 0) {
			return agent;
		}
	}
	throw AgentException();
}

/**
 * Reset the agents, clearing the internal list and freeing the memory.
 */
void Agents::reset()
{
	for (const Agent *agent : agents) {
		delete agent;
	}
	agents.clear();
}

/**
 * To facilitate easy iteration, return a constant beginning of the agents vector.
 * @return The iterator which points to a constant beginning of the agents vector.
 */
std::vector<const Agent *>::const_iterator Agents::begin() const
{
	return agents.begin();
}

/**
 * To facilitate easy iteration, return a constant end of the agents vector.
 * @return The iterator which points to a constant end of the agents vector.
 */
std::vector<const Agent *>::const_iterator Agents::end() const
{
	return agents.end();
}
