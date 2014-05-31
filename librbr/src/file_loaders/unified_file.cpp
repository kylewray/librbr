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


#include <iostream>
#include <fstream>

#include "../../include/file_loaders/unified_file.h"

#include "../../include/utilities/log.h"

#include "../../include/core/core_exception.h"
#include "../../include/core/agents/agent_exception.h"
#include "../../include/core/states/state_exception.h"
#include "../../include/core/actions/action_exception.h"
#include "../../include/core/observations/observation_exception.h"
#include "../../include/core/state_transitions/state_transition_exception.h"
#include "../../include/core/observation_transitions/observation_transition_exception.h"
#include "../../include/core/rewards/reward_exception.h"

#include "../../include/core/states/named_state.h"
#include "../../include/core/states/state_utilities.h"

#include "../../include/core/actions/named_action.h"
#include "../../include/core/actions/action_utilities.h"

#include "../../include/core/observations/named_observation.h"
#include "../../include/core/observations/observation_utilities.h"

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

	loadingAction = nullptr;
	loadingState = nullptr;
	loadingObservation = nullptr;
}

UnifiedFile::UnifiedFile(std::string path)
{
	load(path);
}

UnifiedFile::~UnifiedFile()
{ }

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

	// If the file failed to open, then do not do anything.
	if (!file.is_open()) {
		sprintf(error, "Failed to find file '%s'.", filename.c_str());
		log_message("UnifiedFile::load", error);
		return true;
	}

	while (std::getline(file, line)) {
		// Handle comments by removing all characters down to and including a '#'.
		line = line.substr(0, line.find('#'));

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

			loadingAction = nullptr;
			loadingState = nullptr;
			loadingObservation = nullptr;

			// If this contains a colon, then it is a keyword line. For keyword lines,
			// split the line based on the colon(s) and handle the keyword.
			std::vector<std::string> items = split_string_by_colon(line);

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
				int result = load_state_transition(items);
				if (result == -1) {
					return true;
				} else if (result == 1) {
					loading = 4;
					loadingCounter = 0;
				} else if (result == 2) {
					loading = 5;
					loadingCounter = 0;
				}
			} else if (items[0].compare("O") == 0) {
				int result = load_observation_transition(items);
				if (result == -1) {
					return true;
				} else if (result == 1) {
					loading = 6;
					loadingCounter = 0;
				} else if (result == 2) {
					loading = 7;
					loadingCounter = 0;
				}
			} else if (items[0].compare("R") == 0) {
				int result = load_reward(items);
				if (result == -1) {
					return true;
				} else if (result == 1) {
					loading = 8;
					loadingCounter = 0;
				} else if (result == 2) {
					loading = 9;
					loadingCounter = 0;
				}
			}
		} else {
			// If this does not contain a colon (and is not blank), then it
			// is an information line. For information lines, split the line based on
			// spaces and read each double, creating the appropriate map.
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
				if (load_state_transition_vector(line)) {
					return true;
				}
				break;
			case 5:
				// Loading a matrix for T.
				if (load_state_transition_matrix(loadingCounter, line)) {
					return true;
				}
				break;
			case 6:
				// Loading a vector for O.
				if (load_observation_transition_vector(line)) {
					return true;
				}
				break;
			case 7:
				// Loading a matrix for O.
				if (load_observation_transition_matrix(loadingCounter, line)) {
					return true;
				}
				break;
			case 8:
				// Loading a vector for R.
				if (load_reward_vector(line)) {
					return true;
				}
				break;
			case 9:
				// Loading a matrix for R.
				if (load_reward_matrix(loadingCounter, line)) {
					return true;
				}
				break;
			default:
				sprintf(error, "Failed loading a factor, vector, or matrix on line %i in file '%s'.",
						rows, filename.c_str());
				log_message("UnifiedFile::load", error);
				return true;
				break;
			}

			loadingCounter++;
		}

		rows++;
	}

	return false;
}

bool UnifiedFile::save(std::string path)
{
	return false;
}

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

	loadingState = nullptr;
	loadingAction = nullptr;
	loadingObservation = nullptr;

	orderedStates.clear();
	orderedObservations.clear();
}

