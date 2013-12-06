/**
 * 	The MIT License (MIT)
 *
 * 	Copyright (c) 2013 Kyle Wray and Luis Pineda
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy of
 * 	this software and associated documentation files (the "Software"), to deal in
 * 	the Software without restriction, including without limitation the rights to
 * 	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * 	the Software, and to permit persons to whom the Software is furnished to do so,
 * 	subject to the following conditions:
 *
 * 	The above copyright notice and this permission notice shall be included in all
 * 	copies or substantial portions of the Software.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * 	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * 	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * 	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * 	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <iostream>
#include <string>

#include "../scp/scp.h"
#include "../scp/scp_types.h"

#include "fh_dec_pomdp_solution.h"


/**
 *  A Finite Horizon Decentralized Partially Observable Markov Decision Process (Dec-POMDP).
 */
class FHDecPOMDP : public SCP {

public:

	/**
	 * The constructor for the FHDecPOMDP class.
	 */
	FHDecPOMDP() {
		reset();
	}

	/**
	 * 	The constructor for the FHDecPOMDP class, which enables loading.
	 *
	 *	@param filename The JSON file to load.
	 */
	FHDecPOMDP(std::string filename) {
		reset();
		load(filename);
	}

	/**
	 * A function which loads the Dec-POMDP as a stochastic control process file.
	 *
	 * @param filename The name of the file to load.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load(std::string filename);

	/**
	 * A function which saves the Dec-POMDP as a stochastic control process file.
	 *
	 * @param filename The name of the file to save.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool save(std::string filename);

private:

	/**
	 * Reset the internal variables to default values. This does not free memory.
	 */
	void reset() {
		numAgents = 0;
		numStates = 0;

		numActions = NULL;
		numJointActions = 0;

		stateTransitions = NULL;

		numObservations = NULL;
		numJointObservations = 0;

		observationTransitions = NULL;

		rewards = NULL;

		horizon = 0;
		gamma = 0.9;

		agents = NULL;
		states = NULL;
		actions = NULL;
		observations = NULL;
	}

	/**
	 * Free the memory of the internal variables. This calls reset automatically.
	 */
	void free() {
		if (rewards != NULL) {
			for (int i = 0; i < numStates; i++) {
				for (int j = 0; j < numJointActions; j++) {
					delete [] rewards[i][j];
				}
				delete [] rewards[i];
			}
			delete [] rewards;
		}

		if (observationTransitions != NULL) {
			for (int i = 0; i < numStates; i++) {
				for (int j = 0; j < numObservations[i]; i++) {
					delete [] observationTransitions[i][j];
				}
				delete [] observationTransitions[i];
			}
			delete [] observationTransitions;
		}

		if (stateTransitions != NULL) {
			for (int i = 0; i < numStates; i++) {
				for (int j = 0; j < numJointActions; j++) {
					delete [] stateTransitions[i][j];
				}
				delete [] stateTransitions[i];
			}
			delete [] stateTransitions;
		}

		if (numObservations != NULL) {
			delete [] numObservations;
		}

		if (numActions != NULL) {
			delete [] numActions;
		}

		reset();
	}

	/**
	 * The number of agents in the system.
	 */
	int numAgents;

	/**
	 * The number of states in the system.
	 */
	int numStates;

	/**
	 * An array of the number of actions available for each agent, indexed by [i]:
	 * - i is the agent index
	 */
	int* numActions;

	/**
	 * The total number of possible joint actions.
	 */
	int numJointActions;

	/**
	 * A three-dimensional array of state transition probabilities, indexed by [i][j][k]:
	 * - i is the current state index
	 * - j is the joint action index, after performing them in state i
	 * - k is the next state index, after taking joint actions j
	 * Equivalently, we may write T(i,j,k) = Pr(k|i,j).
	 */
	double*** stateTransitions;

	/**
	 * An array of the number of observations available for each agent, indexed by [i]:
	 * - i is the agent index
	 */
	int* numObservations;

	/**
	 * The total number of possible joint observations.
	 */
	int numJointObservations;

	/**
	 * A three-dimensional array of observation transition probabilities, indexed by [i][j][k]:
	 * - i is the joint action index, after performing them in the previous state
	 * - j is the next state index, after taking joint actions i
	 * - k is the next joint observation index
	 * Equivalently, we may write O(i,j,k) = Pr(k|i,j).
	 */
	double*** observationTransitions;

	/**
	 * A three-dimensional array of rewards, indexed by [i][j][k]:
	 * - i is the current state index
	 * - j is the joint action index, after performing them in state i
	 * - k is the next state index, after taking joint actions j
	 * Equivalently, we may write R(i,j,k).
	 */
	double*** rewards;

	/**
	 * The horizon. A non-positive horizon (often 0) denotes an infinite horizon.
	 */
	int horizon;

	/**
	 * The discount factor. For finite horizon, define gamma = 1. For infinite horizon,
	 * define gamma in [0,1).
	 */
	double gamma;

	/**
	 * An optional array of the agent's names or identifiers, indexed by [i]:
	 * - i is the agent index
	 */
	std::string* agents;

	/**
	 * An optional array of the state's names or identifiers, indexed by [i]:
	 * - i is the state index
	 */
	std::string* states;

	/**
	 * A optional array of agent actions, indexed by [i][j]:
	 * - i is the agent index
	 * - j is the action index
	 */
	std::string** actions;

	/**
	 * An optional array of the observation's names or identifiers, indexed by [i][j]:
	 * - i is the agent index
	 * - j is the observation index for agent i
	 */
	std::string** observations;

};
