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


#include "../include/perform_tests.h"

#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Performing Tests..." << std::endl;

	const int numTests = 12;

	int numSuccesses[numTests];
	for (int i = 0; i < numTests; i++) {
		numSuccesses[i] = 0;
	}

	numSuccesses[0] = test_agents();
	numSuccesses[1] = test_states();
	numSuccesses[2] = test_actions();
	numSuccesses[3] = test_observations();
	numSuccesses[4] = test_rewards();

	numSuccesses[5] = test_state_transitions();
	numSuccesses[6] = test_observation_transitions();

	numSuccesses[7] = test_policy();
	numSuccesses[8] = test_unified_file();
	numSuccesses[9] = test_utilities();

	numSuccesses[10] = test_mdp();
	numSuccesses[11] = test_pomdp();

	std::cout << "Agents:                 " << numSuccesses[0] << " / " << NUM_AGENT_TESTS << std::endl;
	std::cout << "States:                 " << numSuccesses[1] << " / " << NUM_STATE_TESTS << std::endl;
	std::cout << "Actions:                " << numSuccesses[2] << " / " << NUM_ACTION_TESTS << std::endl;
	std::cout << "Observations:           " << numSuccesses[3] << " / " << NUM_OBSERVATION_TESTS << std::endl;
	std::cout << "Rewards:                " << numSuccesses[4] << " / " << NUM_REWARD_TESTS << std::endl;
	std::cout << "StateTransitions:       " << numSuccesses[5] << " / " << NUM_STATE_TRANSITION_TESTS << std::endl;
	std::cout << "ObservationTransitions: " << numSuccesses[6] << " / " << NUM_OBSERVATION_TRANSITION_TESTS << std::endl;
	std::cout << "Policy:                 " << numSuccesses[7] << " / " << NUM_POLICY_TESTS << std::endl;
	std::cout << "UnifiedFile:            " << numSuccesses[8] << " / " << NUM_UNIFIED_FILE_TESTS << std::endl;
	std::cout << "Utilities:              " << numSuccesses[9] << " / " << NUM_UTILITIES_TESTS << std::endl;
	std::cout << "MDP:                    " << numSuccesses[10] << " / " << NUM_MDP_TESTS << std::endl;
	std::cout << "POMDP:                  " << numSuccesses[11] << " / " << NUM_POMDP_TESTS << std::endl;

	int total = 0;
	int totalPossible = NUM_AGENT_TESTS + NUM_STATE_TESTS + NUM_ACTION_TESTS + NUM_OBSERVATION_TESTS +
			NUM_REWARD_TESTS + NUM_STATE_TRANSITION_TESTS + NUM_OBSERVATION_TRANSITION_TESTS +
			NUM_POLICY_TESTS + NUM_UNIFIED_FILE_TESTS + NUM_UTILITIES_TESTS + NUM_MDP_TESTS + NUM_POMDP_TESTS;
	for (int i = 0; i < numTests; i++) {
		total += numSuccesses[i];
	}
	std::cout << "Total:                  " << total << " / " << totalPossible << std::endl;
	std::cout << "Done." << std::endl;

	return 0;
}
