/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "Operation_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace Chad {

int _kRtypeValues[] = {
  Rtype::BOOL,
  Rtype::INTEGER,
  Rtype::STRING,
  Rtype::SET,
  Rtype::MAP,
  Rtype::LIST,
  Rtype::BIN
};
const char* _kRtypeNames[] = {
  "BOOL",
  "INTEGER",
  "STRING",
  "SET",
  "MAP",
  "LIST",
  "BIN"
};
const std::map<int, const char*> _Rtype_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(7, _kRtypeValues, _kRtypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

std::ostream& operator<<(std::ostream& out, const Rtype::type& val) {
  std::map<int, const char*>::const_iterator it = _Rtype_VALUES_TO_NAMES.find(val);
  if (it != _Rtype_VALUES_TO_NAMES.end()) {
    out << it->second;
  } else {
    out << static_cast<int>(val);
  }
  return out;
}


IOArgs::~IOArgs() throw() {
}


void IOArgs::__set_type(const Rtype::type val) {
  this->type = val;
}

void IOArgs::__set_status(const bool val) {
  this->status = val;
}

void IOArgs::__set_i(const int32_t val) {
  this->i = val;
}

void IOArgs::__set_s(const std::set<std::string> & val) {
  this->s = val;
}

void IOArgs::__set_m(const std::map<std::string, std::string> & val) {
  this->m = val;
}

void IOArgs::__set_l(const std::vector<std::string> & val) {
  this->l = val;
}

void IOArgs::__set_b(const std::string& val) {
  this->b = val;
}
std::ostream& operator<<(std::ostream& out, const IOArgs& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t IOArgs::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->type = (Rtype::type)ecast0;
          this->__isset.type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->status);
          this->__isset.status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->i);
          this->__isset.i = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_SET) {
          {
            this->s.clear();
            uint32_t _size1;
            ::apache::thrift::protocol::TType _etype4;
            xfer += iprot->readSetBegin(_etype4, _size1);
            uint32_t _i5;
            for (_i5 = 0; _i5 < _size1; ++_i5)
            {
              std::string _elem6;
              xfer += iprot->readString(_elem6);
              this->s.insert(_elem6);
            }
            xfer += iprot->readSetEnd();
          }
          this->__isset.s = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->m.clear();
            uint32_t _size7;
            ::apache::thrift::protocol::TType _ktype8;
            ::apache::thrift::protocol::TType _vtype9;
            xfer += iprot->readMapBegin(_ktype8, _vtype9, _size7);
            uint32_t _i11;
            for (_i11 = 0; _i11 < _size7; ++_i11)
            {
              std::string _key12;
              xfer += iprot->readString(_key12);
              std::string& _val13 = this->m[_key12];
              xfer += iprot->readString(_val13);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.m = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->l.clear();
            uint32_t _size14;
            ::apache::thrift::protocol::TType _etype17;
            xfer += iprot->readListBegin(_etype17, _size14);
            this->l.resize(_size14);
            uint32_t _i18;
            for (_i18 = 0; _i18 < _size14; ++_i18)
            {
              xfer += iprot->readString(this->l[_i18]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.l = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->b);
          this->__isset.b = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t IOArgs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("IOArgs");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_BOOL, 2);
  xfer += oprot->writeBool(this->status);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("i", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->i);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("s", ::apache::thrift::protocol::T_SET, 4);
  {
    xfer += oprot->writeSetBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->s.size()));
    std::set<std::string> ::const_iterator _iter19;
    for (_iter19 = this->s.begin(); _iter19 != this->s.end(); ++_iter19)
    {
      xfer += oprot->writeString((*_iter19));
    }
    xfer += oprot->writeSetEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("m", ::apache::thrift::protocol::T_MAP, 5);
  {
    xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_STRING, ::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->m.size()));
    std::map<std::string, std::string> ::const_iterator _iter20;
    for (_iter20 = this->m.begin(); _iter20 != this->m.end(); ++_iter20)
    {
      xfer += oprot->writeString(_iter20->first);
      xfer += oprot->writeString(_iter20->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("l", ::apache::thrift::protocol::T_LIST, 6);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->l.size()));
    std::vector<std::string> ::const_iterator _iter21;
    for (_iter21 = this->l.begin(); _iter21 != this->l.end(); ++_iter21)
    {
      xfer += oprot->writeString((*_iter21));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("b", ::apache::thrift::protocol::T_STRING, 7);
  xfer += oprot->writeBinary(this->b);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(IOArgs &a, IOArgs &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.status, b.status);
  swap(a.i, b.i);
  swap(a.s, b.s);
  swap(a.m, b.m);
  swap(a.l, b.l);
  swap(a.b, b.b);
  swap(a.__isset, b.__isset);
}

IOArgs::IOArgs(const IOArgs& other22) {
  type = other22.type;
  status = other22.status;
  i = other22.i;
  s = other22.s;
  m = other22.m;
  l = other22.l;
  b = other22.b;
  __isset = other22.__isset;
}
IOArgs& IOArgs::operator=(const IOArgs& other23) {
  type = other23.type;
  status = other23.status;
  i = other23.i;
  s = other23.s;
  m = other23.m;
  l = other23.l;
  b = other23.b;
  __isset = other23.__isset;
  return *this;
}
void IOArgs::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "IOArgs(";
  out << "type=" << to_string(type);
  out << ", " << "status=" << to_string(status);
  out << ", " << "i=" << to_string(i);
  out << ", " << "s=" << to_string(s);
  out << ", " << "m=" << to_string(m);
  out << ", " << "l=" << to_string(l);
  out << ", " << "b=" << to_string(b);
  out << ")";
}


InvalidOperation::~InvalidOperation() throw() {
}


void InvalidOperation::__set_whatOp(const int32_t val) {
  this->whatOp = val;
}

void InvalidOperation::__set_why(const std::string& val) {
  this->why = val;
}
std::ostream& operator<<(std::ostream& out, const InvalidOperation& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t InvalidOperation::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->whatOp);
          this->__isset.whatOp = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->why);
          this->__isset.why = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t InvalidOperation::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("InvalidOperation");

  xfer += oprot->writeFieldBegin("whatOp", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->whatOp);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("why", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->why);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(InvalidOperation &a, InvalidOperation &b) {
  using ::std::swap;
  swap(a.whatOp, b.whatOp);
  swap(a.why, b.why);
  swap(a.__isset, b.__isset);
}

InvalidOperation::InvalidOperation(const InvalidOperation& other24) : TException() {
  whatOp = other24.whatOp;
  why = other24.why;
  __isset = other24.__isset;
}
InvalidOperation& InvalidOperation::operator=(const InvalidOperation& other25) {
  whatOp = other25.whatOp;
  why = other25.why;
  __isset = other25.__isset;
  return *this;
}
void InvalidOperation::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "InvalidOperation(";
  out << "whatOp=" << to_string(whatOp);
  out << ", " << "why=" << to_string(why);
  out << ")";
}

const char* InvalidOperation::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: InvalidOperation";
  }
}

} // namespace
