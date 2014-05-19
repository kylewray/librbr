/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#include "../../include/core/horizon.h"

#include <algorithm>

/**
 * The constructor for the Horizon class.
 */
Horizon::Horizon()
{
	set_horizon(1);
	set_discount_factor(1.0);
}

/**
 * The constructor for the Horizon class which defines a finite horizon.
 * @param h The positive horizon to use.
 */
Horizon::Horizon(unsigned int h)
{
	set_horizon(h);
	set_discount_factor(1.0);
}

/**
 * The constructor for the Horizon class which defines an infinite horizon.
 * @param d The discount factor (gamma) value in [0, 1).
 */
Horizon::Horizon(double d)
{
	set_horizon(0);
	set_discount_factor(d);
}

/**
 * The deconstructor for the Horizon class.
 */
Horizon::~Horizon()
{ }

/**
 * Return true if the horizon is finite; false if it is infinite.
 * @return Return @code{true} if the horizon is infinite; @code{false} otherwise.
 */
bool Horizon::is_finite() const
{
	return horizon > 0;
}

/**
 * Get the horizon.
 * @return The horizon.
 */
unsigned int Horizon::get_horizon() const
{
	return horizon;
}

/**
 * Return the discount factor gamma.
 * @return The discount factor gamma in [0, 1].
 */
double Horizon::get_discount_factor() const
{
	return discountFactor;
}

/**
 * Set the horizon.
 * @param h The non-negative horizon to use. A horizon of 0 means infinite horizon.
 */
void Horizon::set_horizon(unsigned int h)
{
	horizon = h;
}

/**
 * Set the discount factor.
 * @param d The discount factor (gamma) value in [0, 1).
 */
void Horizon::set_discount_factor(double d)
{
	discountFactor = std::max(0.0, std::min(1.0, d));
}

/**
 * Reset the horizon to a default horizon of 1 (finite) with 1.0 discount factor.
 */
void Horizon::reset()
{
	horizon = 1;
	discountFactor = 1.0;
}
