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

#include <iostream>
#include <fstream>
#include <sstream>

#include "../../include/utilities/unified_file.h"
#include "../../include/utilities/log.h"

#include "../../include/core/actions/action_exception.h"
#include "../../include/core/observations/observation_exception.h"
#include "../../include/core/states/state_exception.h"


/**
 * The default constructor for a unified file.
 */
UnifiedFile::UnifiedFile()
{
	agents = nullptr;
	states = nullptr;
	actions = nullptr;
	observations = nullptr;
	stateTransitions = nullptr;
	observationTransitions = nullptr;
	rewards = nullptr;
	initialState = nullptr;
	horizon = nullptr;

	rewardValue = true;
	rows = 1;
	filename = "";
}

/**
 * The constructor which enables automatic loading of the specified MDP-like file.
 * @param path The filename of the MDP-like file to load.
 */
UnifiedFile::UnifiedFile(std::string path)
{
	load(path);
}

/**
 * The deconstructor for a unified file which cleans up the memory.
 */
UnifiedFile::~UnifiedFile()
{ }

/**
 * A function which loads any MDP-like file.
 * @param path The filename and relative path of the MDP-like file to load.
 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
 */
bool UnifiedFile::load(std::string path)
{
	reset();
	filename = path;

	// Open the file.
	std::ifstream file(filename);
	std::string line;

	// Two variables to help with loading proceeding lines into a particular variable,
	// e.g., actions and observations in their joint form, and T, R, or O in their
	// matrix representation.
	// 0 = null, 1 = factored states, 2 = joint actions, 3 = joint observations
	// 4 = T vector, 5 = T matrix
	// 6 = O vector, 7 = O matrix
	// 8 = R vector, 9 = R matrix
	int loading = 0;
	int loadingCounter = 0;

	// This is not necessarily the best way to do it, but these are used in loading
	// matrices or vectors for T, O, and R.
	void *loadingAlpha = nullptr;
	void *loadingBeta = nullptr;
	void *loadingGamma = nullptr;

	// If the file failed to open, then do not do anything.
	if (file.is_open()) {
		// Iterate over all lines in the file, and then each character in a line.
		FileObjectCategory category = FileObjectCategory::OBJECT_NONE;

		while (std::getline(file, line)) {
			// Handle comments by removing all characters down to and including a '#'.
			line = line.substr(0, line.find('#'));

			//line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

			// Skip over blank lines.
			if (line.length() == 0) {
				rows++;
				continue;
			}

			// Two cases, either this is a keyword line, or it is an information line.
			if (line.find(':') != std::string::npos) {
				// Reset these variables since we have started a new ":" statement.
				loading = 0;
				loadingCounter = 0;

				// If this contains a colon, then it is a keyword line. For keyword lines,
				// split the line based on the colon(s) and handle the keyword.
				std::istringstream ssLine(line);
				std::vector<std::string> items;
				std::string temp;

				while (std::getline(ssLine, temp, ':')) {
					items.push_back(temp);
				}

				// Handle the specific case.
				if (items[0].compare("horizon") == 0) {
					if (load_horizon(items)) {
						return true;
					}
				} else if (items[0].compare("discount") == 0) {
					if (load_discount_factor(items)) {
						return true;
					}
				} else if (items[0].compare("start") == 0) {
					if (load_initial_state(items)) {
						return true;
					}
				} else if (items[0].compare("start include") == 0) {
					if (load_initial_state_inclusive(items)) {
						return true;
					}
				} else if (items[0].compare("start exclude") == 0) {
					if (load_initial_state_exclusive(items)) {
						return true;
					}
				} else if (items[0].compare("values") == 0) {
					if (load_value(items)) {
						return true;
					}
				} else if (items[0].compare("agents") == 0) {
					if (load_agents(items)) {
						return true;
					}
				} else if (items[0].compare("states") == 0) {
					int result = load_states(items);
					if (result == -1) {
						return true;
					} else if (result == 1) {
						loading = 1;
						loadingCounter = 0;
					}
				} else if (items[0].compare("actions") == 0) {
					int result = load_actions(items);
					if (result == -1) {
						return true;
					} else if (result == 1) {
						loading = 2;
						loadingCounter = 0;
					}
				} else if (items[0].compare("observations") == 0) {
					int result = load_observations(items);
					if (result == -1) {
						return true;
					} else if (result == 1) {
						loading = 3;
						loadingCounter = 0;
					}
				} else if (items[0].compare("T") == 0) {

				} else if (items[0].compare("O") == 0) {

				} else if (items[0].compare("R") == 0) {

				}
			} else {
				// If this does not contain a colon (and is not blank), then it
				// is an information line. For information lines, split the line based on
				// spaces and read each float, creating the appropriate map.
				switch (loading) {
				case 1:
					// Loading factored states.
					if (load_factored_states(loadingCounter, line)) {
						return true;
					}
					break;
				case 2:
					// Loading an agent's actions.
					if (load_agent_actions(loadingCounter, line)) {
						return true;
					}
					break;
				case 3:
					// Loading an agent's observations.
					if (load_agent_observations(loadingCounter, line)) {
						return true;
					}
					break;
				case 4:
					// Loading a vector for T.
					break;
				case 5:
					// Loading a matrix for T.
					break;
				case 6:
					// Loading a vector for O.
					break;
				case 7:
					// Loading a matrix for O.
					break;
				case 8:
					// Loading a vector for R.
					break;
				case 9:
					// Loading a matrix for R.
					break;
				default:
					sprintf(error, "Failed loading a factor, vector, or matrix on line %i in file '%s'.",
							rows, filename.c_str());
					log_message(std::cout, "UnifiedFile::load", error);
					return true;
					break;
				}
			}

			// Check if this new line contains a ":" and the category exists. This means
			// that the category will switch, so we need to save the 'current' before it
			// does.
			if (category != FileObjectCategory::OBJECT_NONE && line.find(':') != std::string::npos) {
				switch (category) {
				case FileObjectCategory::OBJECT_STATE_TRANSITIONS:
					break;
				case FileObjectCategory::OBJECT_OBSERVATION_TRANSITIONS:
					break;
				case FileObjectCategory::OBJECT_REWARDS:
					break;
				default:
					break;
				}
			}
		}
	}

	return false;
}

