/*
 * INCLUDES
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * DEFINITIONS VARIABLES
 */


/* 
 *	PROTOTYPES  FONCTIONS
 */

void usage();

//fonction permettant l'envoi de la requête search au serveur
void search(char*[]);

//fonction permettant l'envoi de la requête publish au serveur
void publish(char*,char*[]);