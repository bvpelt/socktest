# Macros for the compiler
CXX= g++

# Compile flags
CXXFLAGS= -O2 -g -march=x86-64

ALL: clean server

server: server.o bssocket.o
	$(CXX) -o server server.o bssocket.o    
    
bssocket.o: bssocket.cpp bssocket.h
	$(CXX) -c $(CXXFLAGS) bssocket.cpp
    
server.o: server.cpp bssocket.h
	$(CXX) -c $(CXXFLAGS) server.cpp
	
clean:
	rm -f *.o
	    