/**
 * A function which saves the information as a MDP-like file.
 * @param path The filename and relative path of the MDP-like file to save.
 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
 */
bool UnifiedFile::save(std::string path)
{
	return false;
}

/**
 * Reset the internal objects in the unified file.
 */
void UnifiedFile::reset()
{
	if (agents != nullptr) {
		delete agents;
	}
	agents = nullptr;

	if (states != nullptr) {
		delete states;
	}
	states = nullptr;

	if (actions != nullptr) {
		delete actions;
	}
	actions = nullptr;

	if (observations != nullptr) {
		delete observations;
	}
	observations = nullptr;

	if (stateTransitions != nullptr) {
		delete stateTransitions;
	}
	stateTransitions = nullptr;

	if (observationTransitions != nullptr) {
		delete observationTransitions;
	}
	observationTransitions = nullptr;

	if (rewards != nullptr) {
		delete rewards;
	}
	rewards = nullptr;

	if (initialState != nullptr) {
		delete initialState;
	}
	initialState = nullptr;

	if (horizon != nullptr) {
		delete horizon;
	}
	horizon = nullptr;

	rewardValue = true;
	rows = 1;
	filename = "";
}

/**
 * Load the horizon object from the file's data.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_horizon(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing horizon definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_horizon", error);
		return true;
	}

	// Create the horizon object if it has not been made yet.
	if (horizon == nullptr) {
		horizon = new Horizon();
	}

	// Remove the whitespace.
	items[1].erase(std::remove(items[1].begin(), items[1].end(), ' '), items[1].end());

	// Attempt to convert the string to a double. If successful, then set the horizon.
	int h = 0.0;
	try {
		h = std::stoi(items[1]);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to an integer on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_horizon", error);
		return true;
	}

	if (h < 0) {
		sprintf(error, "Failed to use '%i', because it was negative, on line %i in file '%s'.",
				h, rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_horizon", error);
		return true;
	}

	// Set the horizon equal to the integer provided.
	horizon->set_horizon(h);

	return false;
}

/**
 * Load the discount factor from the file's data.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_discount_factor(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing discount factor definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_discount_factor", error);
		return true;
	}

	// Create the horizon object if it has not been made yet.
	if (horizon == nullptr) {
		horizon = new Horizon();
	}

	// Remove the whitespace.
	items[1].erase(std::remove(items[1].begin(), items[1].end(), ' '), items[1].end());

	// Attempt to convert the string to a double. If successful, then set the discount factor.
	double d = 0.0;
	try {
		d = std::stod(items[1]);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to a float on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_discount_factor", error);
		return true;
	}

	if (d <= 0.0 || d > 1.0) {
		sprintf(error, "Failed to use '%f', because it is not in (0, 1], on line %i in file '%s'.",
				d, rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_discount_factor", error);
		return true;
	}

	horizon->set_discount_factor(d);

	return false;
}


/**
 * Load the initial state from the file's data.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_initial_state(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState != nullptr) {
		initialState = new InitialState();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// Handle the various cases for setting an initial state.
	if (list.size() == 1) {
		// Either this is a single state, uniform, or number of states.
		if (list[0].compare("uniform") == 0) {
			double probability = 1.0 / (double)states->get_num_states();
			for (State *state : states->all()) {
				initialState->set_initial_belief(state, probability);
			}
		}

		// Attempt to convert this to an integer. If it fails, then it must be a state.
		int stateIndex = -1;
		try {
			stateIndex = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) { }

		if (stateIndex == -1) {
			State *state = nullptr;

			try {
				state = states->find(list[0]);
			} catch (const StateException &err) {
				sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
						list[0].c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state", error);
				return true;
			}

			initialState->set_initial_state(state);
		} else {
			// Handle the case in which the user provided an invalid state.
			if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
				sprintf(error, "State integer '%i' out of bounds on line %i in file '%s'.",
						stateIndex, rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state", error);
				return true;
			}

			initialState->set_initial_state(states->all()[stateIndex]);
			initialState->set_initial_belief(states->all()[stateIndex], 1.0);
		}
	} else {
		// This must be a full list of probabilities.
		int stateIndex = 0;
		for (std::string probability : list) {
			// Stop if too many probabilities are defined and return an error.
			if (stateIndex >= states->get_num_states()) {
				sprintf(error, "State integer '%i' out of bounds on line %i in file '%s'.",
						stateIndex, rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state", error);
				return true;
			}

			// Attempt to convert the string into a probability, raising an error as needed.
			double p = 0.0;
			try {
				p = std::stod(probability);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Failed to convert '%s' to a float on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state", error);
				return true;
			}

			initialState->set_initial_belief(states->all()[stateIndex], p);
			stateIndex++;
		}
	}

	return false;
}


/**
 * Load the initial state from the file's data, following the special inclusive structure.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_initial_state_inclusive(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start include', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState != nullptr) {
		initialState = new InitialState();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	if (list.size() == 0) {
		sprintf(error, "No states provided on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	// Check if the first item in the list is an integer. It must be if this is
	// a list of state indices.
	bool indices = true;
	try {
		std::stoi(list[0]);
	} catch (const std::invalid_argument &err) {
		indices = false;
	}

	double probability = 1.0 / (double)list.size();

	// If these are indices, then load them as such. Otherwise, load them as string identifiers.
	if (indices) {
		for (std::string indexString : list) {
			int stateIndex = -1;

			try {
				stateIndex = std::stoi(indexString);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Invalid state index string '%s' on line %i in file '%s'.",
						indexString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
				return true;
			}

			if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
				sprintf(error, "State index '%s' is out of bounds on line %i in file '%s'.",
						indexString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
				return true;
			}

			initialState->set_initial_belief(states->all()[stateIndex], probability);
		}
	} else {
		for (std::string idString : list) {
			State *state = nullptr;

			try {
				state = states->find(idString);
			} catch (const StateException &err) {
				sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
						idString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_inclusive", error);
				return true;
			}

			initialState->set_initial_belief(state, probability);
		}
	}

	return false;
}

/**
 * Load the initial state from the file's data, following the special exclusive structure.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_initial_state_exclusive(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start exclude', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState != nullptr) {
		initialState = new InitialState();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	if (list.size() == 0) {
		sprintf(error, "No states provided on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	// Check if the first item in the list is an integer. It must be if this is
	// a list of state indices.
	bool indices = true;
	try {
		std::stoi(list[0]);
	} catch (const std::invalid_argument &err) {
		indices = false;
	}

	double probability = 1.0 / (double)(states->get_num_states() - list.size());

	std::vector<State *> subset = states->all();

	// If these are indices, then remove them from the 'stateSubset' vector. Otherwise, remove them as
	// string identifiers.
	if (indices) {
		for (std::string indexString : list) {
			int stateIndex = -1;

			try {
				stateIndex = std::stoi(indexString);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Invalid state index string '%s' on line %i in file '%s'.",
						indexString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
				return true;
			}

			if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
				sprintf(error, "State index '%s' is out of bounds on line %i in file '%s'.",
						indexString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
				return true;
			}

			// Remove the state from the list.
			subset.erase(std::remove(subset.begin(), subset.end(), states->all()[stateIndex]), subset.end());
		}
	} else {
		for (std::string idString : list) {
			State *state = nullptr;

			try {
				state = states->find(idString);
			} catch (const StateException &err) {
				sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
						idString.c_str(), rows, filename.c_str());
				log_message(std::cout, "UnifiedFile::load_initial_state_exclusive", error);
				return true;
			}

			// Remove the state from the list.
			subset.erase(std::remove(subset.begin(), subset.end(), state), subset.end());
		}
	}

	// The remaining states must be defined with a uniform belief.
	for (State *state : subset) {
		initialState->set_initial_belief(state, probability);
	}

	return false;
}


/**
 * Load the value type (reward or cost) from the file's data.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_value(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_value", error);
		return true;
	}

	if (items[1].compare("reward") == 0) {
		rewardValue = true;
	} else if (items[1].compare("cost") == 0) {
		rewardValue = false;
	} else {
		sprintf(error, "String '%s' must be either 'reward' or 'cost' on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_value", error);
		return true;
	}

	return false;
}

/**
 * Load the agents from the file's data.
 * @param items	The list of items on the same line.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_agents(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_agents", error);
		return true;
	}

	// Create the agents object if it has not been made yet.
	if (agents == nullptr) {
		agents = new Agents();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two agents.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Agent count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agents", error);
			return true;
		}

		// There must be more than one agent.
		if (n < 2) {
			sprintf(error, "Agent count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agents", error);
			return true;
		}

		// Create a list of agents with names: 0, 1, ..., n - 1.
		char agentName[16];
		for (int i = 0; i < n; i++) {
			sprintf(agentName, "%i", i);
			agents->add(new Agent(agentName));
		}
	} else {
		// This must be a full list of unique agent names.
		for (std::string agentName : list) {
			agents->add(new Agent(agentName));
		}
	}

	return false;
}

/**
 * Load the states from the file's data.
 * @param items	The list of items on the same line.
 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
 * 		loading a sequence of factored states (not necessarily for each agent).
 */
