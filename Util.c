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


char *toSha1(char* nomFichier){ // On ouvre le fichier en mode bianire et on annule sur une erreur
	FILE* fichier = NULL;
	char hash[SHA_DIGEST_LENGTH];
	char buffer[16384];
	int lengthBuffer = sizeof(buffer);
	int nbCaractParcourus;
	//char bonsCaract[2*SHA_DIGEST_LENGTH+1];
	char *bonsCaract;

	// on ouvre le fichier en binaire (d'ou le "rb")
	fichier = fopen(nomFichier, "rb");
	if(fichier == NULL)
	{
		printf("toSha1 : le fichier %s n'a pas été trouvé.", nomFichier);
		return NULL;
	}

	// on parcourt le fichier en petites parties au cas où il serait trop grand.
	bonsCaract = (char *)malloc(sizeof(char)*(SHA_DIGEST_LENGTH*2+1));
	// Initialisation de la structure de SHA1
	SHA_CTX structSha;
	if(SHA1_Init(&structSha) == 0)
	{
		printf("toSha1 : problème dans l'initialisation de SHA1");
		return NULL;
	}

	nbCaractParcourus = 1; // on initialise nbCaractParcourus à 1 pour entrer dans la boucle

	// on fait une boucle : tant qu'il y a des choses à lire dans le fichier
	while(nbCaractParcourus > 0)
	{
		// on avance dans le fichier
		nbCaractParcourus = fread(buffer, 1, lengthBuffer, fichier);
		
		// Update de la structure de SHA1
		if(SHA1_Update(&structSha, buffer, nbCaractParcourus) == 0)
		{
			printf("toSha1 : problème dans l'updating de SHA1");
			// on ferme le fichier
			fclose(fichier);
			return NULL;
		}
	}

	// On ferme le fichier
	fclose(fichier);

	// Final de la structure de SHA1
	if(SHA1_Final(hash, &structSha) == 0)
	{
		printf("toSha1 : problème dans le final de SHA1");
		return NULL;
	}


	// Comme on va transférer le contenu dans une chaîne de caractères, on va utiliser des "bons" caractères
	// on splitera donc chaque caractère du hash actuel en deux caractères du futur hash


	//char* hash_printable = malloc(sizeof(char)*(SHA_DIGEST_LENGTH*2+1));
	//if (bonsCaract == NULL) return NULL;

	int i;
	for(i=0; i<SHA_DIGEST_LENGTH; i++)
	{
		sprintf(bonsCaract+(2*i), "%02X", 0xff & hash[i]);
	}
	// on ajoute le EOF
	bonsCaract[40] = '\0';

	return bonsCaract;
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
