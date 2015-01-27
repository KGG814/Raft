/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef raft_TYPES_H
#define raft_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




class RequestVote;

class VoteResponse;

class LogEntry;

class AppendEntries;

class AppendResponse;

typedef struct _RequestVote__isset {
  _RequestVote__isset() : term(false), candidateID(false), lastLogIndex(false), lastLogTerm(false) {}
  bool term :1;
  bool candidateID :1;
  bool lastLogIndex :1;
  bool lastLogTerm :1;
} _RequestVote__isset;

class RequestVote {
 public:

  static const char* ascii_fingerprint; // = "154BB42C2FFD70F8B3993568C50C5613";
  static const uint8_t binary_fingerprint[16]; // = {0x15,0x4B,0xB4,0x2C,0x2F,0xFD,0x70,0xF8,0xB3,0x99,0x35,0x68,0xC5,0x0C,0x56,0x13};

  RequestVote(const RequestVote&);
  RequestVote& operator=(const RequestVote&);
  RequestVote() : term(0), candidateID(0), lastLogIndex(0), lastLogTerm(0) {
  }

  virtual ~RequestVote() throw();
  int32_t term;
  int32_t candidateID;
  int32_t lastLogIndex;
  int32_t lastLogTerm;

  _RequestVote__isset __isset;

  void __set_term(const int32_t val);

  void __set_candidateID(const int32_t val);

  void __set_lastLogIndex(const int32_t val);

  void __set_lastLogTerm(const int32_t val);

  bool operator == (const RequestVote & rhs) const
  {
    if (!(term == rhs.term))
      return false;
    if (!(candidateID == rhs.candidateID))
      return false;
    if (!(lastLogIndex == rhs.lastLogIndex))
      return false;
    if (!(lastLogTerm == rhs.lastLogTerm))
      return false;
    return true;
  }
  bool operator != (const RequestVote &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RequestVote & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const RequestVote& obj);
};

void swap(RequestVote &a, RequestVote &b);

typedef struct _VoteResponse__isset {
  _VoteResponse__isset() : term(false), voteGranted(false) {}
  bool term :1;
  bool voteGranted :1;
} _VoteResponse__isset;

class VoteResponse {
 public:

  static const char* ascii_fingerprint; // = "4DC0C1A1F380340B40244ADC7FB0BA60";
  static const uint8_t binary_fingerprint[16]; // = {0x4D,0xC0,0xC1,0xA1,0xF3,0x80,0x34,0x0B,0x40,0x24,0x4A,0xDC,0x7F,0xB0,0xBA,0x60};

  VoteResponse(const VoteResponse&);
  VoteResponse& operator=(const VoteResponse&);
  VoteResponse() : term(0), voteGranted(0) {
  }

  virtual ~VoteResponse() throw();
  int32_t term;
  bool voteGranted;

  _VoteResponse__isset __isset;

  void __set_term(const int32_t val);

  void __set_voteGranted(const bool val);

  bool operator == (const VoteResponse & rhs) const
  {
    if (!(term == rhs.term))
      return false;
    if (!(voteGranted == rhs.voteGranted))
      return false;
    return true;
  }
  bool operator != (const VoteResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VoteResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VoteResponse& obj);
};

void swap(VoteResponse &a, VoteResponse &b);

typedef struct _LogEntry__isset {
  _LogEntry__isset() : number(false), term(false) {}
  bool number :1;
  bool term :1;
} _LogEntry__isset;

class LogEntry {
 public:

  static const char* ascii_fingerprint; // = "989D1F1AE8D148D5E2119FFEC4BBBEE3";
  static const uint8_t binary_fingerprint[16]; // = {0x98,0x9D,0x1F,0x1A,0xE8,0xD1,0x48,0xD5,0xE2,0x11,0x9F,0xFE,0xC4,0xBB,0xBE,0xE3};

  LogEntry(const LogEntry&);
  LogEntry& operator=(const LogEntry&);
  LogEntry() : number(0), term(0) {
  }

