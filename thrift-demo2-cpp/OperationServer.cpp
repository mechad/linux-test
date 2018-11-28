// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/OperationService.h"
#include "gen-cpp/Operation_types.h"
#include <thrift/server/TSimpleServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <iostream>
#include <map>

using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::Chad;

class OperationServiceHandler : virtual public OperationServiceIf {
 public:
  OperationServiceHandler() {
    // Your initialization goes here
  }

  bool Ping() {
    // Your implementation goes here
    printf("Ping\n");
  }

  void Set(std::string& _return, const std::map<std::string, std::string> & m) {
    // Your implementation goes here
    printf("Set\n");
    std::map<std::string, std::string>::const_iterator iter;
    for(iter = m.begin(); iter != m.end(); iter++)
        cout << iter->first <<':'<< iter->second <<endl;

    _return = "success";
  }

  void Get(std::map<std::string, std::string> & _return, const std::vector<std::string> & l) {
    // Your implementation goes here
    printf("Get\n");
    int i;
    for(int i=0; i < l.size(); i++) cout << l[i] << ",";

    cout << endl;

    _return.insert(pair<string, string>("status", "success"));
    _return.insert(pair<string, string>("result", "for server."));
  }

  void Op(IOArgs& _return, const IOArgs& io_args) {
    // Your implementation goes here
    printf("Op\n");
    
    switch(io_args.type) {
        case Rtype::type::BIN: 
        {
            cout << "Rec Binary data[" << io_args.b.size() << "]:" << endl;
            const char *p = io_args.b.c_str();
            for(int i = 0; i < io_args.b.size(); i++)
                printf("0x%02X ",p[i]);
            cout << endl;
        } 
        break;
        case Rtype::type::INTEGER:
        break;
        case Rtype::type::STRING:
        break;
        case Rtype::type::SET:
        break;
        case Rtype::type::MAP:
        break;
        case Rtype::type::LIST:
        break;
        default:
            InvalidOperation op;
            op.whatOp = -1;
            op.why = "Unrecognizable type";
            throw op;
    }
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::apache::thrift::stdcxx::shared_ptr<OperationServiceHandler> handler(new OperationServiceHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new OperationServiceProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  //TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

