// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.


#include <fstream>
#include <boost/thread/thread.hpp>
#include <algorithm>

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
#include "gui.hpp"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;

//////////////////// Server Constants ////////////////////
#define TIMEOUT 1000
#define RESEND 800
// Timeout for individual vote connection
#define VOTE_TIMEOUT 1000
// The minimum candidate timeout
#define CANDIDATE_TIMEOUT 800
// The maximum added random timeout for candidate timeouts
#define CANDIDATE_TIMEOUT_RND 200
// Initial value in the log
#define INITIAL 0xDEADBEEF
// No one voted for
#define NONE 0
// Number of servers and majority
// Number of servers should be odd
// Majority should be ceil(NUM_SERVERS/2)
#define MAJ 2
#define NUM_SERVERS 3

//////////////////// Arguments ///////////////////////////
#define ID 1
#define SERVERS 2
#define MIN_ARGS 3

//////////////////// States //////////////////////////////
#define FOLLOWER 0
#define CANDIDATE 1
#define LEADER 2

//////////////////// Debug ///////////////////////////////
// Set to true for debug output
int debug = false;

//////////////////// Functions ///////////////////////////
// Wrapper functions for RPCs
void requestVote (int voteID);
void sendHeartbeat (int voteID);
// Lock wrapper functions
bool logUpToDate (RequestVote vote);
void incrementResponses (void);
void resetResponses (void);
void incrementVotes (void);
void resetVotes (void);
void resetTime(void);
void incrementCurrentTerm (void);
void resetCurrentTerm (void);
void setState (int newState);
void setVotedFor (int newVote);
void resetVotedFor (void);
void resolveConflicts (const AppendEntries append);
void initialiseLogTest(void);
void printLog(void);
// Handles time based Raft events (timeouts etc.)
void heartbeatTimer (void);
void handlerServer (int argc, char* argv[]);



//////////////////// Misc Variables //////////////////////
RaftWindow* window;
std::stringstream buffer;
// Used for timeouts
std::chrono::system_clock::time_point msgTime;
// Condition variable for vote responses
boost::condition_variable cond;
// Locks and their functions
boost::mutex voteLock, responseLock, msgTimeLock, currentTermLock, stateLock,
             votedForLock, logLock;
// Boost thread object for the timer thread
boost::thread * timerThread; 
// Port number for this server
int port = 0;
// Maps IDs to port numbers
std::map<int, int> serverID;
// Number of responses received (including a dropped connection). 
// Required for candidate timeouts.
int numResponse; 
//////////////////// Raft State //////////////////////////

int id;                             // Unique ID for node
int state;                          // What state node is in
int numVotes;                       // Votes node has in current term
         
// Persistent     
int currentTerm;                    // Term server is in
int votedFor;                       // Who it voted for in current term
std::vector<LogEntry> raftLog;
// Volatile
int lastApplied;
int commitIndex;
// Leader only
int nextIndex [NUM_SERVERS];
int matchIndex [NUM_SERVERS];


class RaftHandler : virtual public RaftIf {
 public:

  RaftHandler() {
    // Create an initial entry to allow syncing on an empty log
    LogEntry initial;
    resetCurrentTerm();
    initial.term = currentTerm;
    initial.number = INITIAL;
    raftLog.push_back(initial);
    // State initialization
    setState(FOLLOWER); 
    commitIndex = 0;
    lastApplied = 0;
    resetVotedFor();
    resetVotes ();
    resetResponses();
    resetTime();    
    std::cout << "server started" << std::endl;
  }

