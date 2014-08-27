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


#ifndef JOINT_ACTION_H
#define JOINT_ACTION_H


#include <string>
#include <vector>

#include "action.h"

/**
 * A joint action object. This is essentially a list of actions, each action being an action object itself.
 * Note: This class does *not* manage the memory of the actions provided. Memory should be managed in
 * a class such as FiniteJointActions.
 */
class JointAction : virtual public Action {
public:
	/**
	 * The constructor of the JointAction object which allows initial specification of the
	 * number of joint actions.
	 * @param	numJointActions		The number of joint actions in the tuple.
	 */
	JointAction(int numJointActions);

	/**
	 * The constructor of the JointAction object which allows initial specification of the
	 * actual joint action tuple (vector).
	 * @param	jointState		The list of actions which define this joint action.
	 */
	JointAction(const std::vector<const Action *> &jointAction);

	/**
	 * The copy constructor of the JointAction object.
	 * @param	other	The joint action to copy.
	 */
	JointAction(const JointAction &other);

	/**
	 * The default deconstructor of the JointAction object.
	 */
	virtual ~JointAction();

	/**
	 * Set the joint action given a list of actions.
	 * @param	jointAction		The list of actions which define this joint action.
	 */
	virtual void set(const std::vector<const Action *> &jointAction);

	/**
	 * Get the joint action.
	 * @return	The list of actions.
	 */
	virtual const std::vector<const Action *> &get() const;

	/**
	 * Get a particular action at a index.
	 * @param	index	The index to retrieve the action.
	 * @return	The action at the index provided.
	 */
	virtual const Action *get(int index) const;

	/**
	 * Get the number of actions within the joint action.
	 * @return	The number of actions within the joint action.
	 */
	virtual int get_num_actions() const;

	/**
	 * Overload the equals operator to set this joint action equal to the state provided.
	 * @param	other				The joint action to copy.
	 * @param	ActionException		The other action is not a JointAction.
	 * @return	The new version of this action.
	 */
	virtual Action &operator=(const Action &other);

	/**
	 * Returns a string representation of this action.
	 * @return	Returns the string representing this action.
	 */
	virtual std::string to_string() const;

	/**
	 * Returns a hash value used to quickly identify this action in a collection of actions.
	 * @return	Returns the hash value of this action.
	 */
	virtual unsigned int hash_value() const;

protected:
	/**
	 * The joint action is defined as a tuple of actions. To remain general, this is a vector, but
	 * should remain a fixed size. This class does *not* manage the memory of these action objects.
	 */
	std::vector<const Action *> actions;

};


#endif // JOINT_ACTION_H
