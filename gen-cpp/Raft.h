/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Raft_H
#define Raft_H

#include <thrift/TDispatchProcessor.h>
#include "raft_types.h"



class RaftIf {
 public:
  virtual ~RaftIf() {}
  virtual void RequestVoteRPC(VoteResponse& _return, const RequestVote& vote) = 0;
  virtual void AppendEntriesRPC(AppendResponse& _return, const AppendEntries& append) = 0;
};

class RaftIfFactory {
 public:
  typedef RaftIf Handler;

  virtual ~RaftIfFactory() {}

  virtual RaftIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(RaftIf* /* handler */) = 0;
};

class RaftIfSingletonFactory : virtual public RaftIfFactory {
 public:
  RaftIfSingletonFactory(const boost::shared_ptr<RaftIf>& iface) : iface_(iface) {}
  virtual ~RaftIfSingletonFactory() {}

  virtual RaftIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(RaftIf* /* handler */) {}

 protected:
  boost::shared_ptr<RaftIf> iface_;
};

class RaftNull : virtual public RaftIf {
 public:
  virtual ~RaftNull() {}
  void RequestVoteRPC(VoteResponse& /* _return */, const RequestVote& /* vote */) {
    return;
  }
  void AppendEntriesRPC(AppendResponse& /* _return */, const AppendEntries& /* append */) {
    return;
  }
};

typedef struct _Raft_RequestVoteRPC_args__isset {
  _Raft_RequestVoteRPC_args__isset() : vote(false) {}
  bool vote :1;
} _Raft_RequestVoteRPC_args__isset;

class Raft_RequestVoteRPC_args {
 public:

  static const char* ascii_fingerprint; // = "D21A2677CCE0C12145D6955792CDF5EE";
  static const uint8_t binary_fingerprint[16]; // = {0xD2,0x1A,0x26,0x77,0xCC,0xE0,0xC1,0x21,0x45,0xD6,0x95,0x57,0x92,0xCD,0xF5,0xEE};

  Raft_RequestVoteRPC_args(const Raft_RequestVoteRPC_args&);
  Raft_RequestVoteRPC_args& operator=(const Raft_RequestVoteRPC_args&);
  Raft_RequestVoteRPC_args() {
  }

  virtual ~Raft_RequestVoteRPC_args() throw();
  RequestVote vote;

  _Raft_RequestVoteRPC_args__isset __isset;

  void __set_vote(const RequestVote& val);

  bool operator == (const Raft_RequestVoteRPC_args & rhs) const
  {
    if (!(vote == rhs.vote))
      return false;
    return true;
  }
  bool operator != (const Raft_RequestVoteRPC_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Raft_RequestVoteRPC_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_RequestVoteRPC_args& obj);
};


class Raft_RequestVoteRPC_pargs {
 public:

  static const char* ascii_fingerprint; // = "D21A2677CCE0C12145D6955792CDF5EE";
  static const uint8_t binary_fingerprint[16]; // = {0xD2,0x1A,0x26,0x77,0xCC,0xE0,0xC1,0x21,0x45,0xD6,0x95,0x57,0x92,0xCD,0xF5,0xEE};


  virtual ~Raft_RequestVoteRPC_pargs() throw();
  const RequestVote* vote;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_RequestVoteRPC_pargs& obj);
};

typedef struct _Raft_RequestVoteRPC_result__isset {
  _Raft_RequestVoteRPC_result__isset() : success(false) {}
  bool success :1;
} _Raft_RequestVoteRPC_result__isset;

class Raft_RequestVoteRPC_result {
 public:

  static const char* ascii_fingerprint; // = "49E764C8B69512E0ECAFC9B0C79DBFA8";
  static const uint8_t binary_fingerprint[16]; // = {0x49,0xE7,0x64,0xC8,0xB6,0x95,0x12,0xE0,0xEC,0xAF,0xC9,0xB0,0xC7,0x9D,0xBF,0xA8};

  Raft_RequestVoteRPC_result(const Raft_RequestVoteRPC_result&);
  Raft_RequestVoteRPC_result& operator=(const Raft_RequestVoteRPC_result&);
  Raft_RequestVoteRPC_result() {
  }

  virtual ~Raft_RequestVoteRPC_result() throw();
  VoteResponse success;

  _Raft_RequestVoteRPC_result__isset __isset;

  void __set_success(const VoteResponse& val);

  bool operator == (const Raft_RequestVoteRPC_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Raft_RequestVoteRPC_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Raft_RequestVoteRPC_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_RequestVoteRPC_result& obj);
};

typedef struct _Raft_RequestVoteRPC_presult__isset {
  _Raft_RequestVoteRPC_presult__isset() : success(false) {}
  bool success :1;
} _Raft_RequestVoteRPC_presult__isset;

class Raft_RequestVoteRPC_presult {
 public:

  static const char* ascii_fingerprint; // = "49E764C8B69512E0ECAFC9B0C79DBFA8";
  static const uint8_t binary_fingerprint[16]; // = {0x49,0xE7,0x64,0xC8,0xB6,0x95,0x12,0xE0,0xEC,0xAF,0xC9,0xB0,0xC7,0x9D,0xBF,0xA8};


  virtual ~Raft_RequestVoteRPC_presult() throw();
  VoteResponse* success;

  _Raft_RequestVoteRPC_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const Raft_RequestVoteRPC_presult& obj);
};

typedef struct _Raft_AppendEntriesRPC_args__isset {
  _Raft_AppendEntriesRPC_args__isset() : append(false) {}
  bool append :1;
} _Raft_AppendEntriesRPC_args__isset;

class Raft_AppendEntriesRPC_args {
 public:

