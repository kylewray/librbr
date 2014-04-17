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

#include "../../../include/core/policy/policy_alpha_vectors.h"
#include "../../../include/core/policy/policy_alpha_vector.h"

#include "../../../include/utilities/log.h"

#include "../../../include/core/actions/action_exception.h"
#include "../../../include/core/states/state_exception.h"
#include "../../../include/core/observations/observation_exception.h"
#include "../../../include/core/policy/policy_exception.h"

#include "../../../include/utilities/string_manipulation.h"

/**
 * The default constructor for a PolicyAlphaVectors object. It defaults to a horizon of 1.
 */
PolicyAlphaVectors::PolicyAlphaVectors()
{
	alphaVectors.resize(1);
}

/**
 * A constructor for a PolicyAlphaVectors object which specifies the horizon.
 * @param horizon The horizon of the problem; 0 represents infinite horizon.
 */
PolicyAlphaVectors::PolicyAlphaVectors(unsigned int horizon)
{
	if (horizon > 0) {
		alphaVectors.resize(horizon);
	} else {
		alphaVectors.resize(1);
	}
}

/**
 * A constructor for a PolicyAlphaVectors object which specifies the horizon.
 * @param horizon The horizon object from the MDP-like object.
 */
PolicyAlphaVectors::PolicyAlphaVectors(const Horizon *horizon)
{
	if (horizon->get_horizon() > 0) {
		alphaVectors.resize(horizon->get_horizon());
	} else {
		alphaVectors.resize(1);
	}
}

/**
 * A constructor for a PolicyAlphaVectors object which specifies the alpha vectors.
 * It defaults to a horizon of 1 in this case.
 * @param alphas The set of alpha vectors.
 */
PolicyAlphaVectors::PolicyAlphaVectors(const std::vector<PolicyAlphaVector *> &alphas)
{
	alphaVectors.resize(1);
	set(alphas);
}

/**
 * A constructor for a PolicyAlphaVectors object which specifies the alpha vectors.
 * It defaults to a horizon of 1 in this case.
 * @param alphas The set of alpha vectors at each horizon.
 */
PolicyAlphaVectors::PolicyAlphaVectors(const std::vector<const std::vector<PolicyAlphaVector *> > &alphas)
{
	// If no alpha vectors are properly given, then simply reserve the size for 1 horizon.
	if (alphas.size() == 0 || alphas[0].size() == 0) {
		alphaVectors.resize(1);
		return;
	}

	// Otherwise, set each of the horizon's alpha vectors.
	alphaVectors.resize(alphas.size());
	for (const std::vector<PolicyAlphaVector *> &alpha : alphas) {
		set(alpha);
	}
}

/**
 * A virtual deconstructor to prevent errors upon the deletion of a child object.
 */
PolicyAlphaVectors::~PolicyAlphaVectors()
{ }

/**
 * Set the alpha vectors. For finite horizon, it assumes 0 by default.
 * @param alphas The set of alpha vectors.
 */
void PolicyAlphaVectors::set(const std::vector<PolicyAlphaVector *> &alphas)
{
	set(0, alphas);
}

/**
 * Set the alpha vectors, allowing the explicit specification of the horizon.
 * @param horizon			The horizon to set.
 * @param alphas 			The set of alpha vectors.
 * @throws PolicyException	The horizon was invalid.
 */
void PolicyAlphaVectors::set(unsigned int horizon, const std::vector<PolicyAlphaVector *> &alphas)
{
	if (horizon >= alphaVectors.size()) {
		throw PolicyException();
	}

	// First, free the memory of the current alpha vectors (if any) at this horizon. Then, set it.
	for (PolicyAlphaVector *alpha : alphaVectors[horizon]) {
		delete alpha;
	}
	alphaVectors[horizon].clear();
	alphaVectors[horizon] = alphas;
}

/**
 * Get the action for a given belief state. For finite horizon, it assumes 0 by default. This frees the memory of
 * previous alpha vectors.
 * @param belief The belief state to retrieve a mapping.
 * @return The action to take at the given belief state.
 * @throws PolicyException The policy was not defined for this state.
 */
const Action *PolicyAlphaVectors::get(const BeliefState *belief) const
{
	return get(0, belief);
}

/**
 * Get the action for a given belief state, allowing the explicit specification of the horizon. This frees
 * the memory of previous alpha vectors.
 * @param horizon	The horizon to set.
 * @param belief	The belief state to retrieve a mapping.
 * @return The action to take at the given belief state.
 * @throws PolicyException The policy was not defined for this belief state, or horizon was invalid.
 */
const Action *PolicyAlphaVectors::get(unsigned int horizon, const BeliefState *belief) const
{
	// Ensure that there is an alpha vector defined.
	if (horizon >= alphaVectors.size() || alphaVectors[horizon].size() == 0) {
		throw PolicyException();
	}

	// Initially assume the first alpha vector is the best.
	const Action *action = alphaVectors[horizon][0]->get_action();
	double Vb = alphaVectors[horizon][0]->compute_value(belief);

	// Find the alpha vector which maximizes "dot(b, alpha)" and return the corresponding action.
	for (PolicyAlphaVector *alpha : alphaVectors[horizon]) {
		double VbPrime = alpha->compute_value(belief);
		if (VbPrime > Vb) {
			Vb = VbPrime;
			action = alpha->get_action();
		}
	}

	return action;
}

/**
 * A function which must load a policy file.
 * @param filename		The name and path of the file to load.
 * @param states		The states object which contains the actual state objects to be mapped.
 * @param actions		The actions object which contains the actual action objects to be mapped.
 * @param observations	The observations object which contains the actual observation objects to be mapped.
 * @param horizon		The horizons object to ensure valid policy creation.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool PolicyAlphaVectors::load(std::string filename, const FiniteStates *states, const FiniteActions *actions,
		const FiniteObservations *observations, const Horizon *horizon)
{
	reset();

	/*
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

	int h = 1;

	const State *state = nullptr;
	const Action *action = nullptr;

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

		std::vector<std::string> items = split_string_by_colon(line);

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

			policy[h - 1][state] = action;
		}

		rows++;
	}

	file.close();
	*/

	return false;
}

/**
 * A function which must save a policy file.
 * @param filename The name and path of the file to save.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool PolicyAlphaVectors::save(std::string filename) const
{
	/*
	std::ofstream file(filename);
	if (!file.is_open()) {
		return true;
	}

	char error[1024];

	if (policy.size() > 1) {
		int h = 1;

		for (std::map<const State *, const Action *> p : policy) {
			file << "horizon: " << h << std::endl;

			for (std::map<const State *, const Action *>::value_type iter : p) {
				file << iter.first->to_string() << ": " << iter.second->get_name() << std::endl;
			}

			file << std::endl;
			h++;
		}
	} else if (policy.size() == 1) {
		for (std::map<const State *, const Action *>::value_type iter : policy[0]) {
			file << iter.first->to_string() << ": " << iter.second->get_name() << std::endl;
		}
	} else {
		sprintf(error, "Failed to save file '%s'. No policy was defined.", filename.c_str());
		log_message(std::cout, "MapPolicy::save", error);
		return true;
	}

	file.close();
	*/

	return false;
}

/**
 * Reset the alpha vectors, freeing the memory.
 */
void PolicyAlphaVectors::reset()
{
	for (int t = 0; t < alphaVectors.size(); t++) {
		for (PolicyAlphaVector *alpha : alphaVectors[t]) {
			delete alpha;
		}
		alphaVectors[t].clear();
	}
	alphaVectors.clear();
}
