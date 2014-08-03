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


#include "../../../include/core/observations/named_observation.h"

NamedObservation::NamedObservation()
{ }

NamedObservation::NamedObservation(std::string initialName)
{
	name = initialName;
}

NamedObservation::NamedObservation(const NamedObservation &other)
{
	*this = other;
}

NamedObservation::~NamedObservation()
{ }

void NamedObservation::set_name(std::string newName)
{
	name = newName;
}

std::string NamedObservation::get_name() const
{
	return name;
}

Observation &NamedObservation::operator=(const Observation &other)
{
    const NamedObservation *o = static_cast<const NamedObservation*>(&other);
	name = o->get_name();
	return *this;
}

std::string NamedObservation::to_string() const
{
	return name;
}

unsigned int NamedObservation::hash_value() const
{
	return hash_value(name);
}

unsigned int NamedObservation::hash_value(std::string nameToHash)
{
	unsigned int hash = 7;
	for (char c : nameToHash) {
		hash = 31 * hash + (int)c;
	}
	return hash;
}
