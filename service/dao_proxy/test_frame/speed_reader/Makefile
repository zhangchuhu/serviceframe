export SERVER_BACK_SOURCE_HOME=../../../..
include $(SERVER_BACK_SOURCE_HOME)/server_back/dao_proxy/makefileo3.inc

TARGETFILE = daostat_readerd_d

API_SRC:=$(wildcard ./*.cpp)
API_INCLUDES:=$(wildcard ./*.h ./*.hpp)
API_OBJS:=$(patsubst %.cpp,%.o, $(API_SRC))

API_OBJS:=$(filter-out $(TARGETFILE_OBJS), $(API_OBJS))

TARGETFILE_OBJS=${API_OBJS}

GLOBAL_LIBS=$(SERVER_BACK_SOURCE_HOME)/lib/slib3.a $(SERVER_BACK_SOURCE_HOME)/lib/libuuid.a $(SERVER_BACK_SOURCE_HOME)/lib/libmemcached.a $(SERVER_BACK_SOURCE_HOME)/lib/config.a $(SERVER_BACK_SOURCE_HOME)/server_back/bin/libboost_thread-gcc41-mt-d-1_34_1.so.1.34.1 $(SERVER_BACK_SOURCE_HOME)/server_back/bin/libcurl.so.3 $(SERVER_BACK_SOURCE_HOME)/lib/thrift-bridge.a $(SERVER_BACK_SOURCE_HOME)/server_back/bin/libthrift.so

.PHONEY = all clean install unstall rebuild

all:  $(TARGETFILE) 
$(TARGETFILE):$(TARGETFILE_OBJS) $(GLOBAL_LIBS) $(BASE_LIBS) 
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(API_OBJS) $(TARGETFILE_OBJS)
install:
	

unstall:

rebuild:
	make clean;make -j;make install