int UnifiedFile::load_states(std::vector<std::string> items)
{
	// If the item is not defined, then it must be a sequence of factored states.
	if (items.size() < 2) {
		states = new FiniteFactoredStates();
		return 1;
	}

	// Create the states object if it has not been made yet.
	if (states == nullptr) {
		states = new FiniteStates();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two states.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "State count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_states", error);
			return -1;
		}

		// There must be more than one state.
		if (n < 2) {
			sprintf(error, "State count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_states", error);
			return -1;
		}

		// Create a list of states with names: 0, 1, ..., n - 1.
		char stateName[16];
		for (int i = 0; i < n; i++) {
			sprintf(stateName, "%i", i);
			states->add(new State(stateName));
		}
	} else {
		// This must be a full list of unique state names.
		for (std::string stateName : list) {
			states->add(new State(stateName));
		}
	}

	return 0;
}

/**
 * Load the factored states from the file's data.
 * @param factorIndex	The index of the factored state.
 * @param line			The line string from the file.
 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
 */
bool UnifiedFile::load_factored_states(int factorIndex, std::string line)
{
	// Handle an invalid factor index and undefined states variable.
	if (factorIndex < 0 || factorIndex >= ((FiniteFactoredStates *)states)->get_num_factors()) {
		sprintf(error, "Missing states definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_factored_states", error);
		return -1;
	}

	if (states == nullptr) {
		sprintf(error, "States object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_factored_states", error);
		return -1;
	}

	// Split the line by spaces, skipping over the empty strings.
	std::istringstream ssItem(line);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two states.
	std::vector<State *> newStates;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "State count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_factored_states", error);
			return -1;
		}

		// There must be more than one state.
		if (n < 2) {
			sprintf(error, "State count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_factored_states", error);
			return -1;
		}

		// Create a list of states with names: 0, 1, ..., n - 1.
		newStates.reserve(n);

		char stateName[16];
		for (int i = 0; i < n; i++) {
			sprintf(stateName, "%i", i);
			newStates.push_back(new State(stateName));
		}
	} else {
		// This must be a full list of unique state names.
		std::vector<State *> newStates;
		for (std::string stateName : list) {
			newStates.push_back(new State(stateName));
		}
	}

	((FiniteFactoredStates *)states)->set(factorIndex, newStates);
	((FiniteFactoredStates *)states)->update();

	return 0;
}

