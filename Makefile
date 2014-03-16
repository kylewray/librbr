CC = g++ 
CFLAGS = -std=c++11 -g
tests: all.o tests/core/*.cpp tests/mdp/*.cpp tests/file_loaders/*.cpp tests/utilities/*.cpp
	mkdir -p tests/obj
	$(CC) $(CFLAGS) -c -I.. tests/core/*.cpp tests/mdp/*.cpp tests/file_loaders/*.cpp tests/utilities/*.cpp tests/*.cpp
	$(CC) $(CFLAGS) -o perform_tests obj/*.o *.o
	rm *.o

states.o: src/core/states/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/states/*.cpp 
	mv *.o obj

actions.o: src/core/actions/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/actions/*.cpp 
	mv *.o obj

state_transitions.o: src/core/state_transitions/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/state_transitions/*.cpp 
	mv *.o obj

observation_transitions.o: src/core/observation_transitions/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/observation_transitions/*.cpp 
	mv *.o obj

observations.o: src/core/observations/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/observations/*.cpp 
	mv *.o obj

policy.o: src/core/policy/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/policy/*.cpp 
	mv *.o obj

rewards.o: src/core/rewards/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/rewards/*.cpp 
	mv *.o obj

agents.o: src/core/agents/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/agents/*.cpp 
	mv *.o obj

core.o: src/core/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/core/*.cpp 
	mv *.o obj

util.o: src/utilities/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/utilities/*.cpp 
	mv *.o obj

fload.o: src/file_loaders/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/file_loaders/*.cpp 
	mv *.o obj

mdp.o: src/mdp/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/mdp/*.cpp 
	mv *.o obj

pomdp.o: src/pomdp/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/pomdp/*.cpp 
	mv *.o obj

dec_mdp.o: src/dec_mdp/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/dec_mdp/*.cpp 
	mv *.o obj

dec_pomdp.o: src/dec_pomdp/*.cpp 
	mkdir -p obj 
	$(CC) $(CFLAGS) -c src/dec_pomdp/*.cpp 
	mv *.o obj

make all.o: states.o actions.o state_transitions.o observation_transitions.o observations.o policy.o rewards.o agents.o core.o util.o fload.o mdp.o pomdp.o dec_mdp.o dec_pomdp.o
