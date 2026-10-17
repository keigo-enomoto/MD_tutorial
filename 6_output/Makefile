CC=gcc
CFLAGS=-Wall
LDFLAGS=
LDLIBS=

CFLAGS += -O3
DEBUG_FLAG = -g3

OBJECTS = cal_force.o initial_condition.o md_simulation.o output.o temp.o update.o utility.o
OBJECTS_MK_INIT = initial_condition.o mk_init.o temp.o utility.o
TARGETS = md_simulation mk_init

all : $(TARGETS)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(CC) -c -o $@ $(CFLAGS) $(DEBUG_FLAG) $<

md_simulation : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

mk_init : $(OBJECTS_MK_INIT)
	$(CC) -o $@ $(OBJECTS_MK_INIT) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f core *~ *.o $(TARGETS)