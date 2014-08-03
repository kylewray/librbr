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


#include "../../../include/core/observations/indexed_observation.h"

unsigned int IndexedObservation::indexer = 0;

IndexedObservation::IndexedObservation()
{
	// Assign the index of the observation equal to the current value of
	// indexer. Then, increment the indexer variable in preparation
	// for the next one.
	index = indexer;
	indexer++;
}

IndexedObservation::IndexedObservation(const IndexedObservation &other)
{
    *this = other;
}

IndexedObservation::~IndexedObservation()
{ }

unsigned int IndexedObservation::get_index() const
{
	return index;
}

Observation &IndexedObservation::operator=(const Observation &other)
{
    const IndexedObservation *s = static_cast<const IndexedObservation*> (&other);
    index = s->get_index();
    return *this;
}

std::string IndexedObservation::to_string() const
{
	return std::to_string(index);
}

unsigned int IndexedObservation::hash_value() const
{
	return index;
}

unsigned int IndexedObservation::get_num_observations()
{
	return indexer;
}

void IndexedObservation::reset_indexer()
{
	indexer = 0;
}
