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


#ifndef JOINT_STATE_H
#define JOINT_STATE_H


#include <string>
#include <vector>

#include "state.h"


/**
 * A joint state object. This is essentially a list of states, each state being a state object itself.
 */
class JointState : public State {
public:
	/**
	 * The default constructor of the JointState object.
	 */
	JointState();

	/**
	 * The constructor of the JointState object which allows initial specification of the
	 * number of joint states.
	 * @param numJointStates The number of joint states in the tuple.
	 */
	JointState(int numJointStates);

	/**
	 * The copy constructor of the JointState object.
	 * @param other The joint state to copy.
	 */
	JointState(const JointState &other);

	/**
	 * The default deconstructor of the JointState object.
	 */
	virtual ~JointState();

	/**
	 * Override set name (leave get_name() alone) to raise an error when it is called.
	 * @param newName The new name.
	 * @throws StateException This is no longer a valid function.
	 */
	virtual void set_name(std::string newName);

	/**
	 * Set the joint state given a list of states.
	 * @param jointState The list of states which define this joint state.
	 */
	virtual void set_joint_state(std::vector<State *> jointState);

	/**
	 * Get the joint state.
	 * @return The list of states.
	 */
	virtual std::vector<State *> get_joint_state() const;

	/**
	 * Get a particular state at a index.
	 * @param index The index to retrieve the state.
	 * @return The state at the index provided.
	 */
	virtual State *get_state(int index) const;

	/**
	 * Overload the equals operator to set this state equal to the state provided.
	 * @param other The joint state to copy.
	 * @return The new version of this state.
	 */
	virtual JointState &operator=(const JointState &other);

	/**
	 * Overload the equality comparison operator.
	 * @param other The joint state to compare.
	 * @return Returns @code{true} if this state is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const JointState &other);

	/**
	 * Overload the less than operator for comparison.
	 * @param other The joint state to compare.
	 * @return Returns @code{true} if this state is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const JointState &other) const;

protected:
	/**
	 * The joint state is defined as a tuple of states. To remain general, this is a vector, but
	 * should remain a fixed size.
	 */
	std::vector<State *> states;

};


#endif // JOINT_STATE_H
