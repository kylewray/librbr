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


#ifndef CONVERSION_H
#define CONVERSION_H

#include "../core/states/states_map.h"
#include "../core/actions/actions_map.h"
#include "../core/observations/observations_map.h"
#include "../core/state_transitions/state_transitions.h"
#include "../core/observation_transitions/observation_transitions.h"
#include "../core/rewards/sas_rewards.h"
#include "../core/rewards/saso_rewards.h"

#include "../mdp/mdp.h"
#include "../pomdp/pomdp.h"

/**
 * Convert a map-based MDP to an array-based MDP.
 * @param	mdp				The map-based MDP, which must have map state
 * 							transitions and rewards.
 * @throw	CoreException	The MDP provided was invalid.
 * @return	The new array-based MDP, for its state transitions and rewards.
 */
MDP *convert_map_to_array(MDP *mdp);

/**
 * Convert the components of a map-based MDP to an array-based MDP.
 * @param	states				The states.
 * @param	actions				The actions.
 * @param	stateTransitions	The state transitions.
 * @param	rewards				The state-action-state rewards.
 * @param	initial				The initial state.
 * @param	horizon				The horizon.
 * @throw	CoreException		The MDP provided was invalid.
 * @return	The new array-based MDP, for its state transitions and rewards.
 */
MDP *convert_map_to_array(StatesMap *states, ActionsMap *actions,
		StateTransitions *stateTransitions, SASRewards *rewards,
		Horizon *horizon);

/**
 * Convert a map-based POMDP to an array-based POMDP.
 * @param	pomdp			The map-based POMDP, which must have map state
 * 							and observation transitions, as well as rewards.
 * @throw	CoreException	The POMDP provided was invalid.
 * @return	The new array-based POMDP, for its rewards and its state and
 * 			observation transitions.
 */
POMDP *convert_map_to_array(POMDP *pomdp);


#endif // CONVERSION_H