/**
 * Load the actions from the file's data.
 * @param items	The list of items on the same line.
 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
 * 		loading a sequence of actions for each agent.
 */
int UnifiedFile::load_actions(std::vector<std::string> items)
{
	// If agents have been defined, then this was successful (so far) and we must load actions
	// for each agent in the proceeding lines.
	if (agents != nullptr) {
		actions = new FiniteJointActions(agents->get_num_agents());
		return 1;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing actions definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_actions", error);
		return -1;
	}

	// Create the actions object if it has not been made yet.
	if (actions == nullptr) {
		actions = new FiniteActions();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two agents.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Action count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_actions", error);
			return -1;
		}

		// There must be more than one action.
		if (n < 2) {
			sprintf(error, "Action count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_actions", error);
			return -1;
		}

		// Create a list of actions with names: 0, 1, ..., n - 1.
		char actionName[16];
		for (int i = 0; i < n; i++) {
			sprintf(actionName, "%i", i);
			actions->add(new Action(actionName));
		}
	} else {
		// This must be a full list of unique action names.
		for (std::string actionName : list) {
			actions->add(new Action(actionName));
		}
	}

	return 0;
}

/**
 * Load the actions of one agent from the file's data.
 * @param agentIndex	The index of the agent in the joint action.
 * @param line			The line string from the file.
 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
 * 		loading a sequence of actions.
 */
