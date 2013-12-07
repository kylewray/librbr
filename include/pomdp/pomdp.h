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


#ifndef POMDP_H
#define POMDP_H


#include "../scp/scp.h"
#include "../scp/scp_types.h"


/**
 * An abstract class for a Partially Observable Markov Decision Process (MDP) within librbr.
 */
class POMDP : public SCP {

public:

	/**
	 * A constructor for the POMDP class.
	 */
	POMDP();

	/**
	 * A deconstructor for the POMDP class.
	 */
	~POMDP();

private:

	/**
	 * The states in the POMDP; e.g., an array of strings.
	 */
	States states;

	/**
	 * The actions in the POMDP; e.g., an array of strings.
	 */
	Actions actions;

	/**
	 * The observations in the POMDP; e.g., factored vectors of strings.
	 */
	Observations observations;

	/**
	 * The state transition function in the POMDP; e.g., a three-dimensional array mapping to a double.
	 */
	StateTransitions stateTransitions;

	/**
	 * The observation transition function in the POMDP; e.g., a three-dimensional array mapping to a double.
	 */
	ObservationTransitions observationTransitions;

	/**
	 * The reward function in the POMDP; e.g., a two-dimensional array mapping to a double.
	 */
	Rewards rewards;

	/**
	 * The initial state or initial belief state; e.g., factored initial state.
	 */
	InitialState initialState;

	/**
	 * The horizon, either a finite time or a discount factor.
	 */
	Horizon horizon;

};


#endif // POMDP_H
