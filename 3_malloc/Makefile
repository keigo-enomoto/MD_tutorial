CC=gcc
CFLAGS=-Wall
LDFLAGS=
LDLIBS=

CFLAGS += -O3
DEBUG_FLAG = -g3

OBJECTS = cal_force.o initial_condition.o md_simulation.o temp.o update.o utility.o
TARGETS = md_simulation

all : $(TARGETS)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(DEBUG_FLAG) $<

md_simulation : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f core *~ *.o $(TARGETS)