  virtual ~LogEntry() throw();
  int32_t number;
  int32_t term;

  _LogEntry__isset __isset;

  void __set_number(const int32_t val);

  void __set_term(const int32_t val);

  bool operator == (const LogEntry & rhs) const
  {
    if (!(number == rhs.number))
      return false;
    if (!(term == rhs.term))
      return false;
    return true;
  }
  bool operator != (const LogEntry &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LogEntry & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const LogEntry& obj);
};

void swap(LogEntry &a, LogEntry &b);

typedef struct _AppendEntries__isset {
  _AppendEntries__isset() : term(false), leaderID(false), prevLogIndex(false), prevLogTerm(false), entries(false), leaderCommit(false) {}
  bool term :1;
  bool leaderID :1;
  bool prevLogIndex :1;
  bool prevLogTerm :1;
  bool entries :1;
  bool leaderCommit :1;
} _AppendEntries__isset;

class AppendEntries {
 public:

  static const char* ascii_fingerprint; // = "9C0FF7CFB3FE157E2B16105619A27992";
  static const uint8_t binary_fingerprint[16]; // = {0x9C,0x0F,0xF7,0xCF,0xB3,0xFE,0x15,0x7E,0x2B,0x16,0x10,0x56,0x19,0xA2,0x79,0x92};

  AppendEntries(const AppendEntries&);
  AppendEntries& operator=(const AppendEntries&);
  AppendEntries() : term(0), leaderID(0), prevLogIndex(0), prevLogTerm(0), leaderCommit(0) {
  }

  virtual ~AppendEntries() throw();
  int32_t term;
  int32_t leaderID;
  int32_t prevLogIndex;
  int32_t prevLogTerm;
  std::vector<LogEntry>  entries;
  int32_t leaderCommit;

  _AppendEntries__isset __isset;

  void __set_term(const int32_t val);

  void __set_leaderID(const int32_t val);

  void __set_prevLogIndex(const int32_t val);

  void __set_prevLogTerm(const int32_t val);

  void __set_entries(const std::vector<LogEntry> & val);

  void __set_leaderCommit(const int32_t val);

  bool operator == (const AppendEntries & rhs) const
  {
    if (!(term == rhs.term))
      return false;
    if (!(leaderID == rhs.leaderID))
      return false;
    if (!(prevLogIndex == rhs.prevLogIndex))
      return false;
    if (!(prevLogTerm == rhs.prevLogTerm))
      return false;
    if (!(entries == rhs.entries))
      return false;
    if (!(leaderCommit == rhs.leaderCommit))
      return false;
    return true;
  }
  bool operator != (const AppendEntries &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AppendEntries & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const AppendEntries& obj);
};

void swap(AppendEntries &a, AppendEntries &b);

typedef struct _AppendResponse__isset {
  _AppendResponse__isset() : term(false), success(false) {}
  bool term :1;
  bool success :1;
} _AppendResponse__isset;

class AppendResponse {
 public:

  static const char* ascii_fingerprint; // = "4DC0C1A1F380340B40244ADC7FB0BA60";
  static const uint8_t binary_fingerprint[16]; // = {0x4D,0xC0,0xC1,0xA1,0xF3,0x80,0x34,0x0B,0x40,0x24,0x4A,0xDC,0x7F,0xB0,0xBA,0x60};

  AppendResponse(const AppendResponse&);
  AppendResponse& operator=(const AppendResponse&);
  AppendResponse() : term(0), success(0) {
  }

  virtual ~AppendResponse() throw();
  int32_t term;
  bool success;

  _AppendResponse__isset __isset;

  void __set_term(const int32_t val);

  void __set_success(const bool val);

  bool operator == (const AppendResponse & rhs) const
  {
    if (!(term == rhs.term))
      return false;
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const AppendResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AppendResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const AppendResponse& obj);
};

void swap(AppendResponse &a, AppendResponse &b);



#endif
