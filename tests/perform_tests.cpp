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

#include "core/test_agents.h"
#include "core/test_states.h"
#include "core/test_actions.h"
#include "core/test_observations.h"
#include "core/test_rewards.h"

#include "core/test_state_transitions.h"
#include "core/test_observation_transitions.h"

#include "core/test_policy.h"

#include "utilities/test_unified_file.h"

//#include "mdp/value_iteration.h"
//#include "mdp/policy_iteration.h"

//#include "pomdp/value_iteration.h"
//#include "pomdp/policy_iteration.h"

// ...

/**
 * The main function which executes the test suite.
 * @param argc The number of arguments to main.
 * @param argv The array of strings passed to main.
 * @return Return 0 if no error, non-zero otherwise.
 */
int main(int argc, char *argv[])
{
	std::cout << "Performing Tests..." << std::endl;

	int numTests = 9;
	int totalErrors = 0;

	int numErrors[numTests];
	for (int i = 0; i < numTests; i++) {
		numErrors[i] = 0;
	}

	numErrors[0] = test_agents();
	numErrors[1] = test_states();
	numErrors[2] = test_actions();
	numErrors[3] = test_observations();
	numErrors[4] = test_rewards();

	numErrors[5] = test_state_transitions();
	numErrors[6] = test_observation_transitions();

	numErrors[7] = test_policy();

	numErrors[8] = test_unified_file();

	std::cout << "Number of Errors [Agents]: " << numErrors[0] << std::endl;
	std::cout << "Number of Errors [States]: " << numErrors[1] << std::endl;
	std::cout << "Number of Errors [Actions]: " << numErrors[2] << std::endl;
	std::cout << "Number of Errors [Observations]: " << numErrors[3] << std::endl;
	std::cout << "Number of Errors [Rewards]: " << numErrors[4] << std::endl;
	std::cout << "Number of Errors [StateTransitions]: " << numErrors[5] << std::endl;
	std::cout << "Number of Errors [ObservationTransitions]: " << numErrors[6] << std::endl;
	std::cout << "Number of Errors [Policy]: " << numErrors[7] << std::endl;
	std::cout << "Number of Errors [UnifiedFile]: " << numErrors[8] << std::endl;

	for (int i = 0; i < numTests; i++) {
		totalErrors += numErrors[i];
	}
	std::cout << "Number of Errors [Total]: " << totalErrors << std::endl;
	std::cout << "Done." << std::endl;

	return 0;
}


#endif // PERFORM_TESTS
