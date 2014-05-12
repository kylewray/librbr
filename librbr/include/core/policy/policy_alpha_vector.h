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


#ifndef POLICY_ALPHA_VECTOR_H
#define POLICY_ALPHA_VECTOR_H


#include "../actions/action.h"
#include "../states/belief_state.h"

#include <vector>

/**
 * A small class to manipulate alpha vectors for POMDPs and Dec-POMDPs. This stores an
 * action to take should this alpha vector define the maximal value of the belief state
 * following equation: dot(beta, alpha) = V(b), where beta = <b(s_1), ..., b(s_n)>.
 */
class PolicyAlphaVector {
public:
	/**
	 * The default constructor for the PolicyAlphaVector class.
	 */
	PolicyAlphaVector();

	/**
	 * A constructor for the PolicyAlphaVector class which specifies the initial action.
	 * @param action The action to take if this is the maximal alpha vector.
	 */
	PolicyAlphaVector(const Action *action);

	/**
	 * The copy constructor for the PolicyAlphaVector class.
	 * @param other The other alpha vector to copy.
	 */
	PolicyAlphaVector(const PolicyAlphaVector &other);

	/**
	 * A deconstructor for the PolicyAlphaVector class.
	 */
	virtual ~PolicyAlphaVector();

	/**
	 * Set the value of a state. Unset alpha values are assumed to be 0.
	 * @param state	Set the value of this state.
	 * @param value The alpha value of the state.
	 */
	void set(const State *state, double value);

	/**
	 * Get the value of a state. Unset alpha values are assumed to be 0.
	 * @param state Get the value of this state.
	 * @return The alpha vector's value of the state.
	 */
	double get(const State *state) const;

	/**
	 * Set the action to take if this alpha vector is optimal for a belief state.
	 */
	void set_action(const Action *action);

	/**
	 * Get the action to take at this alpha vector.
	 * @return The action to take if this alpha vector is optimal for a belief state.
	 */
	const Action *get_action() const;

	/**
	 * Get the dimension of this alpha vector (which is the number of states).
	 * @return The dimension of this alpha vector.
	 */
	int get_dimension() const;

	/**
	 * Compute the value of the belief state by computing: dot(beta, alpha).
	 * @param belief The belief state 'beta' vector.
	 * @return The value of the belief state provided.
	 */
	double compute_value(const BeliefState *belief) const;

	/**
	 * Overload the equals operator to set this alpha vector equal to the alpha vector provided.
	 * @param other The alpha vector to copy.
	 * @return The new version of this alpha vector.
	 */
	PolicyAlphaVector &operator=(const PolicyAlphaVector &other);

	/**
	 * Overload the plus operator to return the summation of all elements in the vectors.
	 * @param other The alpha vector to add to this one.
	 * @return The resultant alpha vector from the sum of this one and the other one provided.
	 */
	PolicyAlphaVector operator+(const PolicyAlphaVector &other) const;

	/**
	 * Overload the minus operator to return the subtraction of all elements in the vectors.
	 * @param other The alpha vector to subtract to this one.
	 * @return The resultant alpha vector from the element-wise subtraction of this one and the other one provided.
	 */
	PolicyAlphaVector operator-(const PolicyAlphaVector &other) const;

	/**
	 * Reset the alpha vector.
	 */
	void reset();

	/**
	 * Compute the cross-sum (Minkowski sum) of two alpha vectors. This creates new PolicyAlphaVector in memory.
	 * @param A The left set of alpha vectors.
	 * @param B The right set of alpha vectors.
	 * @return The result from performing the cross-sum on the two sets of alpha vectors.
	 */
	static std::vector<PolicyAlphaVector *> cross_sum(const std::vector<PolicyAlphaVector *> &A, const std::vector<PolicyAlphaVector *> &B);

private:
	/**
	 * The alpha vector values: alpha = <V(s_1), ..., V(s_n)>. Unset alpha values are
	 * assumed to be 0.
	 */
	std::map<const State *, double> alphaVector;

	/**
	 * The action to take if this alpha vector is optimal for a belief state.
	 */
	const Action *alphaVectorAction;

};


#endif // POLICY_ALPHA_VECTOR_H
