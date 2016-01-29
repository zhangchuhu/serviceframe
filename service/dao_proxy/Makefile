export SERVER_BACK_SOURCE_HOME=../..
include $(SERVER_BACK_SOURCE_HOME)/server_back/dao_proxy/makefile.inc

TARGETFILE = db_dao_proxyd_d

API_SRC:=$(wildcard ./*.cpp)
API_INCLUDES:=$(wildcard ./*.h ./*.hpp)
API_OBJS:=$(patsubst %.cpp,%.o, $(API_SRC))
API_LIB_SRC:=$(wildcard $(SERVER_BACK_SOURCE_HOME)/server_back/dao_proxy/gen-cpp/*.cpp)
API_LIB_OBJS:=$(patsubst %.cpp,%.o, $(API_LIB_SRC))
RECL_LIB_SRC:=$(wildcard $(SERVER_BACK_SOURCE_HOME)/server_common/seda/syncall/*.cpp)
RECL_LIB_OBJS:=$(patsubst %.cpp,%.o, $(RECL_LIB_SRC))
ENCRY_LIB_SRC:=$(wildcard $(SERVER_BACK_SOURCE_HOME)/server_common/seda/encryption/*.cpp)
ENCRY_LIB_OBJS:=$(patsubst %.cpp,%.o, $(ENCRY_LIB_SRC))

API_OBJS:=$(filter-out $(TARGETFILE_OBJS), $(API_OBJS))

TARGETFILE_OBJS=${API_OBJS} ${API_LIB_OBJS} ${RECL_LIB_OBJS} ${ENCRY_LIB_OBJS}

GLOBAL_LIBS=$(SERVER_BACK_SOURCE_HOME)/lib/slib3.a $(SERVER_BACK_SOURCE_HOME)/lib/libuuid.a $(SERVER_BACK_SOURCE_HOME)/lib/libmemcached.a $(SERVER_BACK_SOURCE_HOME)/lib/config.a ../bin/libboost_thread-gcc41-mt-d-1_34_1.so.1.34.1 ../bin/libcurl.so.3 $(SERVER_BACK_SOURCE_HOME)/lib/thrift-bridge.a ../bin/libthrift.so

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

tag:
	svn ci ../../server_common/helper/version.h -m "commit deploy version"
	@strDate="server_release_`date +%Y-%m-%d-%H:%M:%S`"; \
	svn cp https://svn.yy.duowan.com/repos/src/server_back/branches/activemq_hzj https://svn.yy.duowan.com/repos/src/server_back/tags/$$strDate -m "server release at $$strDate"; \
	echo "$$strDate released"


