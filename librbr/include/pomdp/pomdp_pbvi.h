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


#ifndef POMDP_PBVI_H
#define POMDP_PBVI_H


#include "pomdp.h"

#include "../core/policy/policy_alpha_vectors.h"
#include "../core/policy/policy_alpha_vector.h"

#include "../core/states/states_map.h"
#include "../core/actions/actions_map.h"
#include "../core/observations/observations_map.h"
#include "../core/state_transitions/state_transitions_map.h"
#include "../core/observation_transitions/observation_transitions_map.h"
#include "../core/rewards/saso_rewards.h"
#include "../core/horizon.h"

/**
 * List the possible expansion rules available while using PBVI.
 */
enum POMDPPBVIExpansionRule {
	NONE,
	RANDOM_BELIEF_SELECTION,
	STOCHASTIC_SIMULATION_RANDOM_ACTION,
	STOCHASTIC_SIMULATION_GREEDY_ACTION,
	STOCHASTIC_SIMULATION_EXPLORATORY_ACTION,
	GREEDY_ERROR_REDUCTION,
	NUM_EXPANSION_RULES
};

/**
 * Solve an POMDP via Point-Based Value Iteration (PBVI) (finite or infinite horizon). This
 * solver has the following requirements:
 * - POMDP states must be of type FiniteStates.
 * - POMDP actions must be of type FiniteActions.
 * - POMDP observations must be of type FiniteObservations.
 * - POMDP state transitions must be of type FiniteStateTransitions.
 * - POMDP observation transitions must be of type FiniteObservationTransitions.
 * - POMDP rewards must be of type SASRewards.
 */
class POMDPPBVI {
public:
	/**
	 * The default constructor for the POMDPPBVI class. Default number of iterations for infinite
	 * horizon POMDPs is 1. The default expansion rule is Random Belief Selection.
	 */
	POMDPPBVI();

	/**
	 * A constructor for the POMDPPBVI class which allows for the specification of the expansion rule,
	 * and the number of iterations (both updates and expansions) to run for infinite horizon.
	 * The default is 1 for both.
	 * @param	expansionRule			The expansion rule to use.
	 * @param	updateIterations 		The number of update iterations to run for infinite horizon POMDPs.
	 * @param	expansionIterations 	The number of expansion iterations to run for infinite horizon POMDPs.
	 */
	POMDPPBVI(POMDPPBVIExpansionRule expansionRule, unsigned int updateIterations, unsigned int expansionIterations);

	/**
	 * The deconstructor for the POMDPPBVI class. This method frees all the belief state memory.
	 */
	virtual ~POMDPPBVI();

	/**
	 * Add an initial belief state which is used to seed the belief states before computing the optimal policy.
	 * Note: This relinquishes control of the belief state's memory management to this class.
	 * @param	b		The initial set of belief states before calling 'solve'.
	 */
	void add_initial_belief_state(BeliefState *b);

	/**
	 * Set the initial set of belief states which are used to seed the belief states before computing
	 * the optimal policy. Note: This relinquishes control of the belief states' memory management to this class.
	 * @param	initialBeliefStates		The initial set of belief states before calling 'solve'.
	 */
	void set_initial_belief_states(const std::vector<BeliefState *> &initialBeliefStates);

	/**
	 * Set the expansion rule to add belief points.
	 * @param	expansionRule		The expansion rule to use.
	 */
	void set_expansion_rule(POMDPPBVIExpansionRule expansionRule);

	/**
	 * Set the number of update iterations to run for infinite horizon POMDPs.
	 * @param	iterations		The number of update iterations to run for infinite horizon POMDPs.
	 */
	void set_num_update_iterations(unsigned int iterations);

	/**
	 * Set the number of expansion iterations to run for infinite horizon POMDPs.
	 * @param	iterations		The number of expansion iterations to run for infinite horizon POMDPs.
	 */
	void set_num_expansion_iterations(unsigned int iterations);

	/**
	 * Get the initial set of belief states which are used to seed the belief states before computing
	 * the optimal policy.
	 * @return	The initial set of belief states before calling 'solve'.
	 */
	std::vector<BeliefState *> &get_initial_belief_states();

	/**
	 * Get the set of belief states which were used to compute the optimal policy. This vector is only
	 * populated after calling 'solve'.
	 * @return	The final set of belief states after calling 'solve'.
	 */
	std::vector<BeliefState *> &get_belief_states();

	/**
	 * Set the expansion rule to add belief points.
	 * @param	expansionRule		The expansion rule to use.
	 */
	POMDPPBVIExpansionRule get_expansion_rule() const;

	/**
	 * Get the number of update iterations to run for infinite horizon POMDPs.
	 * @return	The number of update iterations to run for infinite horizon POMDPs.
	 */
	unsigned int get_num_update_iterations() const;

	/**
	 * Get the number of expansion iterations to run for infinite horizon POMDPs.
	 * @return	The number of expansion iterations to run for infinite horizon POMDPs.
	 */
	unsigned int get_num_expansion_iterations() const;

	/**
	 * Compute the optimal number of update iterations to run for infinite horizon POMDPs, given
	 * the desired tolerance, requiring knowledge of the reward function.
	 * @param	pomdp 				The partially observable Markov decision process to use.
	 * @param	epsilon				The desired tolerance between value functions to check for convergence.
	 * @throw	RewardException		The POMDP did not have a SASORewards rewards object.
	 */
	void compute_num_update_iterations(POMDP *pomdp, double epsilon);

