all: server

server: server.o Raft.o raft_constants.o raft_types.o
	g++ server.o Raft.o raft_constants.o raft_types.o -o server -l thrift -std=c++11 -lboost_thread -lboost_system

server.o: server.cpp
	g++ -c server.cpp -std=c++11

Raft.o: Raft.cpp
	g++ -c Raft.cpp -std=c++11

raft_constants.o: raft_constants.cpp
	g++ -c raft_constants.cpp -std=c++11

raft_types.o: raft_types.cpp
	g++ -c raft_types.cpp -std=c++11

clean:
	rm -rf *o server SpellService.log
