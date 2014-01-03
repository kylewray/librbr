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


#ifndef SCP_POLICY_H
#define SCP_POLICY_H


#include <string>


/**
 * A small struct which describes a particular policy's action to take at a
 * given state.
 */
struct PolicyAction {
	std::string action;
	int actionIndex;
};


/**
 * A small struct which describes a particular state or observation history,
 * and possibly the horizon.
 */
struct PolicyCurrent {
	std::string state;
	int stateIndex;
	int horizon;
};


/**
 * An abstract class for all policies to stochastic control problems (SCPs).
 */
class Policy {
public:
	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~Policy();

	/**
	 * A virtual function which must load a SCP policy file.
	 *
	 * @param filename The name of the file to load.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename) const = 0;

	/**
	 * A virtual function which must save a SCP policy file.
	 *
	 * @param filename The name of the file to save.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename) const = 0;

	/**
	 * A function which follows the defined policy and return the action to select next.
	 *
	 * @param state		The current state (mdp/dec-mdp) or observation history (pomdp/dec-pomdp).
	 * @param action	The return variable for the next action to take.
	 * @return Return @code{true} if an error occurred, and @code{false} otherwise.
	 */
	virtual bool next(const PolicyCurrent* state, PolicyAction* action);

};


#endif SCP_POLICY_H
