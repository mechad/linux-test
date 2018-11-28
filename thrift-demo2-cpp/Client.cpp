/*
 * @file: client.cpp
 * @create time: 2018-11-27 10:07:49
 * @last modified: 2018-11-27 10:07:49
 * @description:
 */
#include "gen-cpp/OperationService.h"
#include "gen-cpp/Operation_types.h"
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <thrift/stdcxx.h>
#include <iostream>

using namespace ::Chad;
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

int main(int argc, char **argv)
{
    stdcxx::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
    stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    try {
        transport->open();

        OperationServiceClient client(protocol);
        client.Ping();
        std::map<std::string, std::string> m;

        m["key1"] = "value1";
        m["key2"] = "value2";
        m["key3"] = "value3";

        std::string sret = "";
        client.Set(sret, m);
        std::cout << "result:" << sret << std::endl;

        vector<string> v;
        v.push_back("test1");
        v.push_back("test2");

        map<string,string> mapret;
        client.Get(mapret, v);
        map<string, string>::iterator iter;
        for(iter = mapret.begin(); iter != mapret.end(); iter++)
            std::cout << iter->first <<':'<< iter->second << std::endl;

        IOArgs img,_result;

        img.type = Rtype::type::BIN;
        char buf[] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,0,0,0,1,2,3};
        char *b = (char*)malloc(sizeof(buf));
        memcpy(b,buf,sizeof(buf));
        img.b = string(buf,sizeof(buf));
        try {
            cout << "Send binary data["<< sizeof(buf) <<"]:" << endl;
            const char *p = img.b.c_str(); 
            for(int i = 0; i < img.b.size(); i++)
                printf("0x%02X ",p[i]);
            cout << endl;
            client.Op(_result, img);
        } catch (InvalidOperation& io) {
            cout << "InvalidOperation: " << io.why << endl;
        }
        free(b);
        transport->close();

    } catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }
    return 0;
}

