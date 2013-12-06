/**
 * 	The MIT License (MIT)
 *
 * 	Copyright (c) 2013 Kyle Wray and Luis Pineda
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy of
 * 	this software and associated documentation files (the "Software"), to deal in
 * 	the Software without restriction, including without limitation the rights to
 * 	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * 	the Software, and to permit persons to whom the Software is furnished to do so,
 * 	subject to the following conditions:
 *
 * 	The above copyright notice and this permission notice shall be included in all
 * 	copies or substantial portions of the Software.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * 	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * 	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * 	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * 	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * An abstract class for all solutions to stochastic control problems (SCPs).
 */
class SCPSolution {

public:

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~SCPSolution() {}

	/**
	 * A virtual function which must load a SCP solution file.
	 *
	 * @param filename	The name of the file to load.
	 * @return			Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename) const = 0;

	/**
	 * A virtual function which must save a SCP solution file.
	 *
	 * @param filename	The name of the file to save.
	 * @return			Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename) const = 0;

};
