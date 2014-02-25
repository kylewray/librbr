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


#ifndef UNIFIED_FILE_H
#define UNIFIED_FILE_H


#include <string>
#include <vector>

#include "../core/agents/agents.h"
#include "../core/states/finite_states.h"
#include "../core/states/finite_factored_states.h"
#include "../core/actions/finite_actions.h"
#include "../core/actions/finite_joint_actions.h"
#include "../core/observations/finite_observations.h"
#include "../core/observations/finite_joint_observations.h"
#include "../core/state_transitions/finite_state_transitions.h"
#include "../core/observation_transitions/finite_observation_transitions.h"
#include "../core/rewards/sas_rewards.h"
#include "../core/initial_state.h"
#include "../core/horizon.h"

#include "../mdp/mdp.h"
#include "../pomdp/pomdp.h"
#include "../dec_mdp/dec_mdp.h"
#include "../dec_pomdp/dec_pomdp.h"

#include "../utilities/string_manipulation.h"

/**
 * A file loading and saving class called UnifiedFile which acts as an intermediate
 * representation for any MDP-like object. It can load any *.mdp, *.pomdp, *.decmdp,
 * or *.decpomdp file and store all variables found within the file. Other class
 * objects such as MDP or Dec-POMDP may then use a UnifiedFile object to copy all
 * their required internal variables, or even just a subset of the variables.
 */
class UnifiedFile {
public:
	/**
	 * The default constructor for a unified file.
	 */
	UnifiedFile();

	/**
	 * The constructor which enables automatic loading of the specified MDP-like file.
	 * @param path The filename of the MDP-like file to load.
	 */
	UnifiedFile(std::string path);

	/**
	 * The deconstructor for a unified file which cleans up the memory.
	 */
	~UnifiedFile();

	/**
	 * A function which loads any MDP-like file.
	 * @param path The filename and relative path of the MDP-like file to load.
	 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load(std::string path);

	/**
	 * A function which saves the information as a MDP-like file.
	 * @param path The filename and relative path of the MDP-like file to save.
	 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool save(std::string path);

	/**
	 * Reset the internal objects in the unified file.
	 */
	void reset();

	/**
	 * Get an MDP version of a loaded file. This releases control of the memory to the
	 * MDP object, and therefore removes pointers to any loaded file information.
	 * @return An MDP defined by the file loaded.
	 * @throws CoreException The MDP was missing a vital component to be defined.
	 */
	MDP *get_mdp();

	/**
	 * Get an POMDP version of a loaded file. This releases control of the memory to the
	 * MDP object, and therefore removes pointers to any loaded file information.
	 * @return A POMDP defined by the file loaded.
	 * @throws CoreException The MDP was missing a vital component to be defined.
	 */
	POMDP *get_pomdp();

	/**
	 * Get an Dec-MDP version of a loaded file. This releases control of the memory to the
	 * MDP object, and therefore removes pointers to any loaded file information.
	 * @return A Dec-MDP defined by the file loaded.
	 * @throws CoreException The MDP was missing a vital component to be defined.
	 */
	DecMDP *get_dec_mdp();

	/**
	 * Get an Dec-POMDP version of a loaded file. This releases control of the memory to the
	 * MDP object, and therefore removes pointers to any loaded file information.
	 * @return A Dec-POMDP defined by the file loaded.
	 * @throws CoreException The MDP was missing a vital component to be defined.
	 */
	DecPOMDP *get_dec_pomdp();

private:
	/**
	 * Load the horizon from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_horizon(std::vector<std::string> items);

	/**
	 * Load the discount factor from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_discount_factor(std::vector<std::string> items);

	/**
	 * Load the initial state from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_initial_state(std::vector<std::string> items);

	/**
	 * Load the initial state from the file's data, following the special inclusive structure.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_initial_state_inclusive(std::vector<std::string> items);

	/**
	 * Load the initial state from the file's data, following the special exclusive structure.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_initial_state_exclusive(std::vector<std::string> items);

	/**
	 * Load the value type (reward or cost) from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_value(std::vector<std::string> items);

	/**
	 * Load the agents from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_agents(std::vector<std::string> items);

	/**
	 * Load the states from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a sequence of factored states (not necessarily for each agent).
	 */
	int load_states(std::vector<std::string> items);

	/**
	 * Load the factored states from the file's data.
	 * @param factorIndex	The index of the factored state.
	 * @param line			The line string from the file.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	bool load_factored_states(int factorIndex, std::string line);

	/**
	 * Load the actions from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a sequence of actions for each agent.
	 */
	int load_actions(std::vector<std::string> items);

