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


#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H


#include <sstream>
#include <string>
#include <vector>

/**
 * Trim the left and right sides of a string, removing the whitespace.
 * @param	item	The string to trim.
 */
void trim_whitespace(std::string &item);

/**
 * Remove all white space from a string.
 * @param	item	The string to remove white space from.
 * @return	The resulting item without any white spaces.
 */
void remove_whitespace(std::string &item);

/**
 * Split a string delimited by spaces ' ' into a vector of strings. If this
 * string happens to represent joint actions, joint observations, or
 * factored states, then it will split on spaces surrounding '<...>' instead.
 * @param	item	The string to split which is delimited by spaces ' '.
 * @return	The resulting vector of items.
 */
std::vector<std::string> split_string_by_space(std::string item);

/**
 * Split a string delimited by colons ':' into a vector of strings. This
 * trims whitespace around each element.
 * @param	item	The string to split which is delimited by colons ':'.
 * @return	The resulting vector of items.
 */
std::vector<std::string> split_string_by_colon(std::string item);


#endif // STRING_MANIPULATION_H
