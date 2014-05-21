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


#include "../../../include/core/agents/agent.h"

/**
 * The default constructor of the Agent object.
 */
Agent::Agent()
{ }

/**
 * The constructor of the Agent object which allows initial specification of the unique name.
 * @param initialName The unique name of the state.
 */
Agent::Agent(std::string initialName)
{
	name = initialName;
}

/**
 * The copy constructor of the Agent object.
 * @param other The agent to copy.
 */
Agent::Agent(const Agent &other)
{
	*this = other;
}

/**
 * The default deconstructor of the Agent object.
 */
Agent::~Agent()
{ }

/**
 * Set the name.
 * @param newName The new name.
 */
void Agent::set_name(std::string newName)
{
	name = newName;
}

/**
 * Get the name.
 * @return The current name.
 */
std::string Agent::get_name() const
{
	return name;
}

/**
 * Overload the equals operator to set this agent equal to the agent provided.
 * @param other The agent to copy.
 * @return The new version of this agent.
 */
Agent &Agent::operator=(const Agent &other)
{
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The agent to compare.
 * @return Returns @code{true} if this agent is equal to the other; @code{false} otherwise.
 */
bool Agent::operator==(const Agent &other) const
{
	return hash_value() == other.hash_value();
}

/**
 * Overload the less than operator for comparison.
 * @param other The agent to compare.
 * @return Returns @code{true} if this agent is less than the other; @code{false} otherwise.
 */
bool Agent::operator<(const Agent &other) const
{
	return hash_value() < other.hash_value();
}

/**
 * Returns a string representation of this agent.
 * @return Returns the string representing this agent.
 */
std::string Agent::to_string() const
{
	return name;
}

/**
 * Returns a hash value used to quickly identify this action in a collection of agent.
 * @returns Returns the hash value of this agent.
 */
int Agent::hash_value() const
{
	int hash = 7;
	for (char c : name) {
		hash = 31 * hash + (int)c;
	}
	return hash;
}
