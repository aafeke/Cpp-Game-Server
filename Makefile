SRC   	      = .
INCLUDE   	  = .
TESTSRC		  = ./test
TARGET 	  	  = main.exe
CXX       	  = g++
CXXFLAGS  	  = -g -c -pedantic -std=c++17 -Wall -Wextra -Wconversion
LDFLAGS   	  = -lWs2_32 -lMswsock -lAdvapi32

# Server
SERVERSRC 	  = ./server/src
SERVERINCLUDE = ./server/include
SERVEROBJ 	  = ./server/obj

#Client
CLIENTSRC 	  = ./client/src
CLIENTINCLUDE = ./client/include
CLIENTOBJ 	  = ./client/obj

# Targets
SERVERTARGET  = server.exe
CLIENTTARGET  = client.exe

test: $(TARGET)

clean:
	del $(TARGET) & del $(SERVERTARGET) & del $(CLIENTTARGET)

server: $(SERVERTARGET)

client: $(CLIENTTARGET)

.PHONY: test clean server client

# use '^' instead of '`' for the new line when running on cmd

$(SERVEROBJ)/*.o: $(SERVERSRC)/*.* $(SRC)/*.cpp $(SERVERINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(CXXFLAGS) $(SERVERSRC)/*.* $(SRC)/*.cpp -I$(SERVERINCLUDE) -I$(INCLUDE)
	move /Y ./*.o $(SERVEROBJ)

$(SERVERTARGET): $(SERVEROBJ)/*.o
	$(CXX) $^ $(LDFLAGS) -o $@

$(CLIENTOBJ)/*.o: $(CLIENTSRC)/*.* $(SRC)/*.cpp $(CLIENTINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(CXXFLAGS) $(CLIENTSRC)/*.* $(SRC)/*.cpp -I$(CLIENTINCLUDE) -I$(INCLUDE)
	move /Y ./*.o $(CLIENTOBJ)

$(CLIENTTARGET): $(CLIENTOBJ)/*.o
	$(CXX) $^ $(LDFLAGS) -o $@

$(TARGET): $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp $(INCLUDE)/*.h $(SERVERINCLUDE)/*.h $(CLIENTINCLUDE)/*.h
	$(CXX) $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp -I$(INCLUDE) -I$(SERVERINCLUDE) -I$(CLIENTINCLUDE) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET)

