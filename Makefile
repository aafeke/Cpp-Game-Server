ifeq ($(OS),Windows_NT) 
isWin 	  	  := 1
else
isWin     	  := 0
endif

SRC   	      := .
INCLUDE   	  := .
TESTSRC		  := ./test
CXX       	  := g++
CXXFLAGS  	  := -g -c -pedantic -std=c++17 -Wall -Wextra -Wconversion

ifeq ($(isWin),1)
LDFLAGS   	  := -static-libgcc -static-libstdc++ -lWs2_32 -lMswsock -lAdvapi32 -static -lpthread
TARGETEXT	  := .exe
MOVE 		  := move /Y
else
LDFLAGS   	  := -Wl,-Bstatic,-lpthread -Wl,-Bdynamic
TARGETEXT	  := _nix
MOVE 		  := mv
endif

# Server
SERVERSRC 	  := ./server/src
SERVERINCLUDE := ./server/include
SERVEROBJ 	  := ./server/obj

#Client
CLIENTSRC 	  := ./client/src
CLIENTINCLUDE := ./client/include
CLIENTOBJ 	  := ./client/obj

# Targets
SERVERTARGET  := server$(TARGETEXT)
CLIENTTARGET  := client$(TARGETEXT)
TARGET 	  	  := main$(TARGETEXT)

test: $(TARGET)

ifeq ($(isWin),1)
clean:
	del $(TARGET)   & del $(SERVERTARGET) & del $(CLIENTTARGET)
	cd $(SERVEROBJ) & del *.o & cd ../.. 
	cd $(CLIENTOBJ) & del *.o & cd ../..
else
clean:
	rm $(TARGET) ; rm $(SERVERTARGET) ; rm $(CLIENTTARGET) ; rm $(SERVEROBJ)/*.o ; rm $(CLIENTOBJ)/*.o
endif

server: $(SERVERTARGET)

client: $(CLIENTTARGET)

.PHONY: test clean server client

# use '^' instead of '`' for the new line when running on cmd

$(SERVEROBJ)/*.o: $(SERVERSRC)/*.* $(SRC)/*.cpp $(SERVERINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(CXXFLAGS) $(SERVERSRC)/*.* $(SRC)/*.cpp -I$(SERVERINCLUDE) -I$(INCLUDE)
	$(MOVE) ./*.o $(SERVEROBJ)

$(SERVERTARGET): $(SERVEROBJ)/*.o
	$(CXX) $^ $(LDFLAGS) -o $@

$(CLIENTOBJ)/*.o: $(CLIENTSRC)/*.* $(SRC)/*.cpp $(CLIENTINCLUDE)/*.h $(INCLUDE)/*.h
	$(CXX) $(CXXFLAGS) $(CLIENTSRC)/*.* $(SRC)/*.cpp -I$(CLIENTINCLUDE) -I$(INCLUDE)
	$(MOVE) ./*.o $(CLIENTOBJ)

$(CLIENTTARGET): $(CLIENTOBJ)/*.o
	$(CXX) $^ $(LDFLAGS) -o $@

$(TARGET): $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp $(INCLUDE)/*.h $(SERVERINCLUDE)/*.h $(CLIENTINCLUDE)/*.h
	$(CXX) $(TESTSRC)/*.cpp $(SRC)/*.cpp $(CLIENTSRC)/*.cpp $(SERVERSRC)/*.cpp -I$(INCLUDE) -I$(SERVERINCLUDE) -I$(CLIENTINCLUDE) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET)

