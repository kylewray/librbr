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


#include <iostream>
#include <fstream>

#include "../../../include/core/policy/policy_alpha_vectors.h"
#include "../../../include/core/policy/policy_alpha_vector.h"

#include "../../../include/core/actions/action_exception.h"
#include "../../../include/core/states/state_exception.h"
#include "../../../include/core/observations/observation_exception.h"
#include "../../../include/core/policy/policy_exception.h"

#include "../../../include/utilities/log.h"
#include "../../../include/utilities/string_manipulation.h"

#include "../../../include/core/states/state_utilities.h"
#include "../../../include/core/actions/action_utilities.h"

#include <coin/OsiSolverInterface.hpp>
#include <coin/OsiClpSolverInterface.hpp>
#include <coin/CoinPackedVector.hpp>
#include <coin/CoinPackedMatrix.hpp>
#include <coin/CoinMessageHandler.hpp>

PolicyAlphaVectors::PolicyAlphaVectors()
{
	alphaVectors.resize(1);
}

PolicyAlphaVectors::PolicyAlphaVectors(unsigned int horizon)
{
	if (horizon > 0) {
		alphaVectors.resize(horizon);
	} else {
		alphaVectors.resize(1);
	}
}

PolicyAlphaVectors::PolicyAlphaVectors(const Horizon *horizon)
{
	if (horizon->get_horizon() > 0) {
		alphaVectors.resize(horizon->get_horizon());
	} else {
		alphaVectors.resize(1);
	}
}

PolicyAlphaVectors::PolicyAlphaVectors(const std::vector<PolicyAlphaVector *> &alphas)
{
	alphaVectors.resize(1);
	set(alphas);
}

PolicyAlphaVectors::PolicyAlphaVectors(const std::vector<std::vector<PolicyAlphaVector *> > &alphas)
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

PolicyAlphaVectors::~PolicyAlphaVectors()
{ }

void PolicyAlphaVectors::set(const std::vector<PolicyAlphaVector *> &alphas)
{
	set(0, alphas);
}

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

const Action *PolicyAlphaVectors::get(const BeliefState *belief) const
{
	return get(0, belief);
}

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

bool PolicyAlphaVectors::load(std::string filename, const StatesMap *states, const ActionsMap *actions,
		const ObservationsMap *observations, const Horizon *horizon)
{
	reset();

	char error[1024];

	// Load the file and return if it failed to open.
	std::ifstream file(filename);
	if (!file.is_open()) {
		sprintf(error, "Failed to open file '%s'.", filename.c_str());
		log_message("PolicyAlphaVectors::load", error);
		return true;
	}

	int rows = 1;
	std::string line;

	int h = 1;

	const State *state = nullptr;
	const Action *action = nullptr;

	// Before starting, reserve the space for the horizon given.
	alphaVectors.resize(horizon->get_horizon());

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

		// Every line must contain at least one colon.
		if (items.size() < 2) {
			sprintf(error, "Improper statement (perhaps missing a colon) on line %i in file '%s'.",
					rows, filename.c_str());
			log_message("PolicyAlphaVectors::load", error);
			return true;
		}

		// Two cases, either this line defines a new horizon, or it states an alpha vector for the current horizon.
		if (items[0].compare("horizon") == 0) {
			// Try to parse the integer given, raising an error if it fails.
			try {
				h = std::stoi(items[1]);
			} catch (const std::invalid_argument &err) {
				sprintf(error, "Failed to convert '%s' to an integer on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message("PolicyAlphaVectors::load", error);
				return true;
			}

			// The horizon must be non-negative. A zero horizon can mean infinite, or simply horizon zero.
			if (h < 1 || h > horizon->get_horizon()) {
				sprintf(error, "Horizon %s is invalid on line %i in file '%s'.",
						items[1].c_str(), rows, filename.c_str());
				log_message("PolicyAlphaVectors::load", error);
				return true;
			}
		} else {
			// Before anything, make sure that this has a correct number of defined items.
			if (items.size() % 2 != 1) {
				sprintf(error, "Invalid number of defined items on line %i in file '%s'.",
						rows, filename.c_str());
				log_message("PolicyAlphaVectors::load", error);
				return true;
			}

			// Since this is an alpha vector, the first item must be an action.
			try {
				action = find_action(actions, items[0]);
			} catch (const ActionException &err) {
				sprintf(error, "Action %s was not defined on line %i in file '%s'.",
						items[0].c_str(), rows, filename.c_str());
				log_message("PolicyAlphaVectors::load", error);
				return true;
			}

			// The remaining items are state-value pairs.
			PolicyAlphaVector *newAlphaVector = new PolicyAlphaVector(action);
			for (int i = 1; i < items.size(); i += 2) {
				// First is the state in the pair.
				try {
					state = find_state(states, items[i]);
				} catch (const StateException &err) {
					sprintf(error, "State %s was not defined on line %i in file '%s'.",
							items[i].c_str(), rows, filename.c_str());
					log_message("PolicyAlphaVectors::load", error);
					return true;
				}

				// Next is the value function for that state.
				double alphaValue = 0.0;
				try {
					alphaValue = std::stod(items[i + 1]);
				} catch (const StateException &err) {
					sprintf(error, "The value '%s' is not a valid number on line %i in file '%s'.",
							items[i + 1].c_str(), rows, filename.c_str());
					log_message("PolicyAlphaVectors::load", error);
					return true;
				}

				newAlphaVector->set(state, alphaValue);
			}

			alphaVectors[h - 1].push_back(newAlphaVector);
		}

		rows++;
	}

	file.close();

	return false;
}

