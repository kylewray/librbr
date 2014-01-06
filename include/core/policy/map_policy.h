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


#ifndef MAP_POLICY_H
#define MAP_POLICY_H


#include <string>
#include <map>

#include "policy.h"


/**
 * A simple map policy which works for MDPs and Dec-MDPs; each actual state
 * deterministically maps to a single action. This will likely be used
 * exclusively for infinite horizon MDPs and Dec-MDPs.
 */
class MapPolicy : public Policy {
public:
	/**
	 * The default constructor for a MapPolicy object.
	 */
	MapPolicy();

	/**
	 * The constructor for a MapPolicy object which allows the user to set the number of states.
	 */
	MapPolicy();

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~MapPolicy();

	/**
	 * A virtual function which must load a policy file.
	 * @param filename The name and path of the file to load.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename);

	/**
	 * A virtual function which must save a policy file.
	 * @param filename The name and path of the file to save.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename) const;

	/**
	 * A function which follows the defined policy, having the current state stored internally,
	 * and returns the action to select next.
	 */
	virtual Action next();

	/**
	 * Reset the policy to the initial state.
	 */
	virtual void reset();

private:
	/**
	 * The internal mapping from states to actions.
	 */
	std::map<State, Action> policy;

};


#endif // MAP_POLICY_H
