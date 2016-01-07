GXX34 = 1
include ../Makefile.inc



SRC_COMM := $(wildcard *.cpp) 

OBJ_COMM := $(SRC_COMM:.cpp=.o)


.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(VIP_FLAG) $(INCLUDE) -c -o $@ $<

PROGRAM = deamon_d


$(PROGRAM) : $(OBJ_COMM) 
	$(CXX) -o $@ $(MCXXFLAG) $(INCLUDE) $(OBJ_COMM) $(LIB) 


clean: 
	rm -rf $(OBJ_COMM) 
	rm -rf $(PROGRAM)
