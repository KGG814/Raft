// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <iostream>
#include <fstream>
#include <boost/thread/thread.hpp>

#include "Raft.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <thread>
#include <chrono>
#include <ctime>

#include <boost/thread.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;

#define TIMEOUT 1

#define INITIAL 0xDEADBEEF

#define NONE 0

#define MAJ 2
#define NUM_SERVERS 3

#define ID 1
#define SERVERS 2
#define MIN_ARGS 3

#define FOLLOWER 0
#define CANDIDATE 1
#define LEADER 2

int debug = false;
void requestVote (int voteID);
void sendHeartbeat (int voteID);
void timer (void);



std::chrono::system_clock::time_point msgTime; // TODO lock
boost::condition_variable cond;
boost::mutex voteLock;


std::list<boost::thread *> timeOuts;
int port = 0;
std::map<int, int> serverID;

// Raft server state

int id;                             // Unique ID for node
int state;                          // What state node is in TODO lock
int numVotes;                       // Votes node has in current term

// Persistent     
int currentTerm;                    // Term node is in TODO lock
int votedFor;                       // Who it voted for in current term TODO lock
std::vector<LogEntry> raftLog;

// Volatile
int commitIndex;
int lastApplied;

// Leader only
int nextIndex [5];
int matchIndex [5];


class RaftHandler : virtual public RaftIf {
 public:

  RaftHandler() {
    LogEntry initial;
    initial.term = 0;
    initial.number = INITIAL;
    // State initialization
    currentTerm = 0;
    state = FOLLOWER;
    votedFor = NONE; 
    commitIndex = 0;
    lastApplied = 0;
    numVotes = 0;
    msgTime = std::chrono::system_clock::now();
    raftLog.push_back(initial);
    std::cout << "server started" << std::endl;
  }

  void RequestVoteRPC(VoteResponse& _return, const RequestVote& vote) {
    if (debug) {std::cout << "RequestVoteRPC" << std::endl;}
    if (state == LEADER) {
      if (vote.term > currentTerm) {
        // TODO might be some problems here when log checking is implemented
        currentTerm = vote.term;
        state = FOLLOWER;
        votedFor = vote.candidateID;
        // Restart timeout timer
        msgTime = std::chrono::system_clock::now();
        timeOuts.push_back(new boost::thread(timer));
        // Return
        _return.voteGranted = true;
        _return.term = currentTerm;
      } else {
        // Return
        _return.voteGranted = false;
        _return.term = currentTerm;
      }
    } else if (state == FOLLOWER) {
      if (vote.term < currentTerm) {  
        // Return
        _return.voteGranted = false;
        _return.term = currentTerm;
      } else if (vote.term > currentTerm) {
        currentTerm = vote.term;
        if (vote.lastLogIndex >= raftLog.size()-1) {
          votedFor = vote.candidateID;      
          msgTime = std::chrono::system_clock::now();
          // Return
          _return.voteGranted = true;
          return.term = currentTerm;
        } else {
          votedFor = NONE;
          // Return
          _return.voteGranted = false;
        } 
      } else {
        if (votedFor == NONE || votedFor == vote.candidateID) {
          if (vote.lastLogIndex >= raftLog.size()-1) {
            votedFor = vote.candidateID;
            msgTime = std::chrono::system_clock::now();
            // Return
            _return.voteGranted = true;
            _return.term = currentTerm;
          } else {
            _return.voteGranted = false;
            return.term = currentTerm;
          }
        } else {
          _return.voteGranted = false;
          return.term = currentTerm;
        }
      }
    } else if (state == CANDIDATE) {
      // Do nothing
      
    }
  }

  void AppendEntriesRPC(AppendResponse& _return, const AppendEntries& append) {
    if (debug) {std::cout << "AppendEntriesRPC" << std::endl;}
    msgTime = std::chrono::system_clock::now();
    if (state == LEADER) {
      // Something weird happens
    } else if (state == FOLLOWER) {
      // Check terms and send vote
    } else if (state == CANDIDATE) {
      // If term received is >= currentTerm, return to follower state
      
      if (append.term >= currentTerm) {
        state = FOLLOWER;
        if (raftLog[append.prevLogIndex].term == append.prevLogTerm) {
          _return.success = true;
        }
        //TODO Delete conflicting entries of new entries
        //TODO Append entries
        //TODO If leaderCommit > commitIndex, 
        //commitIndex = min(leaderCommit, last new entry)
        //std::cout << "Candidate -> Follower: " << currentTerm << std::endl;
        currentTerm = append.term;
        // TODO interrupt here
        timeOuts.front()->interrupt();
      }
      // If not, continue in candidate
    }
  }

};

