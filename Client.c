#include "Client.h"
#include "Util.h"

void usage(){
  printf("usage : cliecho adresseIP_serveur(x.x.x.x) \n");
}

//fonction qui fera la requete afin de récupérer la liste des clients qui possède des fichiers correspondant aux mots clé
void search(int serverSocket, struct sockaddr_in * serv_addr, char* servIP, socklen_t len)  {
	int n;
	char sendbuf[BUFSIZ];
	
	// le ServerSearch est au port 2222
	serv_addr->sin_port = htons(2222);

	// on ouvre un socket UDP
	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
		perror ("erreur socket");
		exit (1);
	}

	 strcpy(sendbuf,"PING");
	 n=sendto(serverSocket,(void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)serv_addr, len);

	 //set_timeout(serverSocket,5);
	 n=recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)serv_addr, &len);
	 sendbuf[n]='\0';
	 printf("%s\n",sendbuf);

	
}

void publish(int serverSocket, struct sockaddr_in * serv_addr, char* servIP, socklen_t len) {
	  int n;
	  char sendbuf[BUFSIZ];
	  struct hostent *hp;  


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
	 sendbuf[n]='\0';
	 printf("%s\n",sendbuf);
}

int main (int argc, char *argv[])

{

  int serverSocket;
  struct sockaddr_in  serv_addr;
  int ttl;	
  socklen_t len=sizeof(serv_addr);
  

  /* Verifier le nombre de paramètre en entrée */
  /* clientUDP <hostname> <numero_port>        */
 if (argc != 2){
	 usage();
	 exit(1);
 }

	 /* 
	  * Remplir la structure  serv_addr avec l'adresse du serveur sauf port qui est def par publish ou search
	  */
	  memset( (char *) &serv_addr,0, sizeof(serv_addr) );
	  serv_addr.sin_family = PF_INET;
	  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	  publish(serverSocket , &serv_addr, argv[1], len);

}
