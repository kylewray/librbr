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

#include "scp_types.h"


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
	 * The constructor which enables automatic loading of the specified SCP file.
	 * @param filename The filename of the SCP file to load.
	 */
	UnifiedFile(std::string filename);

	/**
	 * The deconstructor for a unified file which cleans up the memory.
	 */
	~UnifiedFile();

	/**
	 * A function which loads any mdp-like file.
	 * @param filename The filename and relative path of the mdp-like file to load.
	 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool load(std::string filename);

	/**
	 * A function which saves the information as a SCP file.
	 * @param filename The filename and relative path of the mdp-like file to save.
	 * @return Returns @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool save(std::string filename);

	/**
	 * Reset the internal objects in the unified file.
	 */
	void reset();

private:
	/**
	 * The agents in the SCP; e.g., a vector of strings.
	 */
	Agents agents;

	/**
	 * The states in the SCP; e.g., factored vectors of strings.
	 */
	States states;

	/**
	 * The actions in the SCP; e.g., factored vectors of strings.
	 */
	Actions actions;

	/**
	 * The observations in the SCP; e.g., factored vectors of strings.
	 */
	Observations observations;

	/**
	 * The state transition function in the SCP; e.g., a three-dimensional array mapping to a double.
	 */
	StateTransitions stateTransitions;

	/**
	 * The observation transition function in the SCP; e.g., a three-dimensional array mapping to a double.
	 */
	ObservationTransitions observationTransitions;

	/**
	 * The reward function in the SCP; e.g., a two-dimensional array mapping to a double.
	 */
	Rewards rewards;

	/**
	 * The initial state or initial belief state; e.g., factored initial state.
	 */
	InitialState initialState;

	/**
	 * The horizon, either a finite time or a discount factor.
	 */
	Horizon horizon;

};


#endif // UNIFIED_FILE_H
