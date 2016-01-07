include ../Makefile.inc
CXXFLAGS1 = $(CXXFLAGS) -DXML_NULL
CXXFLAGS1_R = $(CXXFLAGS_R) -DXML_NULL 
SRCS =  tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp TinyXmlServerConfigImp.cpp LoaderContainer.cpp TinyXmlDbConfigImp.cpp test.cpp MemcachedConfigImp.cpp TinyXmlDbConfigImp2.cpp TinyXmlHADbConfigImp.cpp MemcachedConfigImp2.cpp XmlPath.cpp $(wildcard *_cipher.cpp)


OBJS = $(SRCS:.cpp=.o)
OBJC = $(SRCC:.c=.o)
OBJSS = $(OBJS) $(OBJC) #ntesclient.object

OBJS_R = $(SRCS:.cpp=.ro)
OBJC_R = $(SRCC:.c=.ro)
OBJSS_R = $(OBJS_R) $(OBJC_R)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) -g $(CXXFLAGS1) $(INCLUDE) -c -o $@ $<

.SUFFIXES: .ro .c
.c.ro:
	$(CXX32) $(CXXFLAGS1_R)  $(INCLUDE) -c -o $@ $<

.SUFFIXES: .o .cpp
.cpp.ro:
	$(CXX32) $(CXXFLAGS1_R) -O3 $(INCLUDE)  -c -o $@ $<

all: config.a 
release: config.ra

config.a: $(OBJS) $(OBJC) | ../../lib
	$(ARRU) ../../lib/config.a $(OBJSS:test.o=)
	ranlib ../../lib/config.a
	$(ARRU) ../../lib/libconfig.a $(OBJSS:test.o=)
	ranlib ../../lib/libconfig.a

config.ra: $(OBJS_R) $(OBJC_R) | ../../lib
	$(ARRU) ../../lib/config.ra $(OBJSS_R:test.ro=)
	ranlib ../../lib/config.ra

../../lib:
	mkdir ../../lib

test: $(OBJS) $(OBJC)
	$(CXX) -g -o $@ $(LINK_CXXFLAG) $(INCLUDE) $(OBJSS) $(COMM_LIB)
depend:
	mkdep $(CXXFLAGS_R) $(SRCS) $(SRCC)

clean:
	rm -f *.o
	rm -f *.ro

install:

distclean: clean
	rm -f .depend

#	vim: set ts=4 sts=4 syn=make :
#