bool UnifiedFile::load_horizon(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing horizon definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_horizon", error);
		return true;
	}

	// Create the horizon object if it has not been made yet.
	if (horizon == nullptr) {
		horizon = new Horizon();
	}

	remove_whitespace(items[1]);

	// Handle the infinite horizon case with the keyword 'infinite'.
	if (items[1].compare("infinite") == 0) {
		horizon->set_horizon(0);
		return false;
	}

	// Attempt to convert the string to a double. If successful, then set the horizon.
	int h = 0.0;
	try {
		h = std::stoi(items[1]);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to an integer on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_horizon", error);
		return true;
	}

	if (h < 0) {
		sprintf(error, "Failed to use '%i', because it was negative, on line %i in file '%s'.",
				h, rows, filename.c_str());
		log_message("UnifiedFile::load_horizon", error);
		return true;
	}

	// Set the horizon equal to the integer provided.
	horizon->set_horizon(h);

	return false;
}

bool UnifiedFile::load_discount_factor(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing discount factor definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_discount_factor", error);
		return true;
	}

	// Create the horizon object if it has not been made yet.
	if (horizon == nullptr) {
		horizon = new Horizon();
	}

	remove_whitespace(items[1]);

	// Attempt to convert the string to a double. If successful, then set the discount factor.
	double d = 0.0;
	try {
		d = std::stod(items[1]);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_discount_factor", error);
		return true;
	}

	if (d <= 0.0 || d > 1.0) {
		sprintf(error, "Failed to use '%f', because it is not in (0, 1], on line %i in file '%s'.",
				d, rows, filename.c_str());
		log_message("UnifiedFile::load_discount_factor", error);
		return true;
	}

	horizon->set_discount_factor(d);

	return false;
}

bool UnifiedFile::load_initial_state(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState == nullptr) {
		initialState = new Initial();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	// Handle the various cases for setting an initial state.
	// Either this is a single state, uniform, or number of states.
	if (list[0].compare("uniform") == 0) {
		double probability = 1.0 / (double)states->get_num_states();
		for (auto state : *states) {
			initialState->set_initial_belief(resolve(state), probability);
		}
	} else if (list.size() == 1) {
		const State *state = nullptr;

		try {
			state = find_state(states, list[0]);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_initial_state", error);
			return true;
		}

		initialState->set_initial_state(state);
		initialState->set_initial_belief(state, 1.0);
	} else {
		// This must be a full list of probabilities.
		int stateIndex = 0;
		for (std::string probability : list) {
			// Stop if too many probabilities are defined and return an error.
			if (stateIndex >= states->get_num_states()) {
				sprintf(error, "State integer '%i' out of bounds on line %i in file '%s'.",
						stateIndex, rows, filename.c_str());
				log_message("UnifiedFile::load_initial_state", error);
				return true;
			}

			// Attempt to convert the string into a probability, raising an error as needed.
			double p = 0.0;
			try {
				p = std::stod(probability);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message("UnifiedFile::load_initial_state", error);
				return true;
			}

			initialState->set_initial_belief(orderedStates[stateIndex], p);
			stateIndex++;
		}
	}

	return false;
}

bool UnifiedFile::load_initial_state_inclusive(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start include', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState == nullptr) {
		initialState = new Initial();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	if (list.size() == 0) {
		sprintf(error, "No states provided on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_inclusive", error);
		return true;
	}

	double probability = 1.0 / (double)list.size();

	for (std::string idString : list) {
		const State *state = nullptr;

		try {
			state = find_state(states, idString);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					idString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_initial_state_inclusive", error);
			return true;
		}

		initialState->set_initial_belief(state, probability);
	}

	return false;
}

