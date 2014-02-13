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

#include "../../../include/core/policy/map_policy.h"

#include "../../../include/utilities/log.h"

#include "../../../include/core/actions/action_exception.h"
#include "../../../include/core/states/state_exception.h"
#include "../../../include/core/policy/policy_exception.h"

#include "../../../include/utilities/string_manipulation.h"

/**
 * The default constructor for a MapPolicy object. It defaults to 1 level.
 */
MapPolicy::MapPolicy()
{
	policy.resize(1);
}

/**
 * A constructor for a MapPolicy object which specifies the horizon.
 * @param horizon The horizon of the problem; 0 represents infinite horizon.
 */
MapPolicy::MapPolicy(unsigned int horizon)
{
	if (horizon > 0) {
		policy.resize(horizon);
	} else {
		policy.resize(1);
	}
}

/**
 * A constructor for a MapPolicy object which specifies the horizon.
 * @param horizon The horizon object from the MDP-like object.
 */
MapPolicy::MapPolicy(const Horizon *horizon)
{
	if (horizon->get_horizon() > 0) {
		policy.resize(horizon->get_horizon());
	} else {
		policy.resize(1);
	}
}

/**
 * A virtual deconstructor to prevent errors upon the deletion of a child object.
 */
MapPolicy::~MapPolicy()
{ }

/**
 * Set the mapping from a state to an action. For finite horizon, it assumes 0 by default.
 * @param state		The state to define.
 * @param action	The action which should be taken at the state.
 */
void MapPolicy::set(State *state, Action *action)
{
	policy[0][state] = action;
}

/**
 * Set the mapping from a state to an action, allowing the explicit specification of the horizon.
 * @param horizon			The horizon to set.
 * @param state				The state to define.
 * @param action			The action which should be taken at the state.
 * @throws PolicyException	The horizon was invalid.
 */
void MapPolicy::set(unsigned int horizon, State *state, Action *action)
{
	if (horizon >= policy.size()) {
		throw PolicyException();
	}

	policy[horizon][state] = action;
}

/**
 * Get the action for a given state. For finite horizon, it assumes 0 by default.
 * @param state The state to retrieve a mapping.
 * @return The action to take at the given state.
 * @throws PolicyException The policy was not defined for this state.
 */
Action *MapPolicy::get(State *state) const
{
	std::map<State *, Action *>::const_iterator result = policy[0].find(state);
	if (result == policy[0].end()) {
		throw PolicyException();
	}

	return result->second;
}

/**
 * Get the action for a given state, allowing the explicit specification of the horizon.
 * @param horizon	The horizon to set.
 * @param state		The state to retrieve a mapping.
 * @return The action to take at the given state.
 * @throws PolicyException The policy was not defined for this state, or horizon was invalid.
 */
Action *MapPolicy::get(unsigned int horizon, State *state) const
{
	if (horizon >= policy.size()) {
		throw PolicyException();
	}

	std::map<State *, Action *>::const_iterator result = policy[horizon].find(state);
	if (result == policy[horizon].end()) {
		throw PolicyException();
	}

	return result->second;
}

/**
 * A function which must load a policy file.
 * @param filename	The name and path of the file to load.
 * @param states	The states object which contains the actual state objects to be mapped.
 * @param actions	The actions object which contains the actual action objects to be mapped.
 * @param horizon	The horizons object to ensure valid policy creation.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool MapPolicy::load(std::string filename, const FiniteStates *states, const FiniteActions *actions, const Horizon *horizon)
{
	reset();

	char error[1024];

	// Load the file and return if it failed to open.
	std::ifstream file(filename);
	if (!file.is_open()) {
		sprintf(error, "Failed to open file '%s'.", filename.c_str());
		log_message(std::cout, "MapPolicy::load", error);
		return true;
	}

	int rows = 1;
	std::string line;

	int h = 0;

	State *state = nullptr;
	Action *action = nullptr;

	// Before starting, reserve the space for the horizon given.
	policy.resize(horizon->get_horizon());

	// Iterate over all lines of the file separately.
	while (std::getline(file, line)) {
		// Handle comments by removing all characters down to and including a '#'.
		line = line.substr(0, line.find('#'));

		// Skip over blank lines.
		if (line.length() == 0) {
			rows++;
			continue;
		}

		// Split the key and value of the line, and trim the white spaces.
		std::istringstream ssLine(line);
		std::vector<std::string> items;
		std::string temp;

		while (std::getline(ssLine, temp, ':')) {
			trim_whitespace(temp);
			if (temp.length() == 0) {
				continue;
			}
			items.push_back(temp);
		}

		// Every line must contain exactly one a colon.
		if (items.size() != 2) {
			sprintf(error, "Improper statement (perhaps missing a colon) on line %i in file '%s'.",
					rows, filename.c_str());
			log_message(std::cout, "MapPolicy::load", error);
			return true;
		}

		// Two cases, either this line defines a new horizon, or it states a mapping.
		if (items[0].compare("horizon") == 0) {
			// Try to parse the integer given, raising an error if it fails.
			try {
				h = std::stoi(items[1]);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Failed to convert '%s' to an integer on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message(std::cout, "MapPolicy::load", error);
				return true;
			}

			// The horizon must be non-negative. A zero horizon can mean infinite, or simply horizon zero.
			if (h < 0 || h > horizon->get_horizon()) {
				sprintf(error, "Horizon %s is invalid on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message(std::cout, "MapPolicy::load", error);
				return true;
			}
		} else {
			// Since this is an actual mapping, we simply map the key to the value at the horizon. Note,
			// however, that we must first find the actual state and action.
			try {
				state = states->find(items[0]);
			} catch (const StateException &err) {
				sprintf(error, "State %s was not defined on line %i in file '%s'.",
						items[0].c_str(), rows, filename.c_str());
				log_message(std::cout, "MapPolicy::load", error);
				return true;
			}

			try {
				action = actions->find(items[1]);
			} catch (const ActionException &err) {
				sprintf(error, "Action %s was not defined on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message(std::cout, "MapPolicy::load", error);
				return true;
			}

			policy[h][state] = action;
		}

		rows++;
	}

	file.close();

	return false;
}

/**
 * A function which must save a policy file.
 * @param filename The name and path of the file to save.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool MapPolicy::save(std::string filename) const
{
	std::ofstream file(filename);
	if (!file.is_open()) {
		return true;
	}

	char error[1024];

	if (policy.size() > 1) {
		int h = 0;

		for (std::map<State *, Action *> p : policy) {
			file << "horizon: " << h << std::endl;

			for (std::map<State *, Action *>::value_type iter : p) {
				file << iter.first->get_name() << ": " << iter.second->get_name() << std::endl;
			}

			file << std::endl;
			h++;
		}
	} else if (policy.size() == 1) {
		for (std::map<State *, Action *>::value_type iter : policy[0]) {
			file << iter.first->get_name() << ": " << iter.second->get_name() << std::endl;
		}
	} else {
		sprintf(error, "Failed to save file '%s'. No policy was defined.", filename.c_str());
		log_message(std::cout, "MapPolicy::save", error);
		return true;
	}

	file.close();

	return false;
}

/**
 * Reset the policy mapping.
 */
void MapPolicy::reset()
{
	policy.clear();
}
