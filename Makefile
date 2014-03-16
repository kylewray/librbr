CC = g++ 
CFLAGS = -std=c++11 -g
SDIR = src
TDIR = tests
ODIR  = obj

CORE_SRC_DIR = $(SDIR)/core
CORE_OBJ_DIR = $(ODIR)/core
UTIL_SRC_DIR = $(SDIR)/utilities
UTIL_OBJ_DIR = $(ODIR)/utilities
FLOAD_SRC_DIR = $(SDIR)/file_loaders
FLOAD_OBJ_DIR = $(ODIR)/file_loaders
MDP_SRC_DIR = $(SDIR)/mdp
MDP_OBJ_DIR = $(ODIR)/mdp
STATES_SRC_DIR = $(CORE_SRC_DIR)/states
STATES_OBJ_DIR = $(CORE_OBJ_DIR)/states
ACTIONS_SRC_DIR = $(CORE_SRC_DIR)/actions
ACTIONS_OBJ_DIR = $(CORE_OBJ_DIR)/actions
STATE_TRANSITIONS_SRC_DIR = $(CORE_SRC_DIR)/state_transitions
STATE_TRANSITIONS_OBJ_DIR = $(CORE_OBJ_DIR)/state_transitions
OBSERVATION_TRANSITIONS_SRC_DIR = $(CORE_SRC_DIR)/observation_transitions
OBSERVATION_TRANSITIONS_OBJ_DIR = $(CORE_OBJ_DIR)/observation_transitions
OBSERVATIONS_SRC_DIR = $(CORE_SRC_DIR)/observations
OBSERVATIONS_OBJ_DIR = $(CORE_OBJ_DIR)/observations
POLICY_SRC_DIR = $(CORE_SRC_DIR)/policy
POLICY_OBJ_DIR = $(CORE_OBJ_DIR)/policy
REWARDS_SRC_DIR = $(CORE_SRC_DIR)/rewards
REWARDS_OBJ_DIR = $(CORE_OBJ_DIR)/rewards
AGENTS_SRC_DIR = $(CORE_SRC_DIR)/agents
AGENTS_OBJ_DIR = $(CORE_OBJ_DIR)/agents

states.o: $(STATES_SRC_DIR)/*.cpp 
	mkdir -p $(STATES_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(STATES_SRC_DIR)/*.cpp 
	mv *.o $(STATES_OBJ_DIR)

actions.o: $(ACTIONS_SRC_DIR)/*.cpp 
	mkdir -p $(ACTIONS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(ACTIONS_SRC_DIR)/*.cpp 
	mv *.o $(ACTIONS_OBJ_DIR)

state_transitions.o: $(STATE_TRANSITIONS_SRC_DIR)/*.cpp 
	mkdir -p $(STATE_TRANSITIONS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(STATE_TRANSITIONS_SRC_DIR)/*.cpp 
	mv *.o $(STATE_TRANSITIONS_OBJ_DIR)

observation_transitions.o: $(OBSERVATION_TRANSITIONS_SRC_DIR)/*.cpp 
	mkdir -p $(OBSERVATION_TRANSITIONS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(OBSERVATION_TRANSITIONS_SRC_DIR)/*.cpp 
	mv *.o $(OBSERVATION_TRANSITIONS_OBJ_DIR)

observations.o: $(OBSERVATIONS_SRC_DIR)/*.cpp 
	mkdir -p $(OBSERVATIONS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(OBSERVATIONS_SRC_DIR)/*.cpp 
	mv *.o $(OBSERVATIONS_OBJ_DIR)

policy.o: $(POLICY_SRC_DIR)/*.cpp 
	mkdir -p $(POLICY_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(POLICY_SRC_DIR)/*.cpp 
	mv *.o $(POLICY_OBJ_DIR)

rewards.o: $(REWARDS_SRC_DIR)/*.cpp 
	mkdir -p $(REWARDS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(REWARDS_SRC_DIR)/*.cpp 
	mv *.o $(REWARDS_OBJ_DIR)

agents.o: $(AGENTS_SRC_DIR)/*.cpp 
	mkdir -p $(AGENTS_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(AGENTS_SRC_DIR)/*.cpp 
	mv *.o $(AGENTS_OBJ_DIR)

core.o: $(CORE_SRC_DIR)/*.cpp 
	mkdir -p $(CORE_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(CORE_SRC_DIR)/*.cpp 
	mv *.o $(CORE_OBJ_DIR)

util.o: $(UTIL_SRC_DIR)/*.cpp 
	mkdir -p $(UTIL_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(UTIL_SRC_DIR)/*.cpp 
	mv *.o $(UTIL_OBJ_DIR)

mdp.o: $(MDP_SRC_DIR)/*.cpp 
	mkdir -p $(MDP_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(MDP_SRC_DIR)/*.cpp 
	mv *.o $(MDP_OBJ_DIR)

fload.o: $(FLOAD_SRC_DIR)/*.cpp 
	mkdir -p $(FLOAD_OBJ_DIR) 
	$(CC) $(CFLAGS) -c $(FLOAD_SRC_DIR)/*.cpp 
	mv *.o $(FLOAD_OBJ_DIR)

make all.o: states.o actions.o state_transitions.o observation_transitions.o observations.o policy.o rewards.o agents.o core.o util.o fload.o mdp.o