bool UnifiedFile::load_initial_state_exclusive(std::vector<std::string> items)
{
	// Ensure states are defined, otherwise you cannot specify an initial distribution over states.
	if (states == nullptr) {
		sprintf(error, "Failed to define 'start exclude', since states are undefined, on line %i in file '%s'.",
				rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	// If the initial state is not yet defined, then create it.
	if (initialState == nullptr) {
		initialState = new Initial();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	if (list.size() == 0) {
		sprintf(error, "No states provided on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_initial_state_exclusive", error);
		return true;
	}

	double probability = 1.0 / (double)(states->get_num_states() - list.size());

	std::vector<const State *> subset;

	for (std::string idString : list) {
		const State *state = nullptr;

		try {
			state = find_state(states, idString);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					idString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_initial_state_exclusive", error);
			return true;
		}

		// Add the state to the subset list.
		subset.push_back(state);
	}

	// The remaining states must be defined with a uniform belief.
	for (auto s : *states) {
		const State *state = resolve(s);
		if (std::find(subset.begin(), subset.end(), state) == subset.end()) {
			initialState->set_initial_belief(state, probability);
		}
	}

	return false;
}

bool UnifiedFile::load_value(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing state definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_value", error);
		return true;
	}

	remove_whitespace(items[1]);

	if (items[1].compare("reward") == 0) {
		rewardValue = true;
	} else if (items[1].compare("cost") == 0) {
		rewardValue = false;
	} else {
		sprintf(error, "String '%s' must be either 'reward' or 'cost' on line %i in file '%s'.",
				items[1].c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_value", error);
		return true;
	}

	return false;
}

bool UnifiedFile::load_agents(std::vector<std::string> items)
{
	// Raise an error if the item is not defined.
	if (items.size() < 2) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_agents", error);
		return true;
	}

	// Create the agents object if it has not been made yet.
	if (agents == nullptr) {
		agents = new Agents();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	// If this is one item, then it must be a number, since there must be at least two agents.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Agent count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agents", error);
			return true;
		}

		// There must be more than one agent.
		if (n < 2) {
			sprintf(error, "Agent count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agents", error);
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

	std::vector<std::string> list = split_string_by_space(items[1]);

	// If this is one item, then it must be a number, since there must be at least two states.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "State count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_states", error);
			return -1;
		}

		// There must be more than one state.
		if (n < 2) {
			sprintf(error, "State count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_states", error);
			return -1;
		}

		// Create a list of states with names: 0, 1, ..., n - 1.
		char stateName[16];
		for (int i = 0; i < n; i++) {
			sprintf(stateName, "%i", i);
			const State *newState = new NamedState(stateName);
			states->add(newState);
			orderedStates.push_back(newState);
		}
	} else {
		// This must be a full list of unique state names.
		for (std::string stateName : list) {
			const State *newState = new NamedState(stateName);
			states->add(newState);
			orderedStates.push_back(newState);
		}
	}

	return 0;
}

bool UnifiedFile::load_factored_states(int factorIndex, std::string line)
{
	// Handle an invalid factor index and undefined states variable.
	if (factorIndex < 0) { // || factorIndex >= ((FiniteFactoredStates *)states)->get_num_factors()) {
		sprintf(error, "Missing states definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_factored_states", error);
		return -1;
	}

	if (states == nullptr) {
		sprintf(error, "States object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_factored_states", error);
		return -1;
	}

	std::vector<std::string> list = split_string_by_space(line);

	// If this is one item, then it must be a number, since there must be at least two states.
	std::vector<const State *> newStates;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "State count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_factored_states", error);
			return -1;
		}

		// There must be more than one state.
		if (n < 2) {
			sprintf(error, "State count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_factored_states", error);
			return -1;
		}

		// Create a list of states with names: 0, 1, ..., n - 1.
		newStates.reserve(n);

		char stateName[16];
		for (int i = 0; i < n; i++) {
			sprintf(stateName, "%i", i);
			newStates.push_back(new NamedState(stateName));
		}
	} else {
		// This must be a full list of unique state names.
		for (std::string stateName : list) {
			newStates.push_back(new NamedState(stateName));
		}
	}

	if (factorIndex >= ((FiniteFactoredStates *)states)->get_num_factors()) {
		((FiniteFactoredStates *)states)->add_factor(newStates);
	} else {
		((FiniteFactoredStates *)states)->set(factorIndex, newStates);
	}
	((FiniteFactoredStates *)states)->update();

	// After the update, the internal states have been reset, so we need to remake
	// the ordered list of states.
	orderedStates.clear();
	for (auto s : *states) {
		orderedStates.push_back(resolve(s));
	}

	return 0;
}

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
		log_message("UnifiedFile::load_actions", error);
		return -1;
	}

	// Create the actions object if it has not been made yet.
	if (actions == nullptr) {
		actions = new FiniteActions();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	// If this is one item, then it must be a number, since there must be at least two agents.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Action count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_actions", error);
			return -1;
		}

		// There must be more than one action.
		if (n < 2) {
			sprintf(error, "Action count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_actions", error);
			return -1;
		}

		// Create a list of actions with names: 0, 1, ..., n - 1.
		char actionName[16];
		for (int i = 0; i < n; i++) {
			sprintf(actionName, "%i", i);
			actions->add(new NamedAction(actionName));
		}
	} else {
		// This must be a full list of unique action names.
		for (std::string actionName : list) {
			actions->add(new NamedAction(actionName));
		}
	}

	return 0;
}

