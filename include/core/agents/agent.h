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


#ifndef AGENT_H
#define AGENT_H


#include <string>


/**
 * An abstract agent object. Typically, this will just be a string; however, by abstracting
 * the agents, we are able to have agents that are classes in-and-of-themselves.
 */
class Agent {
public:
	/**
	 * The default constructor of the Agent object.
	 */
	Agent();

	/**
	 * The constructor of the Agent object which allows initial specification of the unique name.
	 * @param initialName The unique name of the state.
	 */
	Agent(std::string initialName);

	/**
	 * The copy constructor of the Agent object.
	 * @param other The agent to copy.
	 */
	Agent(const Agent &other);

	/**
	 * The default deconstructor of the Agent object.
	 */
	virtual ~Agent();

	/**
	 * Overload the equals operator to set this agent equal to the agent provided.
	 * @param other The agent to copy.
	 * @return The new version of this agent.
	 */
	virtual Agent &operator=(const Agent &other);

	/**
	 * Overload the equality comparison operator.
	 * @param other The agent to compare.
	 * @return Returns @code{true} if this agent is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const Agent &other);

	/**
	 * Overload the less than operator for comparison.
	 * @param other The agent to compare.
	 * @return Returns @code{true} if this agent is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const Agent &other) const;

	/**
	 * All agents must have the ability to convert the internal representation to a string.
	 * For most policy solvers, this must be unique.
	 */
	std::string name;

};


#endif // AGENT_H