int UnifiedFile::load_agent_actions(int agentIndex, std::string line)
{
	// Handle an invalid agent index and undefined agents variable.
	if (agents == nullptr || agentIndex < 0 || agentIndex >= agents->get_num_agents()) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_agent_actions", error);
		return -1;
	}

	if (actions == nullptr) {
		sprintf(error, "Actions object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_agent_actions", error);
		return -1;
	}

	// Split the line by spaces, skipping over the empty strings.
	std::istringstream ssItem(line);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two actions.
	std::vector<Action *> newActions;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Action count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agent_actions", error);
			return -1;
		}

		// There must be more than one action.
		if (n < 2) {
			sprintf(error, "Action count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agent_actions", error);
			return -1;
		}

		// Create a list of actions with names: 0, 1, ..., n - 1.
		newActions.reserve(n);

		char actionName[16];
		for (int i = 0; i < n; i++) {
			sprintf(actionName, "%i", i);
			newActions.push_back(new Action(actionName));
		}
	} else {
		// This must be a full list of unique action names.
		std::vector<Action *> newActions;
		for (std::string actionName : list) {
			newActions.push_back(new Action(actionName));
		}
	}

	((FiniteJointActions *)actions)->set(agentIndex, newActions);
	((FiniteJointActions *)actions)->update();

	return 0;
}

