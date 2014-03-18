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


#include "../../include/pomdp/pomdp_alpha_vector.h"

/**
 * The default constructor for the POMDPAlphaVector class.
 */
POMDPAlphaVector::POMDPAlphaVector()
{
	alphaVectorAction = nullptr;
}

/**
 * A constructor for the POMDPAlphaVector class which specifies the initial action.
 * @param action The action to take if this is the maximal alpha vector.
 */
POMDPAlphaVector::POMDPAlphaVector(const Action *action)
{
	alphaVectorAction = action;
}

/**
 * The copy constructor for the POMDPAlphaVector class.
 * @param other The other alpha vector to copy.
 */
POMDPAlphaVector::POMDPAlphaVector(const POMDPAlphaVector &other)
{

}

/**
 * A deconstructor for the POMDPAlphaVector class.
 */
POMDPAlphaVector::~POMDPAlphaVector()
{
	reset();
}

/**
 * Set the value of a state. Unset alpha values are assumed to be 0.
 * @param state	Set the value of this state.
 * @param value The alpha value of the state.
 */
void POMDPAlphaVector::set(const State *state, double value)
{
	alphaVector[state] = value;
}

/**
 * Get the value of a state. Unset alpha values are assumed to be 0.
 * @param state Get the value of this state.
 * @return The alpha vector's value of the state.
 */
double POMDPAlphaVector::get(const State *state) const
{
	std::map<const State *, double>::const_iterator result = alphaVector.find(state);
	if (result == alphaVector.end()) {
		return 0.0;
	} else {
		return result->second;
	}
}

/**
 * Set the action to take if this alpha vector is optimal for a belief state.
 */
void POMDPAlphaVector::set_action(const Action *action)
{
	alphaVectorAction = action;
}

/**
 * Get the action to take at this alpha vector.
 * @return The action to take if this alpha vector is optimal for a belief state.
 */
const Action *POMDPAlphaVector::get_action() const
{
	return alphaVectorAction;
}

/**
 * Compute the value of the belief state by computing: dot(beta, alpha).
 * @param belief The belief state 'beta' vector.
 * @return The value of the belief state provided.
 */
double POMDPAlphaVector::compute_value(const BeliefState *belief)
{
	// Perform the dot product: dot(beta, alpha), but do so with map objects.
	double value = 0.0;
	for (std::map<const State *, double>::value_type alpha : alphaVector) {
		value += alpha.second * belief->get(alpha.first);
	}
	return value;
}

/**
 * Overload the equals operator to set this alpha vector equal to the alpha vector provided.
 * @param other The alpha vector to copy.
 * @return The new version of this alpha vector.
 */
POMDPAlphaVector &POMDPAlphaVector::operator=(const POMDPAlphaVector &other)
{
	alphaVector = other.alphaVector;
	alphaVectorAction = other.get_action();
	return *this;
}

/**
 * Reset the alpha vector.
 */
void POMDPAlphaVector::reset()
{
	alphaVector.clear();
	alphaVectorAction = nullptr;
}
