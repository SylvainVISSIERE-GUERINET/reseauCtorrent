
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
#include <openssl/sha.h>

void set_timeout(int, int);

void viderBuffer();

<<<<<<< HEAD
char *toSha1(char *);
=======

/**
 * Découpe une chaîne de caractères en plusieurs sous chaînes d'après le caractère délimiteur.
 * Si des caractères délimiteurs se suivent alors des chaînes vides sont insérées.
 * @str la chaîne à découper (elle ne sera pas modifiée);
 * @delimiter le caractère délimiteur
 * @size la taille du pointeur de retour
 * @return un pointeur vers les défférents éléments de la chaîne, NULL si une affectation mémoire a râté
 **/
char** com_split(char*, char , int* );
>>>>>>> bbea00a8a69648e9a2fec9869146f597d94b6cca
