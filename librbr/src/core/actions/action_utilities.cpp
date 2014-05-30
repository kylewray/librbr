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


#include "../../../include/core/actions/action_utilities.h"
#include "../../../include/core/actions/action_exception.h"

#include "../../../include/core/actions/named_action.h"
#include "../../../include/core/actions/joint_action.h"

const Action *find_action(const FiniteActions *A, std::string actionName)
{
	for (const Action *action : *A) {
		// Try to cast the action as a NamedAction. If it fails, it may still be a joint action.
	    const NamedAction *a = dynamic_cast<const NamedAction *>(action);
	    if (a == nullptr) {
	    	// Try to cast it as a JointAction.
	    	const JointAction *ja = dynamic_cast<const JointAction *>(action);
		    if (ja == nullptr) {
		    	throw ActionException();
		    }

		    // Construct the joint action name from the component names, separated by spaces.
		    std::string jaName = "";
	    	for (int i = 0; i < ja->get_num_actions(); i++) {
	    		a = dynamic_cast<const NamedAction *>(ja->get(i));
	    		if (a == nullptr) {
	    			throw ActionException();
	    		}

	    		jaName += a->get_name();
	    		if (i < ja->get_num_actions() - 1) {
	    			jaName += " ";
	    		}
	    	}

    		if (jaName.compare(actionName) == 0) {
	    		return ja;
    		}
	    } else {
			if (a->get_name().compare(actionName) == 0) {
				return action;
			}
	    }
	}

	throw ActionException();
}
