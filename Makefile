CC = gcc
LDLIBS = -lssl -lcrypto
CFLAGS = -O1 -march=native -pipe
all: Client ServerSearch ServerPublish ServerClient

Client: Client.o Util.o
	$(CC) $(LDLIBS) -o Client Client.o Util.o

ServerSearch: ServerSearch.o Util.o
	$(CC) $(LDLIBS) -o ServerSearch ServerSearch.o Util.o

ServerPublish: ServerPublish.o Util.o
	$(CC) $(LDLIBS) -o ServerPublish ServerPublish.o Util.o

ServerClient: ServerClient.o Util.o
	$(CC) $(LDLIBS) -o ServerClient ServerClient.o Util.o

Client.o: Client.c Client.h Util.h
	$(CC) $(CFLAGS) -o Client.o -c Client.c

ServerPublish.o: ServerPublish.c ServerPublish.h Util.h
	$(CC) $(CFLAGS) -o ServerPublish.o -c ServerPublish.c

ServerSearch.o: ServerSearch.c ServerSearch.h Util.h
	$(CC) $(CFLAGS) -o ServerSearch.o -c ServerSearch.c

ServerClient.o: ServerClient.c ServerClient.h Util.h
	$(CC) $(CFLAGS) -o ServerClient.o -c ServerClient.c

Util.o: Util.c Util.h
	$(CC) $(CFLAGS) -o Util.o -c Util.c

clean:
	rm -rf *.o
