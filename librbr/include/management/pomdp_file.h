/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2015 Kyle Hollins Wray, University of Massachusetts
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


#ifndef POMDP_FILE_H
#define POMDP_FILE_H


#include "../mdp/mdp.h"
#include "../pomdp/pomdp.h"
#include "../dec_pomdp/dec_pomdp.h"

#include "../core/states/belief_state.h"

#include <fstream>

/**
 * A class which provides functionality save a Cassandra POMDP file into a array-based Markovian object.
 */
class POMDPFile {
public:
	/**
	 * The default constructor for a POMDPFile object.
	 */
	POMDPFile();

	/**
	 * The default deconstructor for a POMDPFile object.
	 */
	virtual ~POMDPFile();

	/**
	 * A method which simply saves *any* MDP object as a Cassandra MDP file.
	 * @param	mdp				The MDP object to save.
	 * @param	filename		The name of the output file.
	 * @throw	CoreException	An error arose trying to save the MDP object. This could be
	 * 							an invalid mdp was provided, or the filename was invalid.
	 * ToDo: Implement saving pomdp files.
	 */
//	void save_mdp(MDP *mdp, std::string filename);

	/**
	 * A method which simply saves *any* POMDP object as a Cassandra POMDP file.
	 * @param	pomdp			The POMDP object to save.
	 * @param	filename		The name of the output file.
	 * @param	start			The starting belief state.
	 * @throw	CoreException	An error arose trying to save the POMDP object. This could
	 * 							be an invalid mdp was provided, or the filename was invalid.
	 */
	void save_pomdp(POMDP *pomdp, std::string filename, BeliefState *start);

	/**
	 * A method which simply saves *any* Dec-POMDP object as a raw Dec-POMDP file.
	 * @param	decpomdp		The Dec-POMDP object to save.
	 * @param	filename		The name of the output file.
	 * @throw	CoreException	An error arose trying to save the Dec-POMDP object. This could
	 * 							be an invalid mdp was provided, or the filename was invalid.
	 * ToDo: Implement saving dec-pomdp files.
	 */
//	void save_decpomdp(DecPOMDP *decpomdp, std::string filename);

};

#endif // POMDP_FILE_H
