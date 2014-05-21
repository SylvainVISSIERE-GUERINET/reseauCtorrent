#include "Util.h"

void set_timeout(int socket, int secondes){
    // Réglage du timeout
    struct timeval time;
    time.tv_sec = secondes;
    time.tv_usec = 0;
    if(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)) < 0){
        perror("erreur de définition de timeout");
    }
}