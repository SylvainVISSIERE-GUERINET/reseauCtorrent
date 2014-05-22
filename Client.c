#include "Client.h"


void usage(){
  printf("usage : cliecho adresseIP_serveur(x.x.x.x) \n");
}

//fonction qui fera la requete afin de récupérer la liste des clients qui possède des fichiers correspondant aux mots clé
void search(int serverSocket, struct sockaddr_in * serv_addr, char* servIP, socklen_t len)  {
	int n;
	char sendbuf[BUFSIZ];
	struct hostent *hp;
	char *motClef;
	
	// le ServerSearch est au port 2222
	serv_addr->sin_port = htons(2222);


	hp = (struct hostent *)gethostbyname (servIP);

	if (hp == NULL) {
		fprintf(stderr, "Client: %s non trouve dans in /etc/hosts ou dans le DNS\n", servIP);
		exit(1);
	  }

	memcpy( & serv_addr->sin_addr ,  hp->h_addr,  hp->h_length);
	printf ("IP address: %s\n", inet_ntoa (serv_addr->sin_addr));

	// on ouvre un socket UDP
	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
		perror ("erreur socket");
		exit (1);
	}

	// on ping le serveur PING
	strcpy(sendbuf,"PING");
	n=sendto(serverSocket,(void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);

	// on recupere la reponse du serveur ACKPING
	n=recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)serv_addr, &len);
	sendbuf[n]='\0';
	printf("%s\n",sendbuf);

	// on va alors envoyer le mot clef de notre fichier
	// il faut donc demander au client d'entrer le mot clef
	printf("Veuillez entrer votre mot de passe s'il vous plait.");
	scanf("%s",motClef);
	viderBuffer();
	// on envoie alors le motClef au serveur
	strcpy(sendbuf,motClef);
	n=sendto(serverSocket,(void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);

	// une fois qu'il a recu notre mot clef, le serveur va nous envoyer les fichiers
	// on recoit donc le premier :
	n = recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, &len);
	sendbuf[n]='\0';
	printf("%s\n", sendbuf);

	// si il indique qu'aucun ne fichier correspondait a notre mot clef :
	if (strcmp(sendbuf, "Aucun fichier ne correspond a votre mot clef.\0")==0)
	{
		// TODO
	}
	// sinon
	// il risque d'y en avoir plusieurs et il va nous les envoyer dans une boucle while, on va donc les recevoir de la maniere suivante :
	while(strcmp(sendbuf,"Fin fichier\0")!=0)
	{
		n = recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, &len);
		sendbuf[n]='\0';
		printf("%s\n", sendbuf);
	}
	
	// on ne close pas le socket
}

void publish(int serverSocket, struct sockaddr_in * serv_addr, char* servIP, socklen_t len) {
	  int n;
	  char sendbuf[BUFSIZ], ok[BUFSIZ];
	  struct hostent *hp;  
	  char info[BUFSIZ];
	  char nom[BUFSIZ], type[BUFSIZ], liste[BUFSIZ], sha[20], ip[15],nbClef[BUFSIZ];


	  serv_addr->sin_port = htons(2223);

	  hp = (struct hostent *)gethostbyname (servIP);

	  if (hp == NULL) {
	    fprintf(stderr, "Client: %s non trouve dans in /etc/hosts ou dans le DNS\n", servIP);
	    exit(1);
	  }

	  memcpy( & serv_addr->sin_addr ,  hp->h_addr,  hp->h_length);
	  printf ("IP address: %s\n", inet_ntoa (serv_addr->sin_addr));
	   
	 /*
	 * Ouvrir socket UDP
	 */
	 if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
		 perror ("erreur socket");
		 exit (1);
	 }
	 strcpy(sendbuf,"PING");
	 n=sendto(serverSocket,(void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);

	 //set_timeout(serverSocket,5);
	 n=recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)serv_addr, &len);
	 printf("%s\n",sendbuf);
	 
	 FILE* fp=popen("ls -f Partage/", "r");
	 char line[130]; 
	 while(fgets( line, sizeof(line), fp))  {
	 	printf("%s\n", line);
	}
	 strcpy(ok,"non");
	 //compare a oui du coup si pas egale renvoie pas 0 donc on continue
	 while( strcmp(ok, "oui") ) {
	 	 printf("lequel des fichiers ci dessus souhaitez vous publier?\n");
	 	 scanf("%s",nom);
	 	 viderBuffer();
	 	 printf("quel est son type?(txt, png, jpeg, etc)\n");
	 	 scanf("%s",type);
	 	 viderBuffer();
	 	 printf("quel mots cles souhaitez vous associer a ce  fichier?(separe par des ';')\n");
	 	 scanf("%s",liste);
	 	 viderBuffer();
	 	 strcpy(sha,"sha1");
		 sprintf(info, "%s|%s|%s|%s", nom, type, liste, sha);
		 strcpy(sendbuf,info);
		 sendto(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);
		 printf(" info sended to server\n");
		 recvfrom(serverSocket, (void*) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)serv_addr, &len);
		 printf("%s\n", sendbuf);
		 printf("Es ce que ce sont les bonnes donner?(oui ou non)\n");
		 scanf("%s",ok);
		 viderBuffer();
	 }
	 strcpy(sendbuf, "INFO ACK");
	 sendto(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);
	 // server send publish ack
	 printf("publish ack?\n");
	 recvfrom(serverSocket, (void*) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)serv_addr, &len);
	 printf("Publication faite\n");
}

int main (int argc, char *argv[])
{

  int serverSocket, choixAction=-1;
  struct sockaddr_in  serv_addr, cli_addr;
  int ttl;	
  socklen_t len=sizeof(serv_addr);
  

  /* Verifier le nombre de paramètre en entrée */
  /* clientUDP <hostname> */
 if (argc != 2){
	 usage();
	 exit(1);
 }	

 	 /*
 	  * Fixe le port du client pour dialogue, UDP et TCP
 	  *
 	  memset( (char*) &cli_addr, 0, sizeof(cli_addr) );
 	  cli_addr.sin_family = PF_INET;
 	  cli_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
 	  /*port fixer par le systeme et donc fixe mais par defaut,
 	   *de toute façon pas de pb de savoir lequel c'est, vu que en udp: on s'en moque
 	   *et en TCP on verra bien
 	   *
 	  cli_addr.sin_port=htons(0);*/

	 /* 
	  * Remplir la structure  serv_addr avec l'adresse du serveur sauf port qui est def par publish ou search
	  */
	  memset( (char *) &serv_addr,0, sizeof(serv_addr) );
	  serv_addr.sin_family = PF_INET;
	  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	  while(choixAction<0 || choixAction>3) {
	  	printf("Que voulez vous faire?\nPublier un fichier(0)\nRecherche un fichier sur le serveur(1)\nTelecharger un fichier(2)\nQuitter(3)\n");
	  	scanf("%d", &choixAction);
	  	viderBuffer();
	  	switch(choixAction) {
	  		case 0:
	  			publish(serverSocket , &serv_addr, argv[1], len);
	  			break;
	  		case 1:
	  			//search(serverSocket , &serv_addr, argv[1], len);
	  			break;
	  		case 2:
	  			//TCP
	  			break;
	  		case 3:
	  			exit(0);
	  			break;
	  		default:
	  			printf("Commande invalide\n");
	  			break;
	  	}
	  }

}
