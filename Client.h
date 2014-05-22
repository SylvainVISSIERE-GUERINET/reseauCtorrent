/*
 * INCLUDES
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <time.h>
#include <sys/timeb.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"

/*
 * DEFINITIONS VARIABLES
 */

#define PORT_CLI_TCP 2277
#define PORT_SEARCH 2222
#define PORT_PUBLISH 2223
/* 
 *	PROTOTYPES  FONCTIONS
 */


//fonction permettant l'envoi de la requête search au serveur
void search(int, struct sockaddr_in *, char*, socklen_t);

//fonction permettant l'envoi de la requête publish au serveur
void publish(int, struct sockaddr_in*, char*, socklen_t);

//menu pour lancer dl
void quelFichier();

//fonction permettant le telechargmeent d'un fichier a l'aide de son nom, @ipsource, sha1
void getFile(char*, char*, char*);