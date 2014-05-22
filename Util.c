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

void viderBuffer() {
  int c=0;
  while((c=getchar()!='\n')&&(c!=EOF)) {
  	;
  }
}

/**
 * Découpe une chaîne de caractères en plusieurs sous chaînes d'après le caractère délimiteur.
 * Si des caractères délimiteurs se suivent alors des chaînes vides sont insérées.
 * @str la chaîne à découper (elle ne sera pas modifiée);
 * @delimiter le caractère délimiteur
 * @size la taille du pointeur de retour
 * @return un pointeur vers les défférents éléments de la chaîne, NULL si une affectation mémoire a râté
 **/
char** com_split(char* str, char delimiter, int* size){
    int len, max_len = 0, inter_len = 0;
    *size = 1; // Il y a forcément un tronçon au début
    for(len=0; str[len] != '\0'; len++){
        if(str[len] == delimiter){
            if(inter_len > max_len){
                max_len = inter_len;
                inter_len = 0;
            }
            *size = *size + 1;
        }else{
            inter_len++;
        }
    }
    if(inter_len > max_len){
        max_len = inter_len;
    }

    // On alloue de la place pour stocket nos différents futures chaînes
    char** split = malloc(sizeof(char*)*(*size));
    if(split == NULL)
        return NULL;

    // On initialise un buffer avec la taille du plus long tronçon qu'on a découvert
    char buf[max_len+1];
    buf[0] = '\0';
    int offset = 0;
    max_len = 1;
    *size = 0;
    // On parcourt à nouveau pour créer nos chaînes (on réutilise max_len pour voir la taille de ce qu'on lit)
    for(len=0;; len++){
        // Si on tombe sur un délimiteur
        if(str[len] == delimiter || str[len] == '\0'){
            // on alloue la place pour une nouvelle chaîne
            char* p = malloc(sizeof(char)*max_len);
            // Si problème d'allocation on ne va pas plus loin et on libère la mémoire de ce qu'on avait instancié
            if(p == NULL){
                for(len=0; len<*size; len++)
                    free(split[len]);
                free(split);
                return NULL;
            }
            // On ajoute cette chaîne dans le tableau
            split[*size] = p;
            *size = *size+1;
            // On copie le contenu
            buf[len-offset] = '\0';
            strcpy(p, buf);
            // On se prépare à analyser la chaîne suivante
            buf[0] = '\0';
            max_len = 1;
            offset = len+1;

            if(str[len] == '\0')
                break;
        }
        // Sinon on peut copier dans le buffer
        else{
            buf[len-offset] = str[len];
            max_len++;
        }
    }
    return split;
}