  void RequestVoteRPC(VoteResponse& _return, const RequestVote& vote) {
    if (debug) {std::cout << "RequestVoteRPC" << std::endl;}
    stateLock.lock();
    // Handles the case where a Leader receives a vote request from a candidate
    if (state == LEADER) {
      currentTermLock.lock();
      // Term must be greater to override a Leader
      if (vote.term > currentTerm) {
        // If the log is up to date from the leader's perspective, and the term
        // of the request is higher, then the leader must reliquish leadership
        if (logUpToDate(vote)) {
          currentTerm = vote.term;
          state = FOLLOWER;
          if (debug)
          {std::cout << "Leader -> Follower:  " << currentTerm << std::endl;}
          setVotedFor(vote.candidateID);
          resetTime();
          _return.voteGranted = true;
          _return.term = currentTerm;
        // If log is not up to date, deny vote and send back term so the
        // candidate can update accordingly
        } else {
          _return.voteGranted = false;
          _return.term = currentTerm;
        }
      // If term is not greater, then reply false and send term for candidate
      // to update
      } else {
        _return.voteGranted = false;
        _return.term = currentTerm;
      }
      currentTermLock.unlock();
    // Standard case for a follower receiving a vote from a new candidate
    } else if (state == FOLLOWER) {
      currentTermLock.lock();
      // If the request has a lower term, that means the current leader
      // has priority
      if (vote.term < currentTerm) {  
        _return.voteGranted = false;
        _return.term = currentTerm;
      // If the request has higher term, then we should check the logs
      } else if (vote.term > currentTerm) {
        // Regardless of whether the log is up to date or not, we should update
        // the term to ensure the next vote cycle is valid in case this one is
        // invalid, since we got a higher term
        currentTerm = vote.term;
        // If the logs are valid, then we can safely vote
        if (logUpToDate(vote)) {
          setVotedFor(vote.candidateID);      
          resetTime();
          _return.voteGranted = true;
          _return.term = currentTerm;
        // Log isn't valid, reset voted for since we updated term
        } else {
          resetVotedFor();
          _return.voteGranted = false;
        }
      // If term is the same, must check if the server already voted.
      } else {
        votedForLock.lock();
        // If we have not voted, or we have already voted for this candidate
        // this round, then we can check logs
        if (votedFor == NONE || votedFor == vote.candidateID) {
          // If logs are valid, safe to vote
          if (logUpToDate(vote)) {
            votedFor = vote.candidateID;
            resetTime();
            _return.voteGranted = true;
            _return.term = currentTerm;
          // If logs are not valid, reject vote
          } else {
            _return.voteGranted = false;
            _return.term = currentTerm;
          }
        // Already voted for someone else, reject
        } else {
          _return.voteGranted = false;
          _return.term = currentTerm;
        }
        votedForLock.unlock();
      }
      currentTermLock.unlock();
    } else if (state == CANDIDATE) {
      currentTermLock.lock();
      // While candidate, we can only vote for a greater term.
      if (vote.term > currentTerm && logUpToDate(vote)) {
        timerThread->interrupt(); // Interrupt candidate vote collection
        state = FOLLOWER; 
        setVotedFor(vote.candidateID);
        currentTerm = vote.term;     
        _return.voteGranted = true;
        _return.term = currentTerm;
        if (debug)
        {std::cout << "Candidate -> Follower:  " << currentTerm << std::endl;}
      } 
      currentTermLock.unlock(); 
    }
    stateLock.unlock();
  }

  // Handles receiving an AppendEntriesRPC
  void AppendEntriesRPC(AppendResponse& _return, const AppendEntries& append) {
    if (debug) {std::cout << "AppendEntriesRPC" << std::endl;}
    stateLock.lock();
    if (state == LEADER) {
      // TODO Ignore for now, presumably should defer to new leader if valid
    } else if (state == CANDIDATE || state == FOLLOWER) {
      currentTermLock.lock();
      // Term of sender must be greater than or equal to ensure a valid leader
      if (append.term >= currentTerm) {       
        currentTerm = append.term;
        _return.term = currentTerm;
        // Check for valid log up to this point
        if (raftLog[append.prevLogIndex].term == append.prevLogTerm) {
          resolveConflicts(append);
          _return.success = true;
          resetTime();
          // Update commitIndex to what the local server knows is committed
          if (append.leaderCommit > commitIndex) {
            commitIndex = std::min(append.leaderCommit, lastApplied);
          }
        } else {
          
          _return.success = false;      
        }
        if (state == CANDIDATE) {
          // Interrupt candidate elections
          timerThread->interrupt();
        }   
        if (debug) {
          if (state == FOLLOWER) {
            std::cout << "Heartbeat received " << append.leaderID << std::endl;
          } else if (state == CANDIDATE) {
            std::cout << "Candidate -> Follower:  " << currentTerm << std::endl;  
          }    
        }
        state = FOLLOWER;
      } else {
        // Return term so leader can calculate current term of system and update
        _return.term = currentTerm;
        _return.success = false;
      }
      currentTermLock.unlock();
    }
    stateLock.unlock();
  }

};

int main(int argc, char *argv[]) {
  int arg = 1;
  Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(arg, argv,"org.gtkmm.examples.base");
  RaftWindow *raftwindow = new RaftWindow();
  boost::thread serverThread (handlerServer, argc, argv);
  window = raftwindow;
  return app->run(*raftwindow);
}

