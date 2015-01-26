struct RequestVote {
  1: i32 term;
  2: i32 candidateID;
  3: i32 lastLogIndex;
  4: i32 lastLogTerm;
}

struct VoteResponse {
  1: i32 term;
  2: bool voteGranted;
}

struct AppendEntries {
  1: i32 term;
  2: i32 leaderID;
  3: i32 prevLogIndex;
  4: i32 prevLogTerm;
  5: list<i32> entries;
  6: i32 leaderCommit;
}

struct AppendResponse {
  1: i32 term;
  2: bool success;
}

service Raft {
  bool RequestVoteRPC(1:RequestVote vote);
  bool AppendEntriesRPC(1:AppendEntries append);
}