int main(int argc, char **argv) {
  

  if (argc >= MIN_ARGS) {
    // Server Information
    id = atoi(argv[ID]);
  } else {
    std::cout << "Usage: ./server [ID] [serverfile]" << std::endl;
    return 1;
  }
  
  std::string serverPort;
  std::ifstream servers (argv[SERVERS]);
  int i = 1;

  if (servers.is_open()) {
    while (std::getline(servers, serverPort)) {    
      serverID[i] = stoi(serverPort);
      if (id == i) {
         port = serverID[i];
      }
      i++;
    }
  }
  
  std::cout << port << std::endl;

  // Raft stuff
  shared_ptr<RaftHandler> handler(new RaftHandler());
  shared_ptr<TProcessor> processor(new RaftProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  // Initialise and start the timer and server
  TSimpleServer server(processor, serverTransport, transportFactory,
  protocolFactory);
  timeOuts.push_back(new boost::thread(timer));
  
  server.serve();
  
  return 0;
}

void timer (void) {
  std::string server; 
  
  while (1) {
    if (std::chrono::system_clock::now() >= msgTime + std::chrono::seconds(TIMEOUT)
        && state == FOLLOWER) {    
      // Candidate handling
      //std::cout << "Follower -> Candidate: " << currentTerm << std::endl;
      state = CANDIDATE;   
      currentTerm++;
      // Vote for self
      votedFor = id;
      voteLock.lock();
      numVotes = 1;
      voteLock.unlock();
      // Issue request vote RPCs (new threads)
      for (int i = 1; i <= NUM_SERVERS; i++) {
    	  if (id != i) {
          boost::thread voteThread (requestVote, i);
        }
      }
      // Wait for response
      boost::unique_lock<boost::mutex> lock(voteLock);
      try {
        while (numVotes < MAJ) {
      
          cond.wait(lock);
        }
      } catch (boost::thread_interrupted&) {
        // If valid AppendEntries received, thread will be reset
        if (debug) 
        {std::cout << "Candidate -> Follower:  " << currentTerm << std::endl;}
        msgTime = std::chrono::system_clock::now();
        state = FOLLOWER;
        goto end_loop; 
       
      } 

      // If successful, leader
      std::cout << "Candidate -> Leader:    " << currentTerm << std::endl;
      state = LEADER; 
 
      // Send blank AppendEntries to assert dominance
      for (int i = 1; i <= NUM_SERVERS; i++) {
    	  if (id != i) {
          boost::thread hearbeatThread (sendHeartbeat, i);
        }
      }
      timeOuts.pop_front();
      return;
    }
    end_loop:
    continue;
  }
}

void requestVote (int voteID) {
  //TODO Store response in array
  // Create the socket, transport and protocol structures for thrift.
  //TODO Add timeouts
  if (debug) {std::cout << "Requesting votes: " << voteID << std::endl;}
  boost::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", serverID[voteID]));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  VoteResponse response;
  RequestVote request;

  // The thrift client
  RaftClient client(protocol);

  try {
    
    transport->open();
    
    // Fill in request
    request.term = currentTerm;
    request.candidateID = id;
    request.lastLogIndex = raftLog.size()-1;
    request.lastLogTerm = raftLog[request.lastLogIndex].term;
    client.RequestVoteRPC(response, request);

    
    if (response.voteGranted == true) {
      if (debug) {std::cout << "Vote received: " << voteID << std::endl;}
      voteLock.lock();
      numVotes++;
      voteLock.unlock();
      cond.notify_one();
    } else {
      if (debug) {std::cout << "Vote rejected: " << voteID << std::endl;}
      if (response.term > currentTerm) {
        currentTerm = response.term;
        votedFor = NONE;
      }
    }
    transport->close();

  } catch (TException &tx) {
    //std::cout << "Connection Failed" << std::endl;
  } catch (boost::thread_interrupted&) {
  }
}

void sendHeartbeat (int voteID) {
     
  // Setup Raft connection
  //std::cout << "Sending heartbeat: " << voteID << std::endl;
  boost::shared_ptr<TTransport> socket(
  new TSocket("127.0.0.1", serverID[voteID]));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  RaftClient client(protocol);

  try {

    transport->open();    
    
    AppendResponse response;
    AppendEntries append;
    append.term = currentTerm;
    append.leaderID = id;
    append.prevLogIndex = raftLog.size()-1;
    append.prevLogTerm = raftLog[append.prevLogIndex].term;    
    append.leaderCommit = commitIndex;
    client.AppendEntriesRPC(response, append);
    transport->close();
  } catch (TException &tx) {
    //std::cout << "Connection Failed" << std::endl;
  }
}