void handlerServer (int argc, char* argv[]) {
  if (argc >= MIN_ARGS) {
    // Server Information
    id = atoi(argv[ID]);
  } else {
    std::cout << "Usage: ./server [ID] [serverfile]" << std::endl;
    return;
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
  if (port == 0) {
    std::cout << "ID not found" <<std::endl;
    return;
  }
  std::cout << port << std::endl;

  // Raft stuff
  shared_ptr<RaftHandler> handler(new RaftHandler());
  shared_ptr<TProcessor> processor(new RaftProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory
                                (new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  // Initialise and start the timer and server
  TSimpleServer server(processor, serverTransport, transportFactory,
  protocolFactory);
  timerThread = new boost::thread(heartbeatTimer);
  
  server.serve();
}

void heartbeatTimer (void) {
  bool voteSuccess;
  std::string server; 
  srand (time(NULL));
  int rnd = rand() % CANDIDATE_TIMEOUT_RND;
  while (1) {
    try {
      // Maintain Leader status
      if (std::chrono::system_clock::now() >= msgTime + std::chrono::milliseconds
      (RESEND) && state == LEADER) {
        // Send blank AppendEntries to assert dominance
        // TODO Probably need to do some log checking here
        if (debug) {std::cout << "Maintaining leader status" << std::endl;}
        for (int i = 1; i <= NUM_SERVERS; i++) {
    	    if (id != i) {
            boost::thread heartbeatThread (sendHeartbeat, i);
          }
        }
        resetTime();
      }
    
      // Election timeout
      if (std::chrono::system_clock::now() >= msgTime + 
          std::chrono::milliseconds(TIMEOUT) +
          std::chrono::milliseconds(rnd)
          && state == FOLLOWER) {    
        
        voteSuccess = false;
        // If candidacy is reset, start from here
        std::cout << "Follower -> Candidate: " << currentTerm + 1 << std::endl;
        buffer << "Follower -> Candidate: " << currentTerm + 1 << std::endl;
        window->setText(buffer.str());
        while (!voteSuccess) {
         // Check to see if a leader has already been selected before we 
          // continue
          
          boost::this_thread::interruption_point();
          setState(CANDIDATE);      
          srand (time(NULL)*(id+1));
          rnd = rand() % CANDIDATE_TIMEOUT_RND;
          incrementCurrentTerm();
          if (debug) {std::cout << "Candidate: " << currentTerm
          << std::endl;}
          // Vote for self
          setVotedFor(id);
          resetVotes();
          
          incrementVotes();
          // Reset response counter
          resetResponses();
          
          // Issue request vote RPCs (new threads)
          resetTime();
          for (int i = 1; i <= NUM_SERVERS; i++) {
        	  if (id != i) {
              boost::thread voteThread (requestVote, i);
            }
          }
          // Wait for response
          boost::unique_lock<boost::mutex> lock(responseLock);
          while (numVotes < MAJ) {
            if (numResponse < NUM_SERVERS-1) {
              cond.wait(lock);
            } else {
              // Wait a random amount of time to prevent multiple candidates
              while (std::chrono::system_clock::now() < msgTime +
                     std::chrono::milliseconds(CANDIDATE_TIMEOUT) +
                     std::chrono::milliseconds(rnd)) {};
              resetTime();
              // Reset candidacy
              if (debug) {std::cout << "Resetting election" << std::endl;}
              break;
            }
          }
          // Release the lock that was locked by the unique_lock
          responseLock.unlock();
          if (numVotes >= MAJ) {
            voteSuccess = true;
          }
        }   
        // If successful, leader
        std::cout << "Candidate -> Leader:    " << currentTerm << std::endl;
        setState(LEADER);
	      if (raftLog.size() == 1) {
	        initialiseLogTest();
	      }
        resetTime();
        // Send blank AppendEntries to assert dominance
        // TODO Probably need to do some log checking here
        for (int i = 1; i <= NUM_SERVERS; i++) {
      	  if (id != i) {
            boost::thread heartbeatThread (sendHeartbeat, i);
          }
        }
      }
    } catch (boost::thread_interrupted&) {
      // If valid AppendEntries received, thread will be reset
      resetTime();
      continue;
    } catch (TException &tx) {
    }
  }
}

void requestVote (int voteID) {
  // Create the socket, transport and protocol structures for thrift.
  if (debug) {std::cout << "Requesting votes: " << voteID << std::endl;}
  TSocket* _socket = new TSocket("127.0.0.1", serverID[voteID]);
  // Set timeouts so that candidate elections
	_socket->setRecvTimeout(VOTE_TIMEOUT);
	_socket->setConnTimeout(VOTE_TIMEOUT);
  boost::shared_ptr<TTransport> socket(_socket);
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
      incrementResponses();
      incrementVotes();
      cond.notify_one();
    } else {
      if (debug) {std::cout << "Vote rejected: " << voteID << std::endl;}
      currentTermLock.lock();
      if (response.term > currentTerm) {
        currentTerm = response.term;
      }
      currentTermLock.unlock();
      incrementResponses();
      cond.notify_one();
    }
    transport->close();
  } catch (TException &tx) {
    if(debug) {std::cout << "Connection Failed: " << voteID << std::endl;}
    incrementResponses();
    cond.notify_one();
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
    append.prevLogIndex = nextIndex[voteID]-1;
    append.prevLogTerm = raftLog[append.prevLogIndex].term;    
    append.leaderCommit = commitIndex;
    std::vector<LogEntry> send(raftLog.begin()+nextIndex[voteID], raftLog.end());
    append.entries = send;
    client.AppendEntriesRPC(response, append);
    transport->close();
  } catch (TException &tx) {
    //std::cout << "Connection Failed" << std::endl;
  }
}

void incrementResponses (void) {
  responseLock.lock();
  numResponse++;
  responseLock.unlock();
}

void resetResponses (void) {
  responseLock.lock();
  numResponse = 0;
  responseLock.unlock();
}

void incrementVotes (void) {
  voteLock.lock();
  numVotes++; 
  voteLock.unlock();
}

void resetVotes (void) {
  voteLock.lock();
  numVotes = 0;
  voteLock.unlock();
}

void resetTime (void) {
  msgTimeLock.lock();
  msgTime = std::chrono::system_clock::now();
  msgTimeLock.unlock();
}

void incrementCurrentTerm (void) {
  currentTermLock.lock();
  currentTerm++;
  currentTermLock.unlock();
  resetVotedFor();
}


void resetCurrentTerm (void) {
  currentTermLock.lock();
  currentTerm = 0;
  currentTermLock.unlock();
}

void setState (int newState) {
  stateLock.lock();
  state = newState;
  stateLock.unlock();
}

void setVotedFor (int newVote) {
  votedForLock.lock();
  votedFor = newVote;
  votedForLock.unlock();
}

void resetVotedFor (void) {
  votedForLock.lock();
  votedFor = NONE;
  votedForLock.unlock();
}

bool logUpToDate (RequestVote vote) {
  logLock.lock();
  LogEntry last = raftLog.back();
  if (vote.lastLogTerm > last.term) {
    logLock.unlock();
    return true;
  } else if (vote.lastLogTerm == last.term) {
    if (raftLog.size() - 1 <= vote.lastLogIndex) {
      logLock.unlock();
      return true;
    } else {
      logLock.unlock();
      return false;
    }
  } else {
    logLock.unlock();
    return false;
  }
}

// Implements the log update policy of Raft upon receiving a AppendEntriesRPC.
// Essentially, the server log defers to the leader raft log, so if there
// are any discrepancies, they are overwritten. This is checked using the term
// of the respective log entries.
// The log is immediately resized as appropriate Note that this is slightly
// different from the definition in the raft paper, which states that entries are 
// only deleted if there is a direct conflict. However, these entries would be   
// later written over anyway, as they are not in the leader's log. I assume this
// is what the original writers meant.
void resolveConflicts (const AppendEntries append) {
  raftLog.resize(append.prevLogIndex+append.entries.size()+1);
  std::vector<LogEntry>::iterator logEntry = 
  raftLog.begin() + append.prevLogIndex + 1; 
  for (std::vector<LogEntry>::const_iterator newEntry = append.entries.begin();
       newEntry != append.entries.end(); ++newEntry, ++logEntry) {
    *logEntry = *newEntry;
  }
  
  lastApplied = raftLog.size()-1;
  printLog();
}

void initialiseLogTest(void) {
    std::cout << "Initializing test" << std::endl;
    LogEntry second, third, fourth;
    second.term = currentTerm;
    third.term = currentTerm;
    fourth.term = currentTerm;
    second.number = 1;
    third.number = 2;
    fourth.number = 3;
    raftLog.push_back(second);
    raftLog.push_back(third);
    raftLog.push_back(fourth);
    lastApplied = 3; 
    for (int i = 0; i < NUM_SERVERS; i++) {
      nextIndex[i] = 1;
    }
}

void printLog(void) {
  for (std::vector<LogEntry>::const_iterator entry = raftLog.begin();
       entry != raftLog.end(); ++entry) {
    std::cout << entry->number << std::endl;
  }
}
