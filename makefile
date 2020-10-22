# Macros for the compiler
CXX = g++

# Debug flag
DEBUGFLAG = -g

# Compile flags
CXXFLAGS = -O2 $(DEBUGFLAG) -march=x86-64

# Objects
objects = server.o bssocket.o

.PHONY: ALL
ALL: clean server

server: $(objects)
	$(CXX) $(DEBUGFLAG) -o server $(objects) 
    
bssocket.o: bssocket.cpp bssocket.h
	$(CXX) -c $(CXXFLAGS) $<
    
server.o: server.cpp bssocket.h
	$(CXX) -c $(CXXFLAGS) $<
	
#
# run clean without error checking
#	
.PHONY : clean
clean :
	-rm server *.o