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


/* 
 *	PROTOTYPES  FONCTIONS
 */


//fonction permettant l'envoi de la requête search au serveur
void search(int, struct sockaddr_in *, char*, socklen_t);

//fonction permettant l'envoi de la requête publish au serveur
void publish(int, struct sockaddr_in*, char*, socklen_t);
