/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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

Agents::Agents()
{ }

Agents::~Agents()
{
	reset();
}

void Agents::add(const Agent *newAgent)
{
	agents.push_back(newAgent);
}

void Agents::remove(const Agent *removeAgent)
{
	if (std::find(agents.begin(), agents.end(), removeAgent) == agents.end()) {
		throw AgentException();
	}

	agents.erase(std::remove(agents.begin(), agents.end(), removeAgent), agents.end());
	delete removeAgent;
}

void Agents::set(const std::vector<const Agent *> &newAgents)
{
	reset();
	agents = newAgents;
}

const Agent *Agents::get(int agentIndex) const
{
	if (agentIndex < 0 || agentIndex >= agents.size()) {
		throw AgentException();
	}

	return agents[agentIndex];
}

unsigned int Agents::get_num_agents() const
{
	return agents.size();
}

const Agent *Agents::find(std::string agentName) const
{
	for (const Agent *agent : agents) {
		if (agent->get_name().compare(agentName) == 0) {
			return agent;
		}
	}
	throw AgentException();
}

void Agents::reset()
{
	for (const Agent *agent : agents) {
		delete agent;
	}
	agents.clear();
}

std::vector<const Agent *>::const_iterator Agents::begin() const
{
	return agents.begin();
}

std::vector<const Agent *>::const_iterator Agents::end() const
{
	return agents.end();
}
