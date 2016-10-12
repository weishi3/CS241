# CS 241
# University of Illinois

CC = gcc
INC = -I. -Ilibs
FLAGS = -D_GNU_SOURCE -g
LIBS = -lpthread

all:
	make --quiet quietrun

quietrun: copying building testing

testing: building
	rm -rf data
	mkdir data
	./test-1
	rm -rf data
	mkdir data
	./test-2
	rm -rf data
	mkdir data
	./test-3
	rm -rf data
	mkdir data
	./test-4
	rm -rf data
	mkdir data
	./test-5
	rm -rf data

copying:
	cp ../datastore* .

building: copying test-1 test-2 test-3 test-4 test-5

test-1: test-1.o datastore_control.o datastore.o
	$(CC) $(FLAGS) $(INC) $^ -o $@ $(LIBS)

test-1.o: test-1.c test_utility.c datastore_control.o datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

test-2: test-2.o datastore_control.o datastore.o
	$(CC) $(FLAGS) $(INC) $^ -o $@ $(LIBS)

test-2.o: test-2.c test_utility.c datastore_control.o datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

test-3: test-3.o datastore_control.o datastore.o
	$(CC) $(FLAGS) $(INC) $^ -o $@ $(LIBS)

test-3.o: test-3.c test_utility.c datastore_control.o datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

test-4: test-4.o datastore_control.o datastore.o
	$(CC) $(FLAGS) $(INC) $^ -o $@ $(LIBS)

test-4.o: test-4.c test_utility.c datastore_control.o datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

test-5: test-5.o datastore_control.o datastore.o
	$(CC) $(FLAGS) $(INC) $^ -o $@ $(LIBS)

test-5.o: test-5.c test_utility.c datastore_control.o datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

datastore_control.o: datastore_control.c datastore_control.h datastore.o
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

datastore.o: datastore.c datastore.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@ $(LIBS)

clean:
	$(RM) -r *.o test-1 test-2 test-3 test-4 test-5 datastore*
