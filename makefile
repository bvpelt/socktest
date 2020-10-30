# Macros for the compiler
CXX = g++

# Debug flag
DEBUGFLAG = -g

# Optimeze flag
OPTIMIZE =

# Compile flags
CXXFLAGS = $(OPTIMIZE) $(DEBUGFLAG) -march=x86-64

# Objects
objects = bssocket.o bsexception.o work.o worklist.o
serverobjects = server.o tcpserver.o 
clientobjects = client.o tcpclient.o
testworklistobjects = testworklist.o

.PHONY: ALL
ALL: server client testworklist

server: $(objects) $(serverobjects)
	$(CXX) $(DEBUGFLAG) -o server $(objects) $(serverobjects)

client: $(objects) $(clientobjects)
	$(CXX) $(DEBUGFLAG) -o client $(objects) $(clientobjects)	
    
bssocket.o: bssocket.cpp bssocket.h
	$(CXX) -c $(CXXFLAGS) $<
    
server.o: server.cpp tcpserver.h
	$(CXX) -c $(CXXFLAGS) $<

client.o: client.cpp tcpclient.h
	$(CXX) -c $(CXXFLAGS) $<

bsexception.o: bsexception.cpp bsexception.h 
	$(CXX) -c $(CXXFLAGS) $<

tcpserver.o: tcpserver.cpp tcpserver.h bssocket.h
	$(CXX) -c $(CXXFLAGS) $<

tcpclient.o: tcpclient.cpp tcpclient.h bssocket.h
	$(CXX) -c $(CXXFLAGS) $<



work.o: work.cpp work.h 
	$(CXX) -c $(CXXFLAGS) $<

worklist.o: worklist.cpp worklist.h 
	$(CXX) -c $(CXXFLAGS) $<

testworklist.o: testworklist.cpp worklist.h
	$(CXX) -c $(CXXFLAGS) $<

testworklist: $(objects) $(testworklistobjects)
	$(CXX) $(DEBUGFLAG) -o testworklist $(objects) $(testworklistobjects)
#
# run clean without error checking
#	
.PHONY : clean
clean :
	-rm server client *.o