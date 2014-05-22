
all: Client ServerSearch ServerPublish ServerClient

Client: Client.o Util.o
	gcc -o Client Client.o Util.o -lssl -lcrypto

ServerSearch: ServerSearch.o Util.o
	gcc -o ServerSearch ServerSearch.o Util.o -lssl -lcrypto

ServerPublish: ServerPublish.o Util.o
	gcc -o ServerPublish ServerPublish.o Util.o -lssl -lcrypto

ServerClient: ServerClient.o Util.o
	gcc -o ServerClient ServerClient.o Util.o -lssl -lcrypto

Client.o: Client.c Client.h Util.h
	gcc -o Client.o -c Client.c

ServerPublish.o: ServerPublish.c ServerPublish.h Util.h
	gcc -o ServerPublish.o -c ServerPublish.c

ServerSearch.o: ServerSearch.c ServerSearch.h Util.h
	gcc -o ServerSearch.o -c ServerSearch.c

ServerClient.o: ServerClient.c ServerClient.h Util.h
	gcc -o ServerClient.o -c ServerClient.c

Util.o: Util.c Util.h
	gcc -o Util.o -c Util.c

clean:
	rm -rf *.o
