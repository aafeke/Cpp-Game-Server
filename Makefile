SRC   	      = .
INCLUDE   	  = .
TESTSRC		  = ./test
TARGET 	  	  = main.exe
CXX       	  = g++
CXXFLAGS  	  = -std=c++17 -Wall -Wextra -Wconversion
LDFLAGS   	  = -lWs2_32 -lMswsock -lAdvapi32
SERVERSRC 	  = ./server/src
SERVERINCLUDE = ./server/include
CLIENTSRC 	  = ./client/src
CLIENTINCLUDE = ./client/include
SERVERTARGET  = server.exe
CLIENTTARGET  = client.exe

test: $(TARGET)

server: $(SERVERTARGET)

client: $(CLIENTTARGET)

.PHONY: clean server client

$(TARGET): $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp $(INCLUDE)/*.h $(SERVERINCLUDE)/*.h $(CLIENTINCLUDE)/*.h
	$(CXX) $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp -I$(INCLUDE) -I$(SERVERINCLUDE) -I$(CLIENTINCLUDE) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET)

$(SERVERTARGET): $(SERVERSRC)/*.* $(SRC)/*.cpp $(SERVERINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(SERVERSRC)/*.* $(SRC)/*.cpp -I$(SERVERINCLUDE) -I$(INCLUDE) $(LDFLAGS) $(CXXFLAGS) -o $(SERVERTARGET)

$(CLIENTTARGET): $(CLIENTSRC)/*.* $(SRC)/*.cpp $(CLIENTINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(CLIENTSRC)/*.* $(SRC)/*.cpp -I$(CLIENTINCLUDE) -I$(INCLUDE) $(LDFLAGS) $(CXXFLAGS) -o $(CLIENTTARGET)

clean:
	del $(TARGET) & del $(SERVERTARGET) & del $(CLIENTTARGET)
