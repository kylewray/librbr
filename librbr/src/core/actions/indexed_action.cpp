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


#include "../../../include/core/actions/indexed_action.h"

unsigned int IndexedAction::indexer = 0;

IndexedAction::IndexedAction()
{
	// Assign the index of the action equal to the current value of
	// indexer. Then, increment the indexer variable in preparation
	// for the next one.
	index = indexer;
	indexer++;
}

IndexedAction::IndexedAction(const IndexedAction &other)
{
    *this = other;
}

IndexedAction::~IndexedAction()
{ }

unsigned int IndexedAction::get_index() const
{
	return index;
}

Action &IndexedAction::operator=(const Action &other)
{
    const IndexedAction *s = static_cast<const IndexedAction*> (&other);
    index = s->get_index();
    return *this;
}

std::string IndexedAction::to_string() const
{
	return std::to_string(index);
}

unsigned int IndexedAction::hash_value() const
{
	return index;
}

unsigned int IndexedAction::get_num_actions()
{
	return indexer;
}

void IndexedAction::reset_indexer()
{
	indexer = 0;
}
