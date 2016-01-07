GXX34 = 1
C = gcc
CXX32 = g++
CC32 = gcc

ARRU = ar -ru

DEFINE_SEL=-DHAVE_EPOLL

#DEF_STATUSPROTOCOL=-DSTATUS_OLDPROTOCOL
DEF_STATUSPROTOCOL=-DSTATUS_NEWPROTOCOL

DEF_DBTHREAD=-DDB_MULTITHREAD

COMM_LIB = ../../../lib/daemon.a ../../../lib/corelib.a ../../../lib/config.a ../../../lib/sox.a /usr/lib/libcrypto.a /usr/lib/libuuid.a
COMM_LIB_R = ../../../lib/corelib.ra ../../../lib/sox.ra /usr/lib/libcrypto.a 

VERSION=
CXXFLAG = -Wall -ggdb -D_REENTRANT $(DEFINE_SEL) -DXML_NULL  $(DEF_STATUSPROTOCOL)
CXXFLAG_R = -Wall -O2 -O3 -D_REENTRANT $(DEFINE_SEL) $(DEF_STATUSPROTOCOL)

LINK_CXXFLAG = $(CXXFLAG) -Wl,-rpath,../../bin
LINK_CXXFLAG_R = $(CXXFLAG_R) -Wl,-rpath,../../bin

INCLUDE = -I../../ -I../../../ -I../../../common/  -I../../../protocol/ -I../../../common/occi

.ver:
	export LC_ALL=C && sed -i 's/[0-9][0-9]*/'`svn info|grep Revision|awk '{print $$2}'`'/' ../../../server_common/helper/version.h



SRC_COMM = main.cpp ../UdpPing.cpp ../LinkdManager.cpp ../ImLbsRequest.cpp

OBJ_COMM = $(SRC_COMM:.cpp=.o)
OBJ_COMM_R = $(SRC_COMM:.cpp=.ro)
 
LIB =  ${COMM_LIB}
LIB_R =  ${COMM_LIB_R}

#CXXFLAG1 = ${CXXFLAG} -DIMLBS_PING_DEBUG
CXXFLAG1 = ${CXXFLAG}

IMLIB = ../../imlib/imlib.a

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAG1) $(INCLUDE) -c -o $@ $<

.SUFFIXES: .ro .cpp
.cpp.ro:
	$(CXX) $(CXXFLAG_R) $(INCLUDE) -c -o $@ $<

PROGRAM = lbstest_d
all: lbstest_d

lbstest_d: .ver $(OBJ_COMM) $(LIB)
	$(CXX) -o $@ $(LINK_CXXFLAG) $(INCLUDE) $(OBJ_COMM:test.o=) $(IMLIB) $(LIB)

clean: 
	rm -f *.o
	rm -f *.ro
	rm -f lbstest_d

