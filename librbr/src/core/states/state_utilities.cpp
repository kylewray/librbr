/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
 *  Copyright (c) 2013-2014 Kyle Wray and Luis Pineda
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


#include "../../../include/core/states/state_utilities.h"
#include "../../../include/core/states/state_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/states/factored_state.h"

const State *find_state(const StatesMap *S, std::string stateName)
{
	for (auto stateIterator : *S) {
		const State *state = resolve(stateIterator);

		// Try to cast the state as a NamedState. If it fails, it may still be a factored state.
	    const NamedState *s = dynamic_cast<const NamedState *>(state);
	    if (s == nullptr) {
	    	// Try to cast it as a FactoredState.
	    	const FactoredState *fs = dynamic_cast<const FactoredState *>(state);
		    if (fs == nullptr) {
		    	throw StateException();
		    }

		    // Construct the factored state name from the component names, separated by spaces.
		    std::string fsName = "";
	    	for (int i = 0; i < fs->get_num_states(); i++) {
	    		s = dynamic_cast<const NamedState *>(fs->get(i));
	    		if (s == nullptr) {
	    			throw StateException();
	    		}

	    		fsName += s->get_name();
	    		if (i < fs->get_num_states() - 1) {
	    			fsName += " ";
	    		}
	    	}

    		if (fsName.compare(stateName) == 0) {
	    		return fs;
    		}
	    } else {
			if (s->get_name().compare(stateName) == 0) {
				return state;
			}
	    }
	}

	throw StateException();
}
