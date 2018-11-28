CC = g++ -std=c++11 -g

CFLAGS = -I./gen-cpp -I/usr/local/include/thrift

LFLAGS = -L/usr/local/lib

LDEXEFLAGS = -lthrift

OBJS = 	OperationService.o \
        Operation_types.o \
        Operation_constants.o

all:client server


OperationService.o:gen-cpp/OperationService.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
Operation_types.o: gen-cpp/Operation_types.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
Operation_constants.o: gen-cpp/Operation_constants.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
OperationServer.o: OperationServer.cpp
	$(CC) $(CFLAGS) -c $^ -o $@
Client.o: Client.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

server: $(OBJS) OperationServer.o
	$(CC) $(LFLAGS) $(OBJS) OperationServer.o  $(LDEXEFLAGS) -o $@

client: $(OBJS)  Client.o
	$(CC) $(LFLAGS) $(OBJS)  Client.o  $(LDEXEFLAGS) -o $@


clean:
	rm -f ./*.o client server