int UnifiedFile::load_agent_actions(int agentIndex, std::string line)
{
	// Handle an invalid agent index and undefined agents variable.
	if (agents == nullptr || agentIndex < 0 || agentIndex >= agents->get_num_agents()) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_agent_actions", error);
		return -1;
	}

	if (actions == nullptr) {
		sprintf(error, "Actions object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_agent_actions", error);
		return -1;
	}

	std::vector<std::string> list = split_string_by_space(line);

	// If this is one item, then it must be a number, since there must be at least two actions.
	std::vector<const Action *> newActions;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Action count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agent_actions", error);
			return -1;
		}

		// There must be more than one action.
		if (n < 2) {
			sprintf(error, "Action count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agent_actions", error);
			return -1;
		}

		// Create a list of actions with names: 0, 1, ..., n - 1.
		newActions.reserve(n);

		char actionName[16];
		for (int i = 0; i < n; i++) {
			sprintf(actionName, "%i", i);
			newActions.push_back(new NamedAction(actionName));
		}
	} else {
		// This must be a full list of unique action names.
		for (std::string actionName : list) {
			newActions.push_back(new NamedAction(actionName));
		}
	}

	// Set the action index, and ignore any errors on an update. An error means that some action
	// sets for other agents are not yet defined.
	((FiniteJointActions *)actions)->set(agentIndex, newActions);
	try {
		((FiniteJointActions *)actions)->update();
	} catch (const ActionException &err) { }

	return 0;
}

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
		log_message("UnifiedFile::load_observations", error);
		return -1;
	}

	// Create the observations object if it has not been made yet.
	if (observations == nullptr) {
		observations = new FiniteObservations();
	}

	std::vector<std::string> list = split_string_by_space(items[1]);

	// If this is one item, then it must be a number, since there must be at least two observations.
	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Observation count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observations", error);
			return -1;
		}

		// There must be more than one observation.
		if (n < 2) {
			sprintf(error, "Observation count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observations", error);
			return -1;
		}

		// Create a list of observations with names: 0, 1, ..., n - 1.
		char observationName[16];
		for (int i = 0; i < n; i++) {
			sprintf(observationName, "%i", i);
			const Observation *newObservation = new NamedObservation(observationName);
			observations->add(newObservation);
			orderedObservations.push_back(newObservation);
		}
	} else {
		// This must be a full list of unique observation names.
		for (std::string observationName : list) {
			const Observation *newObservation = new NamedObservation(observationName);
			observations->add(newObservation);
			orderedObservations.push_back(newObservation);
		}
	}

	return 0;
}