	/**
	 * Load the actions of one agent from the file's data.
	 * @param agentIndex	The index of the agent in the joint action.
	 * @param line			The line string from the file.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a sequence of actions.
	 */
	int load_agent_actions(int agentIndex, std::string line);

	/**
	 * Load the observations from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a sequence of observations for each agent.
	 */
	int load_observations(std::vector<std::string> items);

	/**
	 * Load the observations of one agent from the file's data.
	 * @param agentIndex	The index of the agent in the joint observation.
	 * @param line			The line string from the file.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a sequence of actions.
	 */
	int load_agent_observations(int agentIndex, std::string line);

	/**
	 * Load the state transitions from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, 1 if this begins
	 * 		loading a vector of state transitions, 2 if this begins loading a
	 * 		matrix of state transitions.
	 */
	int load_state_transition(std::vector<std::string> items);

	/**
	 * Load a state transition vector from the file's data.
	 * @param line		The line to parse containing a vector of probabilities.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_state_transition_vector(std::string line);

	/**
	 * Load a state transition matrix from the file's data.
	 * @param stateIndex	The current state index for the start state.
	 * @param line			The line to parse containing a vector of probabilities.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_state_transition_matrix(int stateIndex, std::string line);

	/**
	 * Load the observation transitions from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a matrix of observation transitions.
	 */
	int load_observation_transition(std::vector<std::string> items);

	/**
	 * Load a observation transition vector from the file's data.
	 * @param line		The line to parse containing a vector of probabilities.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_observation_transition_vector(std::string line);

	/**
	 * Load a state transition matrix from the file's data.
	 * @param stateIndex	The current state index for the end state.
	 * @param line			The line to parse containing a vector of probabilities.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_observation_transition_matrix(int stateIndex, std::string line);

	/**
	 * Load the rewards from the file's data.
	 * @param items	The list of items on the same line.
	 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
	 * 		loading a matrix of rewards.
	 */
	int load_reward(std::vector<std::string> items);

	/**
	 * Load a reward vector from the file's data.
	 * @param line		The line to parse containing a vector of rewards or costs.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_reward_vector(std::string line);

	/**
	 * Load a reward matrix from the file's data.
	 * @param stateIndex	The current state index for the end state.
	 * @param line			The line to parse containing a vector of rewards or costs.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load_reward_matrix(int stateIndex, std::string line);

	/**
	 * Release control over the memory of the variables.
	 */
	void release();

	/**
	 * The agents in the MDP-like object; e.g., a vector of strings.
	 */
	Agents *agents;

	/**
	 * The states in the MDP-like object; e.g., factored vectors of strings.
	 */
	FiniteStates *states;

	/**
	 * The actions in the MDP-like object; e.g., factored vectors of strings.
	 */
	FiniteActions *actions;

	/**
	 * The observations in the MDP-like object; e.g., factored vectors of strings.
	 */
	FiniteObservations *observations;

	/**
	 * The state transition function in the MDP-like object; e.g., a three-dimensional array mapping to a double.
	 */
	FiniteStateTransitions *stateTransitions;

	/**
	 * The observation transition function in the MDP-like object; e.g., a three-dimensional array mapping to a double.
	 */
	FiniteObservationTransitions *observationTransitions;

	/**
	 * The reward function in the SCP; e.g., a two-dimensional array mapping to a double.
	 */
	SASRewards *rewards;

	/**
	 * The initial state or initial belief state; e.g., factored initial state.
	 */
	InitialState *initialState;

	/**
	 * The horizon, either a finite time or a discount factor.
	 */
	Horizon *horizon;

	/**
	 * If this represents a reward or a cost.
	 */
	bool rewardValue;

	/**
	 * The number of rows in the file (primarily for error output purposes).
	 */
	int rows;

	/**
	 * The name of the file (primarily for error output purposes).
	 */
	std::string filename;

	/**
	 * A variable which holds the error message.
	 */
	char error[1024];

	/**
	 * A helper action variable for loading vectors or matrices.
	 */
	Action *loadingAction;

	/**
	 * A helper state variable for loading vectors or matrices.
	 */
	State *loadingState;

	/**
	 * A helper observation variable for loading vectors or matrices.
	 */
	Observation *loadingObservation;

};


#endif // UNIFIED_FILE_H
