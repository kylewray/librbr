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


#include "../../../include/core/policy/policy.h"


/**
 * A virtual deconstructor to prevent errors upon the deletion of a child object.
 */
Policy::~Policy()
{ }

/**
 * A virtual function which must load a policy file.
 * @param filename The name and path of the file to load.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool Policy::load(std::string filename)
{
	return false;
}

/**
 * A virtual function which must save a policy file.
 * @param filename The name and path of the file to save.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool Policy::save(std::string filename) const
{
	return false;
}

/**
 * A function which follows the defined policy, having the current state stored internally,
 * and returns the action to select next.
 */
Action Policy::next()
{
	return Action();
}

/**
 * Reset the policy to the initial state.
 */
void Policy::reset()
{ }
