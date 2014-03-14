CC = g++ 
CFLAGS = -std=c++11 -g
SDIR = src
ODIR  = obj
SCDIR = $(SDIR)/core
OCDIR = $(ODIR)/core

STATES_SRC_DIR = $(SCDIR)/states
STATES_OBJ_DIR = $(OCDIR)/states
ACTIONS_SRC_DIR = $(SCDIR)/actions
ACTIONS_OBJ_DIR = $(OCDIR)/actions
STATE_TRANSITIONS_SRC_DIR = $(SCDIR)/state_transitions
STATE_TRANSITIONS_OBJ_DIR = $(OCDIR)/state_transitions

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

make all: states.o actions.o state_transitions.o 
