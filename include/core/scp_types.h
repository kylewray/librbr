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


#ifndef SCP_TYPES_H
#define SCP_TYPES_H


/**
 * The enumeration for agent data types. It may be:
 * - an integer, specifying the number of agents
 * - a vector of strings, which provide agent identifiers
 */
enum AgentDataType {
	INTEGER,
	IDENTIFIER,
	NUM_TYPES
};


/**
 * The struct for agents. The data may be:
 * - an integer, specifying the number of agents
 * - a vector of strings, which provide agent identifiers
 */
struct Agents {

	/**
	 * The type of agents which are stored.
	 */
	AgentDataType dataType;

	/**
	 * A pointer to the data which defines the agents, depending on the storage type.
	 */
	void *data;

};


/**
 * The enumeration for state data types. It may be:
 * - an integer, specifying the number of states
 * - a vector of strings, which provide state identifiers
 * - a tuple of integers, providing the factored form, each element denoting a number
 * 		of states
 * - a tuple of vectors of strings, which define the factored form, each element
 * 		denoting the state identifiers within the factor
 */
enum StateDataType {
	INTEGER,
	IDENTIFIER,
	FACTORED_INTEGER,
	FACTORED_IDENTIFIER,
	NUM_STATE_TYPES
};


/**
 * The struct for states. The data may be:
 * - an integer specifying the number of states
 * - a vector of strings which provide state identifiers
 * - a tuple of integers, providing the factored form, each element denoting a number
 * 		of states
 * - a tuple of vectors of strings, which define the factored form, each element
 * 		denoting the state identifiers within the factor
 */
struct States {

	/**
	 * The type of states which are stored.
	 */
	StateDataType dataType;

	/**
	 * A pointer to the data which defines the states, depending on the storage type.
	 */
	void *data;

};


struct Actions {

};


struct Observations {

};


struct StateTransitions {

};


struct ObservationTransitions {

};


struct Rewards {

};


struct InitialState {

};


struct Horizon {

};


#endif // SCP_TYPES_H