bool PolicyAlphaVectors::save(std::string filename, const StatesMap *states) const
{
	std::ofstream file(filename);
	if (!file.is_open()) {
		return true;
	}

	char error[1024];

	if (alphaVectors.size() > 1) {
		int h = 1;

		// This ranged-based for loop iterates without cloning each set of alpha vectors.
		for (const std::vector<PolicyAlphaVector *> &alphaVectorSet : alphaVectors) {
			file << "horizon: " << h << std::endl;

			for (PolicyAlphaVector *alpha : alphaVectorSet) {
				file << alpha->get_action()->to_string();
				for (auto s : *states) {
					const State *state = resolve(s);
					file << " : " << state->to_string() << " : " << alpha->get(state);
				}
				file << std::endl;
			}

			file << std::endl;
			h++;
		}
	} else if (alphaVectors.size() == 1) {
		for (PolicyAlphaVector *alpha : alphaVectors[0]) {
			file << alpha->get_action()->to_string();
			for (auto s : *states) {
				const State *state = resolve(s);
				file << " : " << state->to_string() << " : " << alpha->get(state);
			}
			file << std::endl;
		}

		file << std::endl;
	} else {
		sprintf(error, "Failed to save file '%s'. No policy was defined.", filename.c_str());
		log_message("PolicyAlphaVectors::save", error);
		return true;
	}

	file.close();

	return false;
}

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

void PolicyAlphaVectors::prune_dominated(const StatesMap *S, std::vector<PolicyAlphaVector *> &alphas)
{
	if (S == nullptr || S->get_num_states() == 0 || alphas.size() == 0 || alphas[0] == nullptr) {
		throw PolicyException();
	}

	// Prune alpha vectors by solving a linear program. The linear program attempts to find a belief (denoted as beta)
	// for which the alpha vector in question uniquely maximizes the value function at that point in the belief space.
	// If no belief can be found, i.e., the LP returns unbounded or infeasible, then we can prune the alpha vector
	// because it implies at least one other alpha vector dominates it at every belief point.
	for (std::vector<PolicyAlphaVector *>::iterator iter = alphas.begin(); iter != alphas.end(); /* iter++ */) {
		OsiSolverInterface *si = new OsiClpSolverInterface();
		int numCols = S->get_num_states();

		// Disable the standard output messages that CLP generates.
		CoinMessageHandler *cmh = si->messageHandler();
		cmh->setLogLevel(0);

		// This is the constant vector c, defined to be the particular alpha vector in this iteration.
		double *objective = new double[numCols];
		int i = 0;
		for (auto state : *S) {
			// Note: Negative since CLP minimizes the objective function.
			objective[i] = -(*iter)->get(resolve(state));
			i++;
		}

		// Create the bounds on the beta (belief) values, which must be between 0 and 1.
		double *colsLB = new double[numCols];
		double *colsUB = new double[numCols];
		for (i = 0; i < S->get_num_states(); i++) {
			colsLB[i] = 0.0;
			colsUB[i] = 1.0;
		}

		// Create the bounds on the constraint matrix (belief) values.
		int numRows = alphas.size(); // 1 + (alphas.size() - 1)
		double *rowsLB = new double[numRows];
		double *rowsUB = new double[numRows];

		// Define the special first row of the constraint matrix.
		CoinPackedMatrix *constraintMatrix = new CoinPackedMatrix(false, 0, 0);
		constraintMatrix->setDimensions(0, numCols);

		CoinPackedVector firstRow;
		for (i = 0; i < S->get_num_states(); i++) {
			firstRow.insert(i, 1.0);
		}
		constraintMatrix->appendRow(firstRow);

		// Set the lower and upper bounds.
		rowsLB[0] = 1.0;
		rowsUB[0] = 1.0;

		// Define the rest of the constraint matrix.
		i = 1;
		for (std::vector<PolicyAlphaVector *>::iterator iterA = alphas.begin(); iterA != alphas.end(); iterA++) {
			// Do not include the current alpha vector in the constraint matrix's rows.
			if (iterA == iter) {
				continue;
			}

			// Set the row of the constraint matrix.
			CoinPackedVector otherRow;
			int j = 0;
			for (auto s : *S) {
				const State *state = resolve(s);
				otherRow.insert(j, (*iterA)->get(state) - (*iter)->get(state));
				j++;
			}
			constraintMatrix->appendRow(otherRow);

			// Set the lower and upper bounds.
			rowsLB[i] = -1.0 * si->getInfinity();
			rowsUB[i] = 0.0;

			i++;
		}

		// Solve the linear program and erase the alpha vector if no solution could be found.
		si->loadProblem(*constraintMatrix, colsLB, colsUB, objective, rowsLB, rowsUB);
		si->initialSolve();

		if (!si->isProvenOptimal()) {
			// Free the memory of the alpha vector.
			delete *iter;
			iter = alphas.erase(iter);
		} else {
			++iter;
		}

		// Free all the memory that has been allocated.
		delete [] objective;
		delete [] colsLB;
		delete [] colsUB;

		delete constraintMatrix;
		delete [] rowsLB;
		delete [] rowsUB;

		delete si;
	}
}
