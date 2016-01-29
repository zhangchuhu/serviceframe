#directory definations
DIR_SERVER_HOME= $(SERVER_BACK_SOURCE_HOME)
DIR_COMMON     = $(SERVER_BACK_SOURCE_HOME)/common
DIR_INCLUDE    = /usr/local/include
DIR_GLO_LIBRARY= /usr/lib
DIR_LIBRARY    = $(SERVER_BACK_SOURCE_HOME)/lib
DIR_PROTOCOL   = $(SERVER_BACK_SOURCE_HOME)/protocol/
DIR_OCCI       = $(SERVER_BACK_SOURCE_HOME)/common/occi

#base library definations
LIB_COMMON_ALL = ${SERVER_BACK_SOURCE_HOME}/lib/daemon.a ${SERVER_BACK_SOURCE_HOME}/lib/corelib.a ${SERVER_BACK_SOURCE_HOME}/lib/config.a ${SERVER_BACK_SOURCE_HOME}/lib/sox.a
LIB_COMMON_RA  = ${SERVER_BACK_SOURCE_HOME}/lib/corelib.ra ${SERVER_BACK_SOURCE_HOME}/lib/sox.ra /usr/lib/libcrypto.a 

#LIB_THREAD     = -lpthread

INC_SERVER_HOME= -I$(DIR_SERVER_HOME)
INC_COMMON     = -I$(DIR_COMMON)
INC_PROTOCOL   = -I$(DIR_PROTOCOL)
INC_OCCI       = -I$(DIR_OCCI)
INC_THRIFTPROTOCOL = -I../gen-cpp -I/usr/local/include/thrift 
INC_THRIFTBRIDGE = -I$(SERVER_BACK_SOURCE_HOME)/server_back/thrift-bridge

#command definations
EXTRAFLAGS     =  -Wall -ggdb -DXML_NULL -pthread -m32
INCLUDES       = -I../ $(INC_SERVER_HOME) $(INC_COMMON) $(INC_PROTOCOL) $(INC_OCCI) $(INC_THRIFTPROTOCOL) $(INC_THRIFTBRIDGE)
CXXFLAGS       = $(INCLUDES) $(EXTRAFLAGS) -Wl,-rpath,../bin
BASE_LIBS      = $(LIB_COMMON_ALL) $(LIB_THREAD)

MAKE           = make
CXX            = g++
LEX            = flex
YACC           = bison
MV             = mv -f
RM             = rm -f
CP             = cp -f
AR             = ar rc

#rule definations
.SUFFIXES:  .cxx
.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)
.cxx.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)
.cc.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)



