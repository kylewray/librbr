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


// Only perform the tests if the flag is set during compilation.
#ifdef PERFORM_TESTS


#include <iostream>

#include "../../include/utilities/unified_file.h"

// Test the UnifiedFile object. Output the success or failure for each test.
void test_unified_file()
{
	UnifiedFile file;

	std::cout << "UnifiedFile: MDP Test 1... ";
	if (!file.load("tests/resources/test_1.mdp")) {
		std::cout << "OK." << std::endl;
	}
	std::cout.flush();

	std::cout << "UnifiedFile: MDP Test 2... ";
	if (!file.load("tests/resources/test_2.mdp")) {
		std::cout << "OK." << std::endl;
	}
	std::cout.flush();

	std::cout << "UnifiedFile: POMDP Test 1... ";
	if (!file.load("tests/resources/test_1.pomdp")) {
		std::cout << "OK." << std::endl;
	}
	std::cout.flush();

	std::cout << "UnifiedFile: Dec-MDP Test 1... ";
	if (!file.load("tests/resources/test_1.dec_mdp")) {
		std::cout << "OK." << std::endl;
	}
	std::cout.flush();
}


#endif // PERFORM_TESTS
