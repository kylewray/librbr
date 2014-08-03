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


#include "../../include/core/horizon.h"

#include <algorithm>

Horizon::Horizon()
{
	set_horizon(1);
	set_discount_factor(1.0);
}

Horizon::Horizon(unsigned int h)
{
	set_horizon(h);
	set_discount_factor(1.0);
}

Horizon::Horizon(double d)
{
	set_horizon(0);
	set_discount_factor(d);
}

Horizon::~Horizon()
{ }

bool Horizon::is_finite() const
{
	return horizon > 0;
}

unsigned int Horizon::get_horizon() const
{
	return horizon;
}

double Horizon::get_discount_factor() const
{
	return discountFactor;
}

void Horizon::set_horizon(unsigned int h)
{
	horizon = h;
}

void Horizon::set_discount_factor(double d)
{
	discountFactor = std::max(0.0, std::min(1.0, d));
}

void Horizon::reset()
{
	horizon = 1;
	discountFactor = 1.0;
}
