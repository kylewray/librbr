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


#include "../../../include/core/policy/policy_alpha_vector.h"

PolicyAlphaVector::PolicyAlphaVector()
{
	alphaVectorAction = nullptr;
}

PolicyAlphaVector::PolicyAlphaVector(Action *action)
{
	alphaVectorAction = action;
}

PolicyAlphaVector::PolicyAlphaVector(PolicyAlphaVector &other)
{
	*this = other;
}

PolicyAlphaVector::~PolicyAlphaVector()
{
	reset();
}

void PolicyAlphaVector::set(State *state, double value)
{
	alphaVector[state] = value;
}

double PolicyAlphaVector::get(State *state)
{
	std::map<State *, double>::const_iterator result = alphaVector.find(state);
	if (result == alphaVector.end()) {
		return 0.0;
	} else {
		return result->second;
	}
}

void PolicyAlphaVector::set_action(Action *action)
{
	alphaVectorAction = action;
}

Action *PolicyAlphaVector::get_action()
{
	return alphaVectorAction;
}

int PolicyAlphaVector::get_dimension() const
{
	return alphaVector.size();
}

double PolicyAlphaVector::compute_value(BeliefState *belief)
{
	// Perform the dot product: dot(beta, alpha), but do so with map objects.
	double value = 0.0;
	for (State *s : belief->get_states()) {
		value += alphaVector[s] * belief->get(s);
	}
//	for (std::map<State *, double>::value_type alpha : alphaVector) {
//		value += alpha.second * belief->get(alpha.first);
//	}
	return value;
}

PolicyAlphaVector &PolicyAlphaVector::operator=(PolicyAlphaVector &other)
{
	alphaVector = other.alphaVector;
	alphaVectorAction = other.get_action();
	return *this;
}

PolicyAlphaVector PolicyAlphaVector::operator+(PolicyAlphaVector &other)
{
	PolicyAlphaVector result;

	for (std::map<State *, double>::value_type alpha : alphaVector) {
		double a = alpha.second;
		double b = other.get(alpha.first);
		result.set(alpha.first, a + b);
	}

	return result;
}

PolicyAlphaVector &PolicyAlphaVector::operator+=(PolicyAlphaVector &other)
{
	for (std::map<State *, double>::value_type &alpha :alphaVector) {
		alpha.second += other.get(alpha.first);
	}

	return *this;
}

PolicyAlphaVector PolicyAlphaVector::operator-(PolicyAlphaVector &other)
{
	PolicyAlphaVector result;

	for (std::map<State *, double>::value_type alpha : alphaVector) {
		double a = alpha.second;
		double b = other.get(alpha.first);
		result.set(alpha.first, a - b);
	}

	return result;
}

PolicyAlphaVector &PolicyAlphaVector::operator-=(PolicyAlphaVector &other)
{
	for (std::map<State *, double>::value_type &alpha :alphaVector) {
		alpha.second -= other.get(alpha.first);
	}

	return *this;
}

void PolicyAlphaVector::reset()
{
	alphaVector.clear();
	alphaVectorAction = nullptr;
}

std::vector<PolicyAlphaVector *> PolicyAlphaVector::cross_sum(std::vector<PolicyAlphaVector *> &A,
		std::vector<PolicyAlphaVector *> &B)
{
	std::vector<PolicyAlphaVector *> C;

	// Handle two fringe cases, and then the normal case.
	if (A.empty()) {
		for (PolicyAlphaVector *b : B) {
			PolicyAlphaVector *c = new PolicyAlphaVector(*b);
			C.push_back(c);
		}
	} else if (B.empty()) {
		for (PolicyAlphaVector *a : A) {
			PolicyAlphaVector *c = new PolicyAlphaVector(*a);
			C.push_back(c);
		}
	} else {
		// Perform the cross-sum and store the result in C.
		for (PolicyAlphaVector *a : A) {
			for (PolicyAlphaVector *b : B) {
				PolicyAlphaVector *c = new PolicyAlphaVector(*a);
				*c += *b;
				C.push_back(c);
			}
		}
	}

	return C;
}
