GXX34 = 1
include ../../vip_score_v2/include/Makefile64.inc
include ../../vip_score_v2/include/db64.inc

SRC_COMM = $(wildcard *.cpp) \
		  ../../ConnTest/BlockingConn.cpp 
OBJ_COMM = $(SRC_COMM:.cpp=.o)
OBJ_COMM_M = $(SRC_COMM:.cpp=.mo)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(DBCXXFLAG) $(INCLUDE) -c -o $@ $<

PROGRAM=conntest_d 
INCLUDE += -I../../ConnTest

all: $(PROGRAM) 

$(PROGRAM): $(OBJ_COMM) $(LIB)
	$(CXX) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_COMM) $(LIB) 

depend:
	mkdep $(INCLUDE) $(SRC_COMM) $(CXXFLAG)

install:
	install $(PROGRAM) ../bin/
clean: 
	rm -f *.o
	rm -f *.ro
	rm -rf $(PROGRAM)

