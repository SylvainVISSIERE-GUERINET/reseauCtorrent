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
