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


#include "../../../include/core/observations/observation_utilities.h"
#include "../../../include/core/observations/observation_exception.h"

#include "../../../include/core/observations/named_observation.h"
#include "../../../include/core/observations/joint_observation.h"

const Observation *find_observation(const FiniteObservations *Z, std::string observationName)
{
	for (auto z : *Z) {
		const Observation *observation = resolve(z);

		// Try to cast the observation as a NamedObservation. If it fails, it may still be a joint observation.
	    const NamedObservation *o = dynamic_cast<const NamedObservation *>(observation);
	    if (o == nullptr) {
	    	// Try to cast it as a JointObservation.
	    	const JointObservation *jo = dynamic_cast<const JointObservation *>(observation);
		    if (jo == nullptr) {
		    	throw ObservationException();
		    }

		    // Construct the joint observation name from the component names, separated by spaces.
		    std::string joName = "";
	    	for (int i = 0; i < jo->get_num_observations(); i++) {
	    		o = dynamic_cast<const NamedObservation *>(jo->get(i));
	    		if (o == nullptr) {
	    			throw ObservationException();
	    		}

	    		joName += o->get_name();
	    		if (i < jo->get_num_observations() - 1) {
	    			joName += " ";
	    		}
	    	}

    		if (joName.compare(observationName) == 0) {
	    		return jo;
    		}
	    } else {
			if (o->get_name().compare(observationName) == 0) {
				return observation;
			}
	    }
	}

	throw ObservationException();
}
