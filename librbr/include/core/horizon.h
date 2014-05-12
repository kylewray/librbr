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


#ifndef HORIZON_H
#define HORIZON_H


/**
 * The horizon of all MDP-like objects can be either finite or infinite. In the
 * finite horizon case, a number of stages must be specified. In the infinite
 * horizon case, a discount factor must be specified. The discount factor is
 * optional in the finite horizon case.
 */
class Horizon {
public:
	/**
	 * The constructor for the Horizon class.
	 */
	Horizon();

	/**
	 * The constructor for the Horizon class which defines a finite horizon.
	 * @param h The positive horizon to use.
	 */
	Horizon(unsigned int h);

	/**
	 * The constructor for the Horizon class which defines an infinite horizon.
	 * @param d The discount factor (gamma) value in [0, 1).
	 */
	Horizon(double d);

	/**
	 * The deconstructor for the Horizon class.
	 */
	virtual ~Horizon();

	/**
	 * Return true if the horizon is finite; false if it is infinite.
	 * @return Return @code{true} if the horizon is infinite; @code{false} otherwise.
	 */
	bool is_finite() const;

	/**
	 * Get the horizon.
	 * @return The horizon.
	 */
	unsigned int get_horizon() const;

	/**
	 * Return the discount factor gamma.
	 * @return The discount factor gamma in [0, 1].
	 */
	double get_discount_factor() const;

	/**
	 * Set the horizon.
	 * @param h The non-negative horizon to use. A horizon of 0 means infinite horizon.
	 */
	void set_horizon(unsigned int h);

	/**
	 * Set the discount factor.
	 * @param d The discount factor (gamma) value in [0, 1).
	 */
	void set_discount_factor(double d);

	/**
	 * Reset the horizon to a default horizon of 1 (finite) with 1.0 discount factor.
	 */
	void reset();

private:
	/**
	 * The horizon. A horizon of 0 means infinite horizon.
	 */
	unsigned int horizon;

	/**
	 * The discount factor in the range of [0, 1].
	 */
	double discountFactor;

};


#endif // HORIZON_H
