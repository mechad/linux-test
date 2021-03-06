/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Operation_TYPES_H
#define Operation_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>


namespace Chad {

struct Rtype {
  enum type {
    BOOL = 0,
    INTEGER = 1,
    STRING = 2,
    SET = 3,
    MAP = 4,
    LIST = 5,
    BIN = 6
  };
};

extern const std::map<int, const char*> _Rtype_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const Rtype::type& val);

class IOArgs;

class InvalidOperation;

typedef struct _IOArgs__isset {
  _IOArgs__isset() : type(false), status(false), i(false), s(false), m(false), l(false), b(false) {}
  bool type :1;
  bool status :1;
  bool i :1;
  bool s :1;
  bool m :1;
  bool l :1;
  bool b :1;
} _IOArgs__isset;

class IOArgs : public virtual ::apache::thrift::TBase {
 public:

  IOArgs(const IOArgs&);
  IOArgs& operator=(const IOArgs&);
  IOArgs() : type((Rtype::type)0), status(0), i(0), b() {
  }

  virtual ~IOArgs() throw();
  Rtype::type type;
  bool status;
  int32_t i;
  std::set<std::string>  s;
  std::map<std::string, std::string>  m;
  std::vector<std::string>  l;
  std::string b;

  _IOArgs__isset __isset;

  void __set_type(const Rtype::type val);

  void __set_status(const bool val);

  void __set_i(const int32_t val);

  void __set_s(const std::set<std::string> & val);

  void __set_m(const std::map<std::string, std::string> & val);

  void __set_l(const std::vector<std::string> & val);

  void __set_b(const std::string& val);

  bool operator == (const IOArgs & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (!(status == rhs.status))
      return false;
    if (!(i == rhs.i))
      return false;
    if (!(s == rhs.s))
      return false;
    if (!(m == rhs.m))
      return false;
    if (!(l == rhs.l))
      return false;
    if (!(b == rhs.b))
      return false;
    return true;
  }
  bool operator != (const IOArgs &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IOArgs & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(IOArgs &a, IOArgs &b);

std::ostream& operator<<(std::ostream& out, const IOArgs& obj);

typedef struct _InvalidOperation__isset {
  _InvalidOperation__isset() : whatOp(false), why(false) {}
  bool whatOp :1;
  bool why :1;
} _InvalidOperation__isset;

class InvalidOperation : public ::apache::thrift::TException {
 public:

  InvalidOperation(const InvalidOperation&);
  InvalidOperation& operator=(const InvalidOperation&);
  InvalidOperation() : whatOp(0), why() {
  }

  virtual ~InvalidOperation() throw();
  int32_t whatOp;
  std::string why;

  _InvalidOperation__isset __isset;

  void __set_whatOp(const int32_t val);

  void __set_why(const std::string& val);

  bool operator == (const InvalidOperation & rhs) const
  {
    if (!(whatOp == rhs.whatOp))
      return false;
    if (!(why == rhs.why))
      return false;
    return true;
  }
  bool operator != (const InvalidOperation &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InvalidOperation & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(InvalidOperation &a, InvalidOperation &b);

std::ostream& operator<<(std::ostream& out, const InvalidOperation& obj);

} // namespace

#endif