/**
 * Load the observations from the file's data.
 * @param items	The list of items on the same line.
 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
 * 		loading a sequence of observations for each agent.
 */
int UnifiedFile::load_observations(std::vector<std::string> items)
{
	// If agents have been defined, then this was successful (so far) and we must load observations
	// for each agent in the proceeding lines.
	if (agents != nullptr) {
		observations = new FiniteJointObservations(agents->get_num_agents());
		return 1;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing observations definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_observations", error);
		return -1;
	}

	// Create the observations object if it has not been made yet.
	if (observations == nullptr) {
		observations = new FiniteObservations();
	}

	// Split the first item by spaces, skipping over the empty strings.
	std::istringstream ssItem(items[1]);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two observations.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Observation count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_observations", error);
			return -1;
		}

		// There must be more than one observation.
		if (n < 2) {
			sprintf(error, "Observation count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_observations", error);
			return -1;
		}

		// Create a list of observations with names: 0, 1, ..., n - 1.
		char observationName[16];
		for (int i = 0; i < n; i++) {
			sprintf(observationName, "%i", i);
			observations->add(new Observation(observationName));
		}
	} else {
		// This must be a full list of unique observation names.
		for (std::string observationName : list) {
			observations->add(new Observation(observationName));
		}
	}

	return 0;
}

/**
 * Load the observations of one agent from the file's data.
 * @param agentIndex	The index of the agent in the joint observation.
 * @param line			The line string from the file.
 * @return Return -1 if an error occurred, 0 if successful, and 1 if this begins
 * 		loading a sequence of actions.
 */
int UnifiedFile::load_agent_observations(int agentIndex, std::string line)
{
	// Handle an invalid agent index and undefined agents variable.
	if (agents == nullptr || agentIndex < 0 || agentIndex >= agents->get_num_agents()) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_agent_observations", error);
		return -1;
	}

	if (observations == nullptr) {
		sprintf(error, "Observations object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message(std::cout, "UnifiedFile::load_agent_observations", error);
		return -1;
	}

	// Split the line by spaces, skipping over the empty strings.
	std::istringstream ssItem(line);
	std::vector<std::string> list;
	std::string temp;

	while (std::getline(ssItem, temp, ' ')) {
		if (temp.length() == 0) {
			continue;
		}
		list.push_back(temp);
	}

	// If this is one item, then it must be a number, since there must be at least two observations.
	std::vector<Observation *> newObservations;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Observation count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agent_observations", error);
			return -1;
		}

		// There must be more than one observation.
		if (n < 2) {
			sprintf(error, "Observation count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message(std::cout, "UnifiedFile::load_agent_observations", error);
			return -1;
		}

		// Create a list of observations with names: 0, 1, ..., n - 1.
		newObservations.reserve(n);

		char observationName[16];
		for (int i = 0; i < n; i++) {
			sprintf(observationName, "%i", i);
			newObservations.push_back(new Observation(observationName));
		}
	} else {
		// This must be a full list of unique observation names.
		std::vector<Observation *> newObservations;
		for (std::string observationName : list) {
			newObservations.push_back(new Observation(observationName));
		}
	}

	((FiniteJointObservations *)observations)->set(agentIndex, newObservations);
	((FiniteJointObservations *)observations)->update();

	return 0;
}
