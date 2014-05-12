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


#include "../../../include/core/policy/policy_graph.h"

/**
 * The constructor for a PolicyGraph object which specifies the number of internal policy states.
 * @param numPolicyStates The number of policy states internal to the FSC policy.
 */
PolicyGraph::PolicyGraph(int numPolicyStates)
{
	current = PolicyGraphNode();
}

/**
 * A virtual deconstructor to prevent errors upon the deletion of a child object.
 */
PolicyGraph::~PolicyGraph()
{ }

/**
 * A function which must load a policy file.
 * @param filename The name and path of the file to load.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool PolicyGraph::load(std::string filename)
{
	return false;
}

/**
 * A function which must save a policy file.
 * @param filename The name and path of the file to save.
 * @return Return @code{true} if an error occurred, @code{false} otherwise.
 */
bool PolicyGraph::save(std::string filename) const
{
	return false;
}

/**
 * A function which follows the defined policy, having the current state stored internally,
 * and returns the action to select next.
 */
const Action *PolicyGraph::next()
{
	return nullptr;
}

/**
 * Reset the policy to the initial state.
 */
void PolicyGraph::reset()
{ }

/**
 * Initialize the FSCPolicy object, which resets only the execution variables
 * such as the random initial selection of the internal FSC state following the
 * distribution over initial states.
 */
void PolicyGraph::initialize()
{ }
