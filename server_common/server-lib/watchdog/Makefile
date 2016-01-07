GXX34 = 1
include ../Makefile.inc
#include ../db.inc

SRC_COMM = main.cpp WatchDog.cpp Notifier.cpp ImTest.cpp

IMLIB = ../imlib/imlib.a

#-O2 

OBJ_COMM = $(SRC_COMM:.cpp=.o)
OBJ_COMM_R = $(SRC_COMM:.cpp=.ro)

LIB = $(COMM_LIB)
LIB_R = $(COMM_LIB_R)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(DBCXXFLAG) $(INCLUDE) -g -Wall -c -o $@ $<

.SUFFIXES: .ro .cpp
.cpp.ro:
	$(CXX) $(DBCXXFLAG_R) $(INCLUDE) -c -o $@ $<

PROGRAM = watchdog_d
all: watchdog_d

watchdog_d: .ver $(OBJ_COMM) $(IMLIB) $(LIB)
	$(CXX) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_COMM) $(IMLIB) $(LIB)

depend:
	mkdep $(INCLUDE) $(SRC_COMM) $(CXXFLAG)

install:
	install $(PROGRAM) ../bin/
clean: 
	rm -f *.o
	rm -f *.ro
	rm -f watchdog_d
