all: server

server: server.o Raft.o raft_constants.o raft_types.o gui.o
	g++ server.o Raft.o raft_constants.o raft_types.o gui.o -o server -l thrift -std=c++11 -lboost_thread -lboost_system `pkg-config gtkmm-3.0 --cflags --libs`

server.o: server.cpp
	g++ -c server.cpp -std=c++11 `pkg-config gtkmm-3.0 --cflags --libs`

gui.o: gui.cpp
	g++ -c gui.cpp -std=c++11 `pkg-config gtkmm-3.0 --cflags --libs`

Raft.o: Raft.cpp
	g++ -c Raft.cpp -std=c++11

raft_constants.o: raft_constants.cpp
	g++ -c raft_constants.cpp -std=c++11

raft_types.o: raft_types.cpp
	g++ -c raft_types.cpp -std=c++11



clean:
	rm -rf *o server SpellService.log
