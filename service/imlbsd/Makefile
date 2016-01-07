GXX34 = 1
include ../Makefile.inc

SRC_COMM = main.cpp ImLbsRequest.cpp UdpPing.cpp LinkdManager.cpp P2PLbs.cpp

OBJ_COMM = $(SRC_COMM:.cpp=.o)
OBJ_COMM_R = $(SRC_COMM:.cpp=.ro)
 
LIB =  ${COMM_LIB}
LIB_R =  ${COMM_LIB_R}

#CXXFLAG1 = ${CXXFLAG} -DIMLBS_PING_DEBUG
CXXFLAG1 = ${CXXFLAG}

IMLIB = ../imlib/imlib.a

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG1) $(INCLUDE) -c -o $@ $<

.SUFFIXES: .ro .cpp
.cpp.ro:
	$(CXX) $(CXXFLAG_R) $(INCLUDE) -c -o $@ $<

PROGRAM = imlbs_d
all: imlbs_d

imlbs_d: .ver $(OBJ_COMM) $(LIB)
	$(CXX) -o $@ $(LINK_CXXFLAG) $(INCLUDE) $(OBJ_COMM:test.o=) $(IMLIB) $(LIB)


test: test.cpp $(LIB) $(OBJ_COMM) 
	$(CXX) -o test $(CXXFLAG) $(INCLUDE) $(OBJ_COMM:main.o=) $(LIB)
	./test


install:
	install $(PROGRAM) ../bin/
clean: 
	rm -f *.o
	rm -f *.ro
	rm -f imlbs_d

