
include ../Makefile.inc

CXXFLAGS = $(CXXFLAG) $(INCLUDE)#-pthread -Wall -ggdb -I../../ -I../../common/  $(DEFINE_SEL)  $(DEF_STATUSPROTOCOL)
CXXFLAGS_R = -pthread $(INCLUDE) -Wall -O2 -O3 -I../../ -I../../common/  $(DEFINE_SEL) $(DEF_STATUSPROTOCOL)
#-fomit-frame-pointer

##
# Attention:Fnv Hash compile switch
# (!!!) Please open it when compile imlinkd, imapp, imon2.
##
#CXXFLAGS += -DFNV_HASH

SRCS = DaemonServer.cpp DaemonClient.cpp  BRouteAppContext.cpp BRouteWriter.cpp UrlHelper.cpp RouteTarget.cpp \
		ServerWriter.cpp RouteIgnoreAppContext.cpp CookieHelper.cpp RC4_Helper.cpp BucketDist_Alg.cpp MSSelector.cpp \
		PacketImp.cpp PRouterImp.cpp ProtoWriter.cpp ConfigDaemonServer.cpp BranchesFilter.cpp \
		ReliableSendQueue.cpp ReliableWriter.cpp ReliableAppContext.cpp ImDaemonClient.cpp \
		Cli_ImGrpServerHelper.cpp ServerSelectorImp.cpp MulServerSelectorImp.cpp GrpHashServer.cpp  \
		ConsoleServer.cpp

OBJS = $(SRCS:.cpp=.o)
OBJC = $(SRCC:.c=.o)
OBJSS = $(OBJS) $(OBJC) #ntesclient.object

OBJS_R = $(SRCS:.cpp=.ro)
OBJC_R = $(SRCC:.c=.ro)
OBJSS_R = $(OBJS_R) $(OBJC_R)

.SUFFIXES: .o .c
.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.SUFFIXES: .ro .c
.c.ro:
	$(CXX32) $(CXXFLAGS_R) -c -o $@ $<

.SUFFIXES: .o .cpp
.cpp.ro:
	$(CXX32) $(CXXFLAGS_R) -c -o $@ $<

all: daemon.a 
release: daemon.ra
daemon.a: $(OBJS) $(OBJC) ../../lib/sox.a
	$(ARRU) ../../lib/daemon.a $(OBJSS)
	ranlib ../../lib/daemon.a
	$(ARRU) ../../lib/libdaemon.a $(OBJSS)
	ranlib ../../lib/libdaemon.a

daemon.ra: $(OBJS_R) $(OBJC_R) ../../lib/sox.ra
	$(ARRU) ../../lib/daemon.ra $(OBJSS_R)
	ranlib ../../lib/daemon.ra



depend:
	mkdep $(CXXFLAGS_R) $(SRCS) $(SRCC)

clean:
	rm -f *.o
	rm -f *.ro
	rm -f ../../lib/daemon.a
	rm -rf ../server-lib2/*.o

install:

distclean: clean
	rm -f .depend

#	vim: set ts=4 sts=4 syn=make :
#
