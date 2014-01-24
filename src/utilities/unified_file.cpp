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
}

/**
 * The constructor which enables automatic loading of the specified SCP file.
 * @param filename The filename of the SCP file to load.
 */
UnifiedFile::UnifiedFile(std::string filename)
{
	load(filename);
}

/**
 * The deconstructor for a unified file which cleans up the memory.
 */
UnifiedFile::~UnifiedFile()
{ }

/**
 * A function which loads any mdp-like file.
 * @param filename The filename and relative path of the mdp-like file to load.
 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
 */
bool UnifiedFile::load(std::string filename)
{
	reset();

	// Open the file.
	std::ifstream file(filename);
	std::string line;
	char error[1024];

	// If the file failed to open, then do not do anything.
	if (file.is_open()) {
		// Iterate over all lines in the file, and then each character in a line.
		int row = 1;

		FileObjectCategory category = FileObjectCategory::OBJECT_NONE;

		while (std::getline(file, line)) {
			// Handle comments by removing all characters down to and including a '#'.
			line = line.substr(0, line.find('#'));

			//line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

			// Skip over blank lines.
			if (line.length() == 0) {
				row++;
				continue;
			}

			// Two cases, either this is a keyword line, or it is an information line.
			if (line.find(':') != std::string::npos) {
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
					if (horizon == nullptr) {
						horizon = new Horizon();
					}

					// Remove the whitespace.
					items[1].erase(std::remove(items[1].begin(), items[1].end(), ' '), items[1].end());

					// Attempt to convert the string to a double. If successful, then set the horizon.
					double h = 0.0;
					try {
						h = std::stod(items[1]);
					} catch (const std::invalid_argument &err) {
						sprintf(error, "Failed to convert '%s' to a float on line %i in file '%s'.",
								items[1].c_str(), row, filename.c_str());
						log_message(std::cout, "UnifiedFile::load", error);
						return true;
					}

					horizon->set_horizon(h);
				} else if (items[0].compare("discount") == 0) {
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
								items[1].c_str(), row, filename.c_str());
						log_message(std::cout, "UnifiedFile::load", error);
						return true;
					}

					horizon->set_discount_factor(d);
				} else if (items[0].compare("start") == 0) {
					if (initialState != NULL) {
						initialState = new InitialState();
					}

					// Split the first item by spaces, skipping over the empty strings.
					std::istringstream ssItem(items[1]);
					std::vector<std::string> list;

					while (std::getline(ssItem, temp, ' ')) {
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
							State *state = states->find(list[0]);
							if (state == nullptr) {
								sprintf(error, "State '%s' has not been defined on line %i in file '%s'.",
										list[0].c_str(), row, filename.c_str());
								log_message(std::cout, "UnifiedFile::load", error);
								return true;
							}
							initialState->set_initial_state(state);
						} else {
							// Handle the case in which the user provided an invalid state.
							if (stateIndex < 0 || stateIndex >= states->get_num_states()) {
								sprintf(error, "State integer '%i' out of bounds on line %i in file '%s'.",
										stateIndex, row, filename.c_str());
								log_message(std::cout, "UnifiedFile::load", error);
								return true;
							}
							initialState->set_initial_state(states->all()[stateIndex]);
						}
					} else {
						// This must be a full list of probabilities.
						int counter = 0;
						for (std::string probability : list) {
							if (probability.length() == 0) {
								continue;
							}

							double p = 0.0;
							try {
								p = std::stod(probability);
							} catch (const std::invalid_argument &err) {
								sprintf(error, "Failed to convert '%s' to a float on line %i in file '%s'.",
										items[1].c_str(), row, filename.c_str());
								log_message(std::cout, "UnifiedFile::load", error);
								return true;
							}

							initialState->set_initial_belief(((FiniteStates *)states)->all()[counter], p);
							counter++;
						}
					}

				} else if (items[0].compare("start include") == 0) {

				} else if (items[0].compare("start exclude") == 0) {

				} else if (items[0].compare("values") == 0) {

				} else if (items[0].compare("agents") == 0) {

				} else if (items[0].compare("states") == 0) {

				} else if (items[0].compare("actions") == 0) {

				} else if (items[0].compare("observations") == 0) {

				} else if (items[0].compare("T") == 0) {

				} else if (items[0].compare("O") == 0) {

				} else if (items[0].compare("R") == 0) {

				}
			} else {
				// If this does not contain a colon (and is not blank), then it
				// is an information line. For information lines, split the line based on
				// spaces and read each float, creating the appropriate map.

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
 * A function which saves the information as a SCP file.
 * @param filename The filename and relative path of the mdp-like file to save.
 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
 */
bool UnifiedFile::save(std::string filename)
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
}
