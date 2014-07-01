/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#include "../../include/management/raw_file.h"

#include "../../include/utilities/log.h"

#include "../../include/core/core_exception.h"

RawFile::RawFile()
{ }

RawFile::~RawFile()
{ }

MDP *RawFile::load_raw_mdp(std::string filename)
{
	// Open the file stream and make sure the file is found; throw an exception otherwise.
	std::ifstream file(filename);

	if (!file.is_open()) {
		log_message("RawFile::load_raw_mdp", "Failed to open file '" + filename + "'.");
		throw CoreException();
	}

	// Attempt to read the header information: number of states (n), number of actions (m),
	// rewards type (r), the initial state (s0), and the horizon (h).
	unsigned int n = 0;
	unsigned int m = 0;
	unsigned int r = 0; // 0 = SRewards, 1 = SARewards, 2 = SASRewards
	unsigned int s0 = 0;
	unsigned int h = 0; // 0 = infinite

	try {
		file >> n >> m >> r >> s0 >> h;
	} catch (std::exception &err) {
		log_message("RawFile::load_raw_mdp",
				"Failed to read number of states and actions from file '" + filename + "'.");
		throw CoreException();
	}

	// Ensure n, m, r are valid.
	if (n == 0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid number of states in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (m == 0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid number of actions in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (r >= 3) {
		log_message("RawFile::load_raw_mdp",
				"Invalid rewards type in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (s0 >= n) {
		log_message("RawFile::load_raw_mdp",
				"Invalid initial state in the header for file '" + filename + "'.");
		throw CoreException();
	}

	// Note: All values of an unsigned int are valid horizon values.

	// Attempt to read in the state transition blocks.
	float *T = new float[n * m * n];
	for (int s = 0; s < n; s++) {
		load_data(file, m, n, T, s * m * n);
	}

	// Based on the rewards type, load them differently.
	float *R = nullptr;
	if (r == 0) {
		// ToDo: Implement this after creating SRewards.
		throw CoreException();
	} else if (r == 1) {
		// Attempt to read in the SA reward blocks.
		R = new float[n * m];
		load_data(file, n, m, R, 0);
	} else if (r == 2) {
		// Attempt to read in the SAS reward blocks.
		R = new float[n * m * n];
		for (int s = 0; s < n; s++) {
			load_data(file, m, n, R, s * m * n);
		}
	}

	// ToDo: Now that all the variables have been loaded, create the MDP.

	return nullptr;
}

void RawFile::save_raw_mdp(const MDP *mdp, std::string filename)
{
	// ToDo: Implement.
}

void RawFile::load_data(std::ifstream &file, unsigned int rows, unsigned int cols, float *array, unsigned int offset)
{
	// ToDo: Implement.
}
