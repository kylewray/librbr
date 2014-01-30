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

/**
 * Test the UnifiedFile object. Output the success or failure for each test.
 * @return The number of errors encountered during execution.
 */
int test_unified_file()
{
	int numErrors = 0;
	UnifiedFile file;

	std::cout << "UnifiedFile: 'test_01.mdp'...";
	if (!file.load("tests/resources/test_01.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_02.mdp'... ";
	if (!file.load("tests/resources/test_02.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_03.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_03.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_04.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_04.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_05.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_05.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_06.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_06.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_07.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_07.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_08.mdp'... ";
	if (!file.load("tests/resources/test_08.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_09.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_09.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_10.mdp'... ";
	if (!file.load("tests/resources/test_10.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_11.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_11.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_12.mdp'... ";
	if (!file.load("tests/resources/test_12.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_13.mdp' (Expecting Error)...\n\t";
	if (file.load("tests/resources/test_13.mdp")) {
		std::cout << "\tSuccess." << std::endl;
	} else {
		std::cout << "\tFailure." << std::endl;
		numErrors++;
	}

	std::cout << "UnifiedFile: 'test_14.mdp'...";
	if (!file.load("tests/resources/test_14.mdp")) {
		std::cout << " Success." << std::endl;
	} else {
		std::cout << " Failure." << std::endl;
		numErrors++;
	}

	return numErrors;
}


#endif // PERFORM_TESTS
