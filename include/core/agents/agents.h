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


#ifndef AGENTS_H
#define AGENTS_H


/**
 * An abstract class to contain agents. Internally, agents can simply be represented
 * as a number or even with a identifier. The main objective of the class is to abstract
 * the representation of agents and provide simple function which restructure the
 * internal representation into any representation needed by a solver. The template type
 * T defines how the user will interact with the class, not necessarily the internal
 * representation.
 */
template <class T>
class Agents {
public:
	/**
	 * The default constructor for the Agents class.
	 */
	Agents();

	/**
	 * The deconstructor for the Agents class.
	 */
	virtual ~Agents();

	/**
	 * Define the internal representation of agents given a vector of identifiers.
	 * @param identifiers A vector of agent unique identifiers (e.g., names).
	 */
	virtual void define(std::vector<T> identifiers);

	/**
	 * Convert the internal representation to a vector of agents.
	 * @return A vector of agent unique identifiers (e.g., names).
	 */
	virtual std::vector<T> all();

	/**
	 * Clear and reset the internal variables (freeing memory, if necessary).
	 */
	virtual void clear();

	/**
	 * Get a particular agent's identifier.
	 * @param index The integer index of the agent to retrieve.
	 * @return A string identifier corresponding to the index.
	 */
	virtual T operator[](int index) const;

	/**
	 * Get a reference to a particular agent's identifier (often for setting).
	 * @param index The integer index of the agent to retrieve.
	 * @return A string identifier corresponding to the index.
	 */
	virtual T &operator[](int index);

private:

};


#endif // AGENTS_H
