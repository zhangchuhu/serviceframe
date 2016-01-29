INCLUDE = -I. -I../zookeeper/ -I ../../ -I../../common/
LIBS = ../vip_lib/libzookeeper_mt.a ../../lib/libsox.a -lrt -lz
LIB_ZKCPP_NAME = libzkclient.a
PROGRAM = test_zkclient_d

SRC_COMM := $(wildcard *.cpp)
OBJ_COMM := $(SRC_COMM:.cpp=.o)

CXX = g++
RM = rm -f
CXXFLAGS += -Wall -g 
AR = ar


all: $(LIB_ZKCPP_NAME) $(PROGRAM)

$(LIB_ZKCPP_NAME): $(OBJ_COMM)
		$(AR) curs $@ $^ 
    
test_zkclient_d: example.o $(LIB_ZKCPP_NAME) $(LIBS)
		$(CXX) $^ $(LIB_ZKCPP_NAME) $(LIBS) -o $@

%.o: %.cpp
		$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

%.o: %.c
		$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

PROGRAM: $(OBJ_COMM) $(LIB_ZKCPP_NAME) $(LIBS)
		$(CXX) $^ $(INCLUDE) $(LIB_ZKCPP_NAME) $(LIBS) -o $@
	
	

all: $(LIB_ZKCPP_NAME) $(PROGRAM)

clean:
		$(RM) *.o $(LIB_ZKCPP_NAME) example

