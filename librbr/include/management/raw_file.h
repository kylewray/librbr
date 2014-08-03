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


#ifndef RAW_FILE_H
#define RAW_FILE_H


#include "../mdp/mdp.h"
#include "../pomdp/pomdp.h"
#include "../dec_pomdp/dec_pomdp.h"

#include <fstream>

/**
 * An enumeration for the types of rewards that are possible.
 */
enum RawFileRewardsType {
	RawFileSRewards,
	RawFileSARewards,
	RawFileSASRewards,
	RawFileSASORewards,
	NumRawFileRewardsTypes
};

/**
 * A class which provides functionality load a raw Markovian file into a array-based Markovian object,
 * as well as save *any* Markovian object as a the appropriate raw Markovian file.
 */
class RawFile {
public:
	/**
	 * The default constructor for a RawFile object.
	 */
	RawFile();

	/**
	 * The default deconstructor for a RawFile object.
	 */
	virtual ~RawFile();

	/**
	 * A method which simply loads a raw MDP file into an array-based MDP object.
	 * @param	filename		The name of the input file.
	 * @throw	CoreException	An error arose trying to save the MDP object. This is
	 * 							either due to an error within the file, or the file itself
	 * 							was not able to be loaded.
	 */
	MDP *load_raw_mdp(std::string filename);

	/**
	 * A method which simply saves *any* MDP object as a raw MDP file.
	 * @param	mdp				The MDP object to save.
	 * @param	filename		The name of the output file.
	 * @throw	CoreException	An error arose trying to save the MDP object. This could be
	 * 							an invalid mdp was provided, or the filename was invalid.
	 */
	void save_raw_mdp(const MDP *mdp, std::string filename);

	/**
	 * A method which simply loads a raw MDP file into an array-based MDP object.
	 * @param	filename		The name of the input file.
	 * @throw	CoreException	An error arose trying to save the POMDP object. This is
	 * 							either due to an error within the file, or the file itself
	 * 							was not able to be loaded.
	 * ToDo: Implement loading raw pomdp files.
	 */
//	POMDP *load_raw_pomdp(std::string filename);

	/**
	 * A method which simply saves *any* POMDP object as a raw POMDP file.
	 * @param	pomdp			The POMDP object to save.
	 * @param	filename		The name of the output file.
	 * @throw	CoreException	An error arose trying to save the POMDP object. This could
	 * 							be an invalid mdp was provided, or the filename was invalid.
	 * ToDo: Implement saving raw pomdp files.
	 */
//	void save_raw_pomdp(const POMDP *pomdp, std::string filename);

	/**
	 * A method which simply loads a raw MDP file into an array-based MDP object.
	 * @param	filename		The name of the input file.
	 * @throw	CoreException	An error arose trying to save the Dec-POMDP object. This is
	 * 							either due to an error within the file, or the file itself
	 * 							was not able to be loaded.
	 * ToDo: Implement loading raw dec-pomdp files.
	 */
//	DecPOMDP *load_raw_decpomdp(std::string filename);

	/**
	 * A method which simply saves *any* Dec-POMDP object as a raw Dec-POMDP file.
	 * @param	decpomdp		The Dec-POMDP object to save.
	 * @param	filename		The name of the output file.
	 * @throw	CoreException	An error arose trying to save the Dec-POMDP object. This could
	 * 							be an invalid mdp was provided, or the filename was invalid.
	 * ToDo: Implement saving raw dec-pomdp files.
	 */
//	void save_raw_decpomdp(const DecPOMDP *decpomdp, std::string filename);

private:
	/**
	 * Load a matrix of data to into the 1-d array provided, given the offset provided.
	 * @param	file			The file stream.
	 * @param	rows			The number of rows to read.
	 * @param	cols			The number of columns to read for each row.
	 * @param	array			The array to modify.
	 * @param	offset			The offset from the start of the array.
	 * @throw	CoreException	Either ran out of lines, or a parsing error arose.
	 */
	void load_data(std::ifstream &file, unsigned int rows, unsigned int cols, float *array, unsigned int offset);

};

#endif // RAW_FILE_H
