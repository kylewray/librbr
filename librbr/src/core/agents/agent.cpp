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


#include "../../../include/core/agents/agent.h"

Agent::Agent()
{ }

Agent::Agent(std::string initialName)
{
	name = initialName;
}

Agent::Agent(const Agent &other)
{
	*this = other;
}

Agent::~Agent()
{ }

void Agent::set_name(std::string newName)
{
	name = newName;
}

std::string Agent::get_name() const
{
	return name;
}

Agent &Agent::operator=(const Agent &other)
{
	name = other.get_name();
	return *this;
}

bool Agent::operator==(const Agent &other) const
{
	return hash_value() == other.hash_value();
}

bool Agent::operator<(const Agent &other) const
{
	return hash_value() < other.hash_value();
}

std::string Agent::to_string() const
{
	return name;
}

unsigned int Agent::hash_value() const
{
	unsigned int hash = 7;
	for (char c : name) {
		hash = 31 * hash + (int)c;
	}
	return hash;
}
