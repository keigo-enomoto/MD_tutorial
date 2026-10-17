CC=gcc
CFLAGS=-Wall
LDFLAGS=
LDLIBS=

CFLAGS += -O3
DEBUG_FLAG = -g3

OBJECTS = cal_force.o initial_condition.o md_simulation.o temp.o update.o utility.o
OBJECT_TEST_READ = 4_test_read.o
OBJECT_TEST_READ2 = 4_test_read2.o
TARGETS = md_simulation

all : $(TARGETS)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(DEBUG_FLAG) $<

md_simulation : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

test_read : $(OBJECT_TEST_READ)
	$(CC) -o $@ $(OBJECT_TEST_READ) $(LDFLAGS) $(LDLIBS)

test_read2 : $(OBJECT_TEST_READ2)
	$(CC) -o $@ $(OBJECT_TEST_READ2) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f core *~ *.o $(TARGETS)