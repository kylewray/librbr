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


#ifndef POMDP_ALPHA_VECTOR_H
#define POMDP_ALPHA_VECTOR_H


#include "../core/actions/action.h"
#include "../core/states/belief_state.h"

/**
 * A small class to manipulate alpha vectors for POMDPs. This stores an action
 * to take should this alpha vector define the maximal value of the belief state
 * following equation: dot(beta, alpha) = V(b), where beta = <b(s_1), ..., b(s_n)>.
 */
class POMDPAlphaVector {
public:
	/**
	 * The default constructor for the POMDPAlphaVector class.
	 */
	POMDPAlphaVector();

	/**
	 * A constructor for the POMDPAlphaVector class which specifies the initial action.
	 * @param action The action to take if this is the maximal alpha vector.
	 */
	POMDPAlphaVector(const Action *action);

	/**
	 * The copy constructor for the POMDPAlphaVector class.
	 * @param other The other alpha vector to copy.
	 */
	POMDPAlphaVector(const POMDPAlphaVector &other);

	/**
	 * A deconstructor for the POMDPAlphaVector class.
	 */
	virtual ~POMDPAlphaVector();

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
	 * Compute the value of the belief state by computing: dot(beta, alpha).
	 * @param belief The belief state 'beta' vector.
	 * @return The value of the belief state provided.
	 */
	double compute_value(const BeliefState *belief);

	/**
	 * Overload the equals operator to set this alpha vector equal to the alpha vector provided.
	 * @param other The alpha vector to copy.
	 * @return The new version of this alpha vector.
	 */
	POMDPAlphaVector &operator=(const POMDPAlphaVector &other);

	/**
	 * Reset the alpha vector.
	 */
	void reset();

	/**
	 * Allow POMDPAlphaVector objects to access private members in others for setting.
	 */
	friend POMDPAlphaVector;

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


#endif // POMDP_ALPHA_VECTOR_H
