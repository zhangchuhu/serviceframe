include ../Makefile.inc
include ../db.inc

#depends: sox."int_types.h"

CXXFLAGS =  -pthread -Wall -ggdb $(INCLUDE) -I/usr/local/include  -DMYSQLPP_MYSQL_HEADERS_BURIED  $(DEFINE_SEL) 
CXXFLAGS_R = -pthread -Wall -O2 -O3 $(INCLUDE) -I/usr/local/include  -DMYSQLPP_MYSQL_HEADERS_BURIED  $(DEFINE_SEL)
#-fomit-frame-pointer

##
# Attention:Fnv Hash compile switch
# (!!!) Please open it when compile imlinkd, imapp, imon2.
##
#CXXFLAGS += -DFNV_HASH

SRCS = imDataOpHelper.cpp  imDataOpHelper_wuji.cpp  \
	SearchBuddyRecordOpHelper.cpp  AddBuddyRecordOpHelper.cpp \
	ImLinkIDHelper.cpp VerifyCodeHelper.cpp \
	ImTransferIDHelper.cpp ImTransHelper.cpp ImMemDBHelper.cpp \
	ImOnlineIDHelper.cpp ImOnlineCacheHelper.cpp ImServersInfo.cpp \
   	MultiImServersInfo.cpp MultiImTransHelper.cpp \
	ImHashServer.cpp MultiImHashServer.cpp MetaSqlFactory.cpp \
	ImDoubleRoute.cpp MultiImDoubleRoute.cpp ImMLRouter.cpp WatchDog.cpp\
	BuddyListDaoDdbImp.cpp BuddyListMetaDao.cpp \
	GrpHashServer.cpp ImOnlineHashHelper.cpp ../imdao/ImOnlineStatusMemDao.cpp\
	ImStatistics.cpp
		
# UinfoIdx.cpp Birthday.cpp po_signal.cpp

SRCC = #rc4.c

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

all: imlib.a #imlib.ra

imlib.a: $(OBJS) $(OBJC) ../../lib/sox.a
	$(ARRU) imlib.a $(OBJSS:test.o=)
	ranlib imlib.a
	
test:  $(OBJS) $(OBJC) ../../lib/sox.a
	$(CXX) $(MCXXFLAG) -o $@ $(OBJS) $(LIB)
	./test

imlib.ra: $(OBJS_R) $(OBJC_R) ../../lib/sox.ra
	$(ARRU) imlib.ra $(OBJSS_R)
	ranlib imlib.ra
depend:
	mkdep $(CXXFLAGS_R) $(SRCS) $(SRCC)

clean:
	rm -f *.o
	rm -f *.ro
	rm -f ../imdao/*.o
	rm -f imlib.a

install:

distclean: clean
	rm -f .depend

#	vim: set ts=4 sts=4 syn=make :
