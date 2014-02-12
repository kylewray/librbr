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


#include "../../include/utilities/miscellaneous.h"

/**
 * Trim the left and right sides of a string, removing the whitespace.
 * @param item The string to trim.
 */
void trim_whitespace(std::string &item)
{
	// Trim from the left side.
	int left = 0;
	for (left = 0; left < item.length(); left++) {
		if (item[left] != ' ') {
			break;
		}
	}

	// Trim from the right side.
	int right = item.length();
	for (right = item.length(); right > 0; right--) {
		if (item[right - 1] != ' ') {
			break;
		}
	}

	item = item.substr(left, right - left);
}

/**
 * Remove all white space from a string.
 * @param item The string to remove white space from.
 * @return The resulting item without any white spaces.
 */
void remove_whitespace(std::string &item)
{
	item.erase(std::remove(item.begin(), item.end(), ' '), item.end());
}

/**
 * Split a string delimited by spaces ' ' into a vector of strings. If this
 * string happens to represent joint actions, joint observations, or
 * factored states, then it will split on spaces surrounding '<...>' instead.
 * @param item The string to split which is delimited by spaces ' '.
 * @return The resulting vector of items.
 */
std::vector<std::string> split_string(std::string item)
{
	std::istringstream ssItem(item);
	std::vector<std::string> list;
	std::string temp;
	std::string element;
	bool buildingElement = false;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}

		if (temp.find('<') != std::string::npos) {
			element = temp.substr(temp.find('<') + 1);
			buildingElement = true;
		} else if (temp.find('>') != std::string::npos) {
			list.push_back(element + " " + temp.substr(0, temp.find('>')));
			buildingElement = false;
		} else if (buildingElement) {
			element += " " + temp;
		} else {
			list.push_back(temp);
		}
	}

	return list;
}
