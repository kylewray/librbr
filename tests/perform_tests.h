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


//#ifdef PERFORM_TESTS_H		// Build actual library.
#ifndef PERFORM_TESTS_H	// Build tests executable.
#define PERFORM_TESTS_H


#define NUM_AGENT_TESTS 8
#define NUM_STATE_TESTS 21
#define NUM_ACTION_TESTS 21
#define NUM_OBSERVATION_TESTS 21
#define NUM_REWARD_TESTS 5
#define NUM_STATE_TRANSITION_TESTS 5
#define NUM_OBSERVATION_TRANSITION_TESTS 5
#define NUM_POLICY_TESTS 0
#define NUM_UNIFIED_FILE_TESTS 20

/**
 * Test the agents objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_agents();

/**
 * Test the states objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_states();

/**
 * Test the actions objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_actions();

/**
 * Test the observation objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_observations();

/**
 * Test the reward objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_rewards();

/**
 * Test the state transition objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_state_transitions();

/**
 * Test the observation transition objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_observation_transitions();

/**
 * Test the policy objects. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_policy();

/**
 * Test the UnifiedFile object. Output the success or failure for each test.
 * @return The number of successes during execution.
 */
int test_unified_file();


#endif // PERFORM_TESTS_H