int UnifiedFile::load_agent_observations(int agentIndex, std::string line)
{
	// Handle an invalid agent index and undefined agents variable.
	if (agents == nullptr || agentIndex < 0 || agentIndex >= agents->get_num_agents()) {
		sprintf(error, "Missing agents definition on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_agent_observations", error);
		return -1;
	}

	if (observations == nullptr) {
		sprintf(error, "Observations object is not defined on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_agent_observations", error);
		return -1;
	}

	std::vector<std::string> list = split_string_by_space(line);

	// If this is one item, then it must be a number, since there must be at least two observations.
	std::vector<const Observation *> newObservations;

	if (list.size() == 1) {
		// Attempt to convert this to an integer.
		int n = -1;
		try {
			n = std::stoi(list[0]);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Observation count '%s' is not a valid number on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agent_observations", error);
			return -1;
		}

		// There must be more than one observation.
		if (n < 2) {
			sprintf(error, "Observation count '%s' must be greater than 1 on line %i in file '%s'.",
					list[0].c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_agent_observations", error);
			return -1;
		}

		// Create a list of observations with names: 0, 1, ..., n - 1.
		newObservations.reserve(n);

		char observationName[16];
		for (int i = 0; i < n; i++) {
			sprintf(observationName, "%i", i);
			newObservations.push_back(new NamedObservation(observationName));
		}
	} else {
		// This must be a full list of unique observation names.
		for (std::string observationName : list) {
			newObservations.push_back(new NamedObservation(observationName));
		}
	}

	// Set the observation, and ignore any errors on an update. An error means that some observation
	// sets for other agents are not yet defined.
	((FiniteJointObservations *)observations)->set(agentIndex, newObservations);
	try {
		((FiniteJointObservations *)observations)->update();
	} catch (const ObservationException &error) { }

	// After the update, the internal observations have been reset, so we need to remake
	// the ordered list of observations.
	orderedObservations.clear();
	for (auto z : *observations) {
		orderedObservations.push_back(resolve(z));
	}

	return 0;
}

int UnifiedFile::load_state_transition(std::vector<std::string> items)
{
	// Ensure a valid number of items.
	if (items.size() < 2 || items.size() > 5) {
		sprintf(error, "Incomplete statement on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition", error);
		return -1;
	}

	// Create the state transitions object if it has not been made yet.
	if (stateTransitions == nullptr) {
		stateTransitions = new FiniteStateTransitions();
	}

	std::string actionName = split_string_by_space(items[1])[0];
	const Action *action = nullptr;

	if (actionName.compare("*") != 0) {
		try {
			action = find_action(actions, actionName);
		} catch (const ActionException &err) {
			sprintf(error, "Action '%s' has not been defined on line %i in file '%s'.",
					actionName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition", error);
			return -1;
		}
	}

	// If this is of the form "T: action :" then a matrix follows.
	if (items.size() == 2) {
		loadingAction = action;
		return 2;
	}

	std::string startStateName = split_string_by_space(items[2])[0];
	const State *startState = nullptr;

	if (startStateName.compare("*") != 0) {
		try {
			startState = find_state(states, startStateName);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					startStateName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition", error);
			return -1;
		}
	}

	// If this is of the form "T: action : start-state :" then a vector follows.
	if (items.size() == 3) {
		loadingAction = action;
		loadingState = startState;
		return 1;
	}

	std::string endStateName = split_string_by_space(items[3])[0];
	const State *endState = nullptr;

	if (endStateName.compare("*") != 0) {
		try {
			endState = find_state(states, endStateName);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					endStateName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition", error);
			return -1;
		}
	}

	std::string probabilityString = split_string_by_space(items[4])[0];
	double probability = 0.0;

	try {
		probability = std::stod(probabilityString);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
				probabilityString.c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition", error);
		return -1;
	}

	if (probability < 0.0 || probability > 1.0) {
		sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
				probabilityString.c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition", error);
		return true;
	}

	stateTransitions->set(startState, action, endState, probability);

	return 0;
}

bool UnifiedFile::load_state_transition_vector(std::string line)
{
	// Split the line into a list of (hopefully) probabilities equal to the number of states.
	std::vector<std::string> list = split_string_by_space(line);

	// Handle the special case of "uniform".
	if (list[0].compare("uniform") == 0) {
		double probability = 1.0 / (double)states->get_num_states();
		for (int i = 0; i < states->get_num_states(); i++) {
			stateTransitions->set(loadingState, loadingAction, orderedStates[i], probability);
		}
		return false;
	}

	if ((int)list.size() != states->get_num_states()) {
		sprintf(error, "Invalid number of probabilities given: %i != %i on line %i in file '%s'.",
				(int)list.size(), states->get_num_states(), rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition_vector", error);
		return true;
	}

	// Attempt to convert each string to a probability and set the corresponding transition probability.
	int counter = 0;

	for (std::string probabilityString : list) {
		double probability = 0.0;

		try {
			probability = std::stod(probabilityString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition_vector", error);
			return true;
		}

		if (probability < 0.0 || probability > 1.0) {
			sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition_vector", error);
			return true;
		}

		stateTransitions->set(loadingState, loadingAction, orderedStates[counter], probability);

		counter++;
	}

	return false;
}

bool UnifiedFile::load_state_transition_matrix(int stateIndex, std::string line)
{
	// Split the line into a list of (hopefully) probabilities equal to the number of states.
	std::vector<std::string> list = split_string_by_space(line);

	if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
		sprintf(error, "State index '%i' out of bounds on line %i in file '%s'.",
				stateIndex, rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition_matrix", error);
		return true;
	}

	// Handle the special cases of "uniform" and "identity".
	if (list[0].compare("uniform") == 0) {
		double probability = 1.0 / (double)states->get_num_states();
		for (int i = 0; i < states->get_num_states(); i++) {
			for (int j = 0; j < states->get_num_states(); j++) {
				stateTransitions->set(orderedStates[i], loadingAction, orderedStates[j], probability);
			}
		}
		return false;
	} else if (list[0].compare("identity") == 0) {
		for (int i = 0; i < states->get_num_states(); i++) {
			stateTransitions->set(orderedStates[i], loadingAction, orderedStates[i], 1.0);
		}
		return false;
	}

	if ((int)list.size() != states->get_num_states()) {
		sprintf(error, "Invalid number of probabilities given: '%i != %i' on line %i in file '%s'.",
				(int)list.size(), states->get_num_states(), rows, filename.c_str());
		log_message("UnifiedFile::load_state_transition_matrix", error);
		return true;
	}

	// Attempt to convert each string to a probability and set the corresponding transition probability.
	int counter = 0;

	for (std::string probabilityString : list) {
		double probability = 0.0;

		try {
			probability = std::stod(probabilityString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition_matrix", error);
			return true;
		}

		if (probability < 0.0 || probability > 1.0) {
			sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_state_transition_matrix", error);
			return true;
		}

		stateTransitions->set(orderedStates[stateIndex], loadingAction, orderedStates[counter], probability);

		counter++;
	}

	return false;
}

int UnifiedFile::load_observation_transition(std::vector<std::string> items)
{
	// Ensure a valid number of items.
	if (items.size() < 2 || items.size() > 5) {
		sprintf(error, "Incomplete statement on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition", error);
		return -1;
	}

	// Create the observation transitions object if it has not been made yet.
	if (observationTransitions == nullptr) {
		observationTransitions = new FiniteObservationTransitions();
	}

	std::string actionName = split_string_by_space(items[1])[0];
	const Action *action = nullptr;

	if (actionName.compare("*") != 0) {
		try {
			action = find_action(actions, actionName);
		} catch (const ActionException &err) {
			sprintf(error, "Action '%s' has not been defined on line %i in file '%s'.",
					actionName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition", error);
			return -1;
		}
	}

	// If this is of the form "O: action :" then a matrix follows.
	if (items.size() == 2) {
		loadingAction = action;
		return 2;
	}

	std::string endStateName = split_string_by_space(items[2])[0];
	const State *endState = nullptr;

	if (endStateName.compare("*") != 0) {
		try {
			endState = find_state(states, endStateName);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					endStateName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition", error);
			return -1;
		}
	}

	// If this is of the form "O: action : end-state :" then a vector follows.
	if (items.size() == 3) {
		loadingAction = action;
		loadingState = endState;
		return 1;
	}

	std::string observationName = split_string_by_space(items[3])[0];
	const Observation *observation = nullptr;

	if (observationName.compare("*") != 0) {
		try {
			observation = find_observation(observations, observationName);
		} catch (const ObservationException &err) {
			sprintf(error, "Observation '%s' has not been defined on line %i in file '%s'.",
					observationName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition", error);
			return -1;
		}
	}

	std::string probabilityString = split_string_by_space(items[4])[0];
	double probability = 0.0;

	try {
		probability = std::stod(probabilityString);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
				probabilityString.c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition", error);
		return -1;
	}

	if (probability < 0.0 || probability > 1.0) {
		sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
				probabilityString.c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition", error);
		return true;
	}

	observationTransitions->set(action, endState, observation, probability);

	return 0;
}

bool UnifiedFile::load_observation_transition_vector(std::string line)
{
	// Split the line into a list of (hopefully) probabilities equal to the number of observations.
	std::vector<std::string> list = split_string_by_space(line);

	// Handle the special case of "uniform".
	if (list[0].compare("uniform") == 0) {
		double probability = 1.0 / (double)observations->get_num_observations();
		for (int i = 0; i < observations->get_num_observations(); i++) {
			observationTransitions->set(loadingAction, loadingState, orderedObservations[i], probability);
		}
		return false;
	}

	if ((int)list.size() != observations->get_num_observations()) {
		sprintf(error, "Invalid number of probabilities given: %i != %i on line %i in file '%s'.",
				(int)list.size(), observations->get_num_observations(), rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition_vector", error);
		return true;
	}

	// Attempt to convert each string to a probability and set the corresponding transition probability.
	int counter = 0;

	for (std::string probabilityString : list) {
		double probability = 0.0;

		try {
			probability = std::stod(probabilityString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition_vector", error);
			return true;
		}

		if (probability < 0.0 || probability > 1.0) {
			sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition_vector", error);
			return true;
		}

		observationTransitions->set(loadingAction, loadingState, orderedObservations[counter], probability);

		counter++;
	}

	return false;
}

bool UnifiedFile::load_observation_transition_matrix(int stateIndex, std::string line)
{
	// Split the line into a list of (hopefully) probabilities equal to the number of observations.
	std::vector<std::string> list = split_string_by_space(line);

	if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
		sprintf(error, "State index '%i' out of bounds on line %i in file '%s'.",
				stateIndex, rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition_matrix", error);
		return true;
	}

	// Handle the special case of "uniform". Note: "identity" does not make sense here, since the
	// number of observations and states may not be equal.
	if (list[0].compare("uniform") == 0) {
		double probability = 1.0 / (double)observations->get_num_observations();
		for (int i = 0; i < states->get_num_states(); i++) {
			for (int j = 0; j < observations->get_num_observations(); j++) {
				observationTransitions->set(loadingAction, orderedStates[i], orderedObservations[j], probability);
			}
		}
		return false;
	}

	if ((int)list.size() != observations->get_num_observations()) {
		sprintf(error, "Invalid number of probabilities given: '%i != %i' on line %i in file '%s'.",
				(int)list.size(), observations->get_num_observations(), rows, filename.c_str());
		log_message("UnifiedFile::load_observation_transition_matrix", error);
		return true;
	}

	// Attempt to convert each string to a probability and set the corresponding transition probability.
	int counter = 0;

	for (std::string probabilityString : list) {
		double probability = 0.0;

		try {
			probability = std::stod(probabilityString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition_matrix", error);
			return true;
		}

		if (probability < 0.0 || probability > 1.0) {
			sprintf(error, "Invalid probability '%s' on line %i in file '%s'.",
					probabilityString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_observation_transition_matrix", error);
			return true;
		}

		observationTransitions->set(loadingAction, orderedStates[stateIndex], orderedObservations[counter], probability);

		counter++;
	}

	return false;
}

int UnifiedFile::load_reward(std::vector<std::string> items)
{
	// Ensure a valid number of items.
	if (items.size() < 2 || items.size() > 5) {
		sprintf(error, "Incomplete statement on line %i in file '%s'.", rows, filename.c_str());
		log_message("UnifiedFile::load_reward", error);
		return -1;
	}

	// Create the rewards object if it has not been made yet.
	if (rewards == nullptr) {
		rewards = new SASRewardsMap();
	}

	std::string actionName = split_string_by_space(items[1])[0];
	const Action *action = nullptr;

	if (actionName.compare("*") != 0) {
		try {
			action = find_action(actions, actionName);
		} catch (const ActionException &err) {
			sprintf(error, "Action '%s' has not been defined on line %i in file '%s'.",
					actionName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_reward", error);
			return -1;
		}
	}

	// If this is of the form "R: action :" then a matrix follows.
	if (items.size() == 2) {
		loadingAction = action;
		return 2;
	}

	std::string startStateName = split_string_by_space(items[2])[0];
	const State *startState = nullptr;

	if (startStateName.compare("*") != 0) {
		try {
			startState = find_state(states, startStateName);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					startStateName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_reward", error);
			return -1;
		}
	}

	// If this is of the form "R: action : start-state :" then a vector follows.
	if (items.size() == 3) {
		loadingAction = action;
		loadingState = startState;
		return 1;
	}

	std::string endStateName = split_string_by_space(items[3])[0];
	const State *endState = nullptr;

	if (endStateName.compare("*") != 0) {
		try {
			endState = find_state(states, endStateName);
		} catch (const StateException &err) {
			sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
					endStateName.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_reward", error);
			return -1;
		}
	}

	std::string rewardString = split_string_by_space(items[4])[0];
	double reward = 0.0;

	try {
		reward = std::stod(rewardString);
	} catch (const std::invalid_argument &err) {
		sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
				rewardString.c_str(), rows, filename.c_str());
		log_message("UnifiedFile::load_reward", error);
		return -1;
	}

	// Set the reward based on the previously loaded 'reward' or 'cost' specification.
	if (rewardValue) {
		rewards->set(startState, action, endState, reward);
	} else {
		rewards->set(startState, action, endState, -reward);
	}

	return 0;
}

bool UnifiedFile::load_reward_vector(std::string line)
{
	// Split the line into a list of (hopefully) rewards equal to the number of states.
	std::vector<std::string> list = split_string_by_space(line);

	if ((int)list.size() != states->get_num_states()) {
		sprintf(error, "Invalid number of rewards given: %i != %i on line %i in file '%s'.",
				(int)list.size(), states->get_num_states(), rows, filename.c_str());
		log_message("UnifiedFile::load_reward_vector", error);
		return true;
	}

	// Attempt to convert each string to a reward and set the corresponding value accordingly.
	int counter = 0;

	for (std::string rewardString : list) {
		double reward = 0.0;

		try {
			reward = std::stod(rewardString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					rewardString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_reward_vector", error);
			return true;
		}

		if (rewardValue) {
			rewards->set(orderedStates[counter], loadingAction, loadingState, reward);
		} else {
			rewards->set(orderedStates[counter], loadingAction, loadingState, -reward);
		}

		counter++;
	}

	return false;
}

bool UnifiedFile::load_reward_matrix(int stateIndex, std::string line)
{
	// Split the line into a list of (hopefully) rewards equal to the number of states.
	std::vector<std::string> list = split_string_by_space(line);

	if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
		sprintf(error, "State index '%i' out of bounds on line %i in file '%s'.",
				stateIndex, rows, filename.c_str());
		log_message("UnifiedFile::load_reward_matrix", error);
		return true;
	}

	if ((int)list.size() != states->get_num_states()) {
		sprintf(error, "Invalid number of rewards given: '%i != %i' on line %i in file '%s'.",
				(int)list.size(), states->get_num_states(), rows, filename.c_str());
		log_message("UnifiedFile::load_reward_matrix", error);
		return true;
	}

	// Attempt to convert each string to a reward and set the corresponding value accordingly.
	int counter = 0;

	for (std::string rewardString : list) {
		double reward = 0.0;

		try {
			reward = std::stod(rewardString);
		} catch (const std::invalid_argument &err) {
			sprintf(error, "Failed to convert '%s' to a double on line %i in file '%s'.",
					rewardString.c_str(), rows, filename.c_str());
			log_message("UnifiedFile::load_reward_matrix", error);
			return true;
		}

		if (rewardValue) {
			rewards->set(orderedStates[stateIndex], loadingAction, orderedStates[counter], reward);
		} else {
			rewards->set(orderedStates[stateIndex], loadingAction, orderedStates[counter], -reward);
		}

		counter++;
	}

	return false;
}

void UnifiedFile::release()
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
}

MDP *UnifiedFile::get_mdp()
{
	if (states == nullptr || actions == nullptr || stateTransitions == nullptr || rewards == nullptr ||
			initialState == nullptr || horizon == nullptr) {
		throw CoreException();
	}

	MDP *mdp = new MDP(states, actions, stateTransitions, rewards, initialState, horizon);
	release();
	return mdp;
}

POMDP *UnifiedFile::get_pomdp()
{
	if (states == nullptr || actions == nullptr || observations == nullptr || stateTransitions == nullptr ||
			observationTransitions == nullptr || rewards == nullptr || initialState == nullptr ||
			horizon == nullptr) {
		throw CoreException();
	}

	POMDP *pomdp = new POMDP(states, actions, observations, stateTransitions, observationTransitions,
			rewards, initialState, horizon);
	release();
	return pomdp;
}

DecMDP *UnifiedFile::get_dec_mdp()
{
	if (agents == nullptr || states == nullptr || actions == nullptr || stateTransitions == nullptr ||
			rewards == nullptr || initialState == nullptr || horizon == nullptr) {
		throw CoreException();
	}

	DecMDP *decmdp = new DecMDP(agents, states, actions, stateTransitions, rewards, initialState, horizon);
	release();
	return decmdp;
}

DecPOMDP *UnifiedFile::get_dec_pomdp()
{
	if (agents == nullptr || states == nullptr || actions == nullptr || observations == nullptr ||
			stateTransitions == nullptr || observationTransitions == nullptr || rewards == nullptr ||
			initialState == nullptr || horizon == nullptr) {
		throw CoreException();
	}

	DecPOMDP *decpomdp = new DecPOMDP(agents, states, actions, observations, stateTransitions, observationTransitions,
			rewards, initialState, horizon);
	release();
	return decpomdp;
}
