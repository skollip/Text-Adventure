CC=gcc
CFLAGS=-Wall -std=c99 -g
LDFLAGS=-g
OBJS=world.o player.o quester.o make_test_world.o quester_test.o
EXES=make_test_world quester quester_test

all: $(EXES)

clean: 
	rm -f $(OBJS) $(EXES)
    
quester: world.o player.o quester.o

quester_test: world.o player.o quester_test.o

make_test_world: make_test_world.o
    
world.o: world.h
player.o: player.h world.h
quester.o: player.h world.h
quester_test.o: player.h world.h
make_test_world.o: world.h