  static const char* ascii_fingerprint; // = "081224816F45AC953E6344B7AC98D1A8";
  static const uint8_t binary_fingerprint[16]; // = {0x08,0x12,0x24,0x81,0x6F,0x45,0xAC,0x95,0x3E,0x63,0x44,0xB7,0xAC,0x98,0xD1,0xA8};

  Raft_AppendEntriesRPC_args(const Raft_AppendEntriesRPC_args&);
  Raft_AppendEntriesRPC_args& operator=(const Raft_AppendEntriesRPC_args&);
  Raft_AppendEntriesRPC_args() {
  }

  virtual ~Raft_AppendEntriesRPC_args() throw();
  AppendEntries append;

  _Raft_AppendEntriesRPC_args__isset __isset;

  void __set_append(const AppendEntries& val);

  bool operator == (const Raft_AppendEntriesRPC_args & rhs) const
  {
    if (!(append == rhs.append))
      return false;
    return true;
  }
  bool operator != (const Raft_AppendEntriesRPC_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Raft_AppendEntriesRPC_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_AppendEntriesRPC_args& obj);
};


class Raft_AppendEntriesRPC_pargs {
 public:

  static const char* ascii_fingerprint; // = "081224816F45AC953E6344B7AC98D1A8";
  static const uint8_t binary_fingerprint[16]; // = {0x08,0x12,0x24,0x81,0x6F,0x45,0xAC,0x95,0x3E,0x63,0x44,0xB7,0xAC,0x98,0xD1,0xA8};


  virtual ~Raft_AppendEntriesRPC_pargs() throw();
  const AppendEntries* append;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_AppendEntriesRPC_pargs& obj);
};

typedef struct _Raft_AppendEntriesRPC_result__isset {
  _Raft_AppendEntriesRPC_result__isset() : success(false) {}
  bool success :1;
} _Raft_AppendEntriesRPC_result__isset;

class Raft_AppendEntriesRPC_result {
 public:

  static const char* ascii_fingerprint; // = "49E764C8B69512E0ECAFC9B0C79DBFA8";
  static const uint8_t binary_fingerprint[16]; // = {0x49,0xE7,0x64,0xC8,0xB6,0x95,0x12,0xE0,0xEC,0xAF,0xC9,0xB0,0xC7,0x9D,0xBF,0xA8};

  Raft_AppendEntriesRPC_result(const Raft_AppendEntriesRPC_result&);
  Raft_AppendEntriesRPC_result& operator=(const Raft_AppendEntriesRPC_result&);
  Raft_AppendEntriesRPC_result() {
  }

  virtual ~Raft_AppendEntriesRPC_result() throw();
  AppendResponse success;

  _Raft_AppendEntriesRPC_result__isset __isset;

  void __set_success(const AppendResponse& val);

  bool operator == (const Raft_AppendEntriesRPC_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Raft_AppendEntriesRPC_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Raft_AppendEntriesRPC_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const Raft_AppendEntriesRPC_result& obj);
};

typedef struct _Raft_AppendEntriesRPC_presult__isset {
  _Raft_AppendEntriesRPC_presult__isset() : success(false) {}
  bool success :1;
} _Raft_AppendEntriesRPC_presult__isset;

class Raft_AppendEntriesRPC_presult {
 public:

  static const char* ascii_fingerprint; // = "49E764C8B69512E0ECAFC9B0C79DBFA8";
  static const uint8_t binary_fingerprint[16]; // = {0x49,0xE7,0x64,0xC8,0xB6,0x95,0x12,0xE0,0xEC,0xAF,0xC9,0xB0,0xC7,0x9D,0xBF,0xA8};


  virtual ~Raft_AppendEntriesRPC_presult() throw();
  AppendResponse* success;

  _Raft_AppendEntriesRPC_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const Raft_AppendEntriesRPC_presult& obj);
};

class RaftClient : virtual public RaftIf {
 public:
  RaftClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RaftClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void RequestVoteRPC(VoteResponse& _return, const RequestVote& vote);
  void send_RequestVoteRPC(const RequestVote& vote);
  void recv_RequestVoteRPC(VoteResponse& _return);
  void AppendEntriesRPC(AppendResponse& _return, const AppendEntries& append);
  void send_AppendEntriesRPC(const AppendEntries& append);
  void recv_AppendEntriesRPC(AppendResponse& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class RaftProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<RaftIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (RaftProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_RequestVoteRPC(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_AppendEntriesRPC(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  RaftProcessor(boost::shared_ptr<RaftIf> iface) :
    iface_(iface) {
    processMap_["RequestVoteRPC"] = &RaftProcessor::process_RequestVoteRPC;
    processMap_["AppendEntriesRPC"] = &RaftProcessor::process_AppendEntriesRPC;
  }

  virtual ~RaftProcessor() {}
};

class RaftProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  RaftProcessorFactory(const ::boost::shared_ptr< RaftIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< RaftIfFactory > handlerFactory_;
};

class RaftMultiface : virtual public RaftIf {
 public:
  RaftMultiface(std::vector<boost::shared_ptr<RaftIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~RaftMultiface() {}
 protected:
  std::vector<boost::shared_ptr<RaftIf> > ifaces_;
  RaftMultiface() {}
  void add(boost::shared_ptr<RaftIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void RequestVoteRPC(VoteResponse& _return, const RequestVote& vote) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RequestVoteRPC(_return, vote);
    }
    ifaces_[i]->RequestVoteRPC(_return, vote);
    return;
  }

  void AppendEntriesRPC(AppendResponse& _return, const AppendEntries& append) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->AppendEntriesRPC(_return, append);
    }
    ifaces_[i]->AppendEntriesRPC(_return, append);
    return;
  }

};



#endif