	/**
	 * Solve the POMDP provided using point-based value iteration.
	 * @param	pomdp								The partially observable Markov decision process to solve.
	 * @throw	CoreException						The POMDP was null.
	 * @throw	StateException						The POMDP did not have a FiniteStates states object.
	 * @throw	ActionException						The POMDP did not have a FiniteActions actions object.
	 * @throw	ObservationException				The POMDP did not have a FiniteObservations observations object.
	 * @throw	StateTransitionsException			The POMDP did not have a FiniteStateTransitions state transitions object.
	 * @throw	ObservationTransitionsException		The POMDP did not have a FiniteObservationTransitions observation transitions object.
	 * @throw	RewardException						The POMDP did not have a SASRewards rewards object.
	 * @throw	PolicyException						An error occurred computing the policy.
	 * @return	Return the optimal policy as a finite state controller (infinite horizon) or tree (finite horizon).
	 */
	PolicyAlphaVectors *solve(POMDP *pomdp);

	/**
	 * Reset this POMDP PBVI solver. This method frees all the belief state memory.
	 */
	void reset();

protected:
	/**
	 * Solve a finite horizon POMDP using point-based value iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	Z					The finite observations.
	 * @param	T					The finite state transition function.
	 * @param	O					The finite observation transition function.
	 * @param	R					The state-action-state-observation rewards.
	 * @param	h					The horizon.
	 * @throw	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy as a collection of alpha vectors.
	 */
	PolicyAlphaVectors *solve_finite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitions *T, ObservationTransitions *O, SASORewards *R,
			Horizon *h);

	/**
	 * Solve an infinite horizon POMDP using point-based value iteration.
	 * @param	S					The finite states.
	 * @param	A					The finite actions.
	 * @param	Z					The finite observations.
	 * @param	T					The finite state transition function.
	 * @param	O					The finite observation transition function.
	 * @param	R					The state-action-state-observation rewards.
	 * @param	h					The horizon.
	 * @throw	PolicyException		An error occurred computing the policy.
	 * @return	Return the optimal policy as a collection of alpha vectors.
	 */
	PolicyAlphaVectors *solve_infinite_horizon(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitions *T, ObservationTransitions *O, SASORewards *R,
			Horizon *h);

	/**
	 * Expand the set of beliefs following Random Belief Selection. This works by randomly selecting a set of new
	 * belief points at each expansion. One new point is selected for each current belief point, doubling the total
	 * quantity each time.
	 * @param	S	The finite states.
	 */
	void expand_random_belief_selection(StatesMap *S);

	/**
	 * Expand the set of beliefs following Stochastic Simulation with Random Actions. "Stochastic Simulation" means it
	 * generates belief points which are reachable given the initial set of belief points, i.e., it traverses the belief
	 * tree. In this case, for each belief point it randomly selects a state, proportional to the belief, then randomly
	 * selects an action (uniformly), then randomly selects a next state and next observation. The result is a new belief
	 * point.
	 * @param	S	The finite states.
	 * @param	A	The finite actions.
	 * @param	Z	The finite observations.
	 * @param	T	The finite state transition function.
	 * @param	O	The finite observation transition function.
	 */
	void expand_stochastic_simulation_random_actions(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitions *T, ObservationTransitions *O);

	/**
	 * Expand the set of beliefs following Stochastic Simulation with Greedy Action. "Stochastic Simulation" means it
	 * generates belief points which are reachable given the initial set of belief points, i.e., it traverses the belief
	 * tree. In this case, for each belief point it randomly selects a state, proportional to the belief, then randomly
	 * rolls a die. If it is less than some epsilon, it randomly selects an action (uniformly); otherwise, it selects the
	 * optimal action. Next, it randomly selects a next state and next observation. The result is a new belief point.
	 * @param	S 		The finite states.
	 * @param	A 		The finite actions.
	 * @param	Z 		The finite observations.
	 * @param	T 		The finite state transition function.
	 * @param	O 		The finite observation transition function.
	 * @param	gamma	The current set of alpha vectors.
	 */
	void expand_stochastic_simulation_greedy_action(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitions *T, ObservationTransitions *O, std::vector<PolicyAlphaVector *> &gamma);

	/**
	 * Expand the set of beliefs following Stochastic Simulation with Exploratory Action. "Stochastic Simulation" means it
	 * generates belief points which are reachable given the initial set of belief points, i.e., it traverses the belief
	 * tree. In this case, for each belief point it adds a new belief point which maximizes over the actions, given a randomly
	 * selected next belief point following this action, selecting the point which is farthest away from the closest belief point.
	 * @param	S	The finite states.
	 * @param	A	The finite actions.
	 * @param	Z	The finite observations.
	 * @param	T	The finite state transition function.
	 * @param	O	The finite observation transition function.
	 */
	void expand_stochastic_simulation_exploratory_action(StatesMap *S, ActionsMap *A, ObservationsMap *Z,
			StateTransitions *T, ObservationTransitions *O);

	/**
	 * Expand the set of beliefs following Greedy Error Reduction. TODO: Implement.
	 */
	void expand_greedy_error_reduction();

	/**
	 * The expansion rule to use which adds belief points.
	 */
	POMDPPBVIExpansionRule rule;

	/**
	 * The number of update iterations until the solver stops for infinite horizon POMDPs.
	 */
	unsigned int updates;

	/**
	 * The number of expansion iterations until the solver stops for infinite horizon POMDPs.
	 */
	unsigned int expansions;

	/**
	 * The initial set of belief points.
	 */
	std::vector<BeliefState *> initialB;

	/**
	 * The final set of belief points after the solver's iterations.
	 */
	std::vector<BeliefState *> B;

};


#endif // POMDP_PBVI_H
