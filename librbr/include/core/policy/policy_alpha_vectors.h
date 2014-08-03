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


#ifndef POLICY_ALPHA_VECTORS_H
#define POLICY_ALPHA_VECTORS_H


#include <vector>
#include <string>
#include <map>

#include "policy.h"
#include "policy_alpha_vector.h"

#include "../states/state.h"
#include "../states/states_map.h"
#include "../actions/action.h"
#include "../actions/actions_map.h"
#include "../observations/observation.h"
#include "../observations/observations_map.h"
#include "../horizon.h"

/**
 * A policy for a set of alpha vectors, which supports finite or infinite horizon. It allows
 * for the resultant action given a belief point (which can be updated externally).
 *
 * Importantly, this class manages all alpha vectors once provided a list of them. All methods
 * which set alpha vectors automatically assume the user has relinquished control of memory
 * management once passed into the method.
 */
class PolicyAlphaVectors : public Policy {
public:
	/**
	 * The default constructor for a PolicyAlphaVectors object. It defaults to a horizon of 1.
	 */
	PolicyAlphaVectors();

	/**
	 * A constructor for a PolicyAlphaVectors object which specifies the horizon.
	 * @param	horizon		The horizon of the problem; 0 represents infinite horizon.
	 */
	PolicyAlphaVectors(unsigned int horizon);

	/**
	 * A constructor for a PolicyAlphaVectors object which specifies the horizon.
	 * @param	horizon		The horizon object from the MDP-like object.
	 */
	PolicyAlphaVectors(const Horizon *horizon);

	/**
	 * A constructor for a PolicyAlphaVectors object which specifies the alpha vectors.
	 * It defaults to a horizon of 1 in this case.
	 * @param	alphas		The set of alpha vectors.
	 */
	PolicyAlphaVectors(const std::vector<PolicyAlphaVector *> &alphas);

	/**
	 * A constructor for a PolicyAlphaVectors object which specifies the alpha vectors.
	 * It defaults to a horizon of 1 in this case.
	 * @param	alphas		The set of alpha vectors at each horizon.
	 */
	PolicyAlphaVectors(const std::vector<std::vector<PolicyAlphaVector *> > &alphas);

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~PolicyAlphaVectors();

	/**
	 * Set the alpha vectors. For finite horizon, it assumes 0 by default. This frees the memory of
	 * previous alpha vectors.
	 * @param	alphas		The set of alpha vectors.
	 */
	void set(const std::vector<PolicyAlphaVector *> &alphas);

	/**
	 * Set the alpha vectors, allowing the explicit specification of the horizon. This frees
	 * the memory of previous alpha vectors.
	 * @param	horizon				The horizon to set.
	 * @param	alphas 				The set of alpha vectors.
	 * @throw	PolicyException		The horizon was invalid.
	 */
	void set(unsigned int horizon, const std::vector<PolicyAlphaVector *> &alphas);

	/**
	 * Get the action for a given belief state. For finite horizon, it assumes 0 by default.
	 * @param	belief				The belief state to retrieve a mapping.
	 * @throw	PolicyException		The policy was not defined for this state.
	 * @return	The action to take at the given belief state.
	 */
	const Action *get(const BeliefState *belief) const;

	/**
	 * Get the action for a given belief state, allowing the explicit specification of the horizon.
	 * @param	horizon				The horizon to set.
	 * @param	belief				The belief state to retrieve a mapping.
	 * @throw	PolicyException		The policy was not defined for this belief state, or horizon was invalid.
	 * @return	The action to take at the given belief state.
	 */
	const Action *get(unsigned int horizon, const BeliefState *belief) const;

	/**
	 * A function which must load a policy file.
	 * @param	filename		The name and path of the file to load.
	 * @param	states			The states object which contains the actual state objects to be mapped.
	 * @param	actions			The actions object which contains the actual action objects to be mapped.
	 * @param	observations	The observations object which contains the actual observation objects to be mapped.
	 * @param	horizon			The horizons object to ensure valid policy creation.
	 * @return	Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename, const StatesMap *states, const ActionsMap *actions,
			const ObservationsMap *observations, const Horizon *horizon);

	/**
	 * A function which must save a policy file.
	 * @param	filename 	The name and path of the file to save.
	 * @param	states		The states object which contains the actual state objects to be mapped.
	 * @return	Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	bool save(std::string filename, const StatesMap *states) const;

	/**
	 * Reset the alpha vectors, freeing the memory.
	 */
	virtual void reset();

	/**
	 * A static method to prune a set of alpha vectors. This will free the memory of the alpha vectors inside 'alphas',
	 * for those alpha vectors which are pruned.
	 * @param	S					The finite set of states.
	 * @param	alphas 				The set of alpha vectors for which dominated ones will be pruned in place.
	 * @throws	PolicyException		The states were invalid, there were zero alpha vectors, or the first alpha vector is null.
	 */
	static void prune_dominated(const StatesMap *S, std::vector<PolicyAlphaVector *> &alphas);

private:
	/**
	 * Defines the policy itself; it's the internal mapping from states to actions. There is
	 * one of these mappings for each horizon.
	 */
	std::vector<std::vector<PolicyAlphaVector *> > alphaVectors;

};


#endif // POLICY_ALPHA_VECTORS_H
