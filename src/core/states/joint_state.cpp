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


#include "../../../include/core/states/joint_state.h"
#include "../../../include/core/states/state_exception.h"


/**
 * The constructor of the JointState object which allows initial specification of the
 * number of joint states.
 * @param numJointStates The number of joint states in the tuple.
 */
JointState::JointState(int numJointStates)
{
	states.reserve(numJointStates);
}

/**
 * The constructor of the JointState object which allows initial specification of the
 * actual joint state tuple (vector).
 * @param jointState The list of states which define this joint state.
 */
JointState::JointState(std::vector<State *> jointState)
{
	states.reserve(jointState.size());
	states = jointState;
}

/**
 * The copy constructor of the JointState object.
 * @param other The state to copy.
 */
JointState::JointState(const JointState &other)
{
	*this = other;
}

/**
 * The default deconstructor of the JointState object.
 */
JointState::~JointState()
{ }

/**
 * Override set name (leave get_name() alone) to raise an error when it is called.
 * @param newName The new name.
 * @throws StateException This is no longer a valid function.
 */
void JointState::set_name(std::string newName)
{
	throw StateException();
}

/**
 * Set the joint state given a list of states.
 * @param jointState The list of states which define this joint state.
 */
void JointState::set(std::vector<State *> jointState)
{
	states = jointState;
}

/**
 * Get the joint state.
 * @return The list of states.
 */
std::vector<State *> JointState::get() const
{
	return states;
}

/**
 * Get a particular state at a index.
 * @param index The index to retrieve the state.
 * @return The state at the index provided.
 * @throws StateException The index is not valid.
 */
State *JointState::get_state(int index) const
{
	if (index < 0 || index >= states.size()) {
		throw StateException();
	}
	return states[index];
}

/**
 * Overload the equals operator to set this state equal to the state provided.
 * @param other The joint state to copy.
 * @return The new version of this state.
 */
JointState &JointState::operator=(const JointState &other)
{
	states = other.get();
	name = other.get_name();
	return *this;
}

/**
 * Overload the equality comparison operator.
 * @param other The joint state to compare.
 * @return Returns @code{true} if this state is equal to the other; @code{false} otherwise.
 */
bool JointState::operator==(const JointState &other)
{
	int counter = 0;
	for (State *state : states) {
		if (*state == *(other.get_state(counter))) {
			return false;
		}
		counter++;
	}
	return true;
}

/**
 * Overload the less than operator for comparison.
 * @param other The joint state to compare.
 * @return Returns @code{true} if this state is less than the other; @code{false} otherwise.
 */
bool JointState::operator<(const JointState &other) const
{
	return name < other.get_name();
}
