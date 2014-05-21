#include "Client.h"
#include "Util.h"

usage(){
  printf("usage : cliecho adresseIP_serveur(x.x.x.x)  numero_port_serveur\n");
}

//fonction qui fera la requete afin de récupérer la liste des clients qui possède des fichiers correspondant aux mots clé
void search(char* motsCles[])  {

}

void publish(char* nomFichier, char* motsCles[]) {
 
}

int main (int argc, char *argv[])

{

  int serverSocket, unixSocket,  n, retread;
  struct sockaddr_in  serv_addr;
  struct sockaddr_un  unixAddr;
  char* data;
  char sendbuf[BUFSIZ];
  struct timeb tp;
  struct hostent *hp;  
  int ttl;	
  socklen_t len=sizeof(serv_addr);
  

  /* Verifier le nombre de paramètre en entrée */
  /* clientUDP <hostname> <numero_port>        */
 if (argc != 3){
	 usage();
	 exit(1);
 }



 /* 
  * Remplir la structure  serv_addr avec l'adresse du serveur 
  */
  memset( (char *) &serv_addr,0, sizeof(serv_addr) );
  serv_addr.sin_family = PF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));
  
  hp = (struct hostent *)gethostbyname (argv[1]);
  if (hp == NULL) {
    fprintf(stderr, "%s: %s non trouve dans in /etc/hosts ou dans le DNS\n", argv[0], argv[1]);
    exit(1);
  }

  memcpy( & serv_addr.sin_addr ,  hp->h_addr,  hp->h_length);
  printf ("IP address: %s\n", inet_ntoa (serv_addr.sin_addr));
 
   
 /*
 * Ouvrir socket UDP
 */
 if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
	 perror ("erreur socket");
	 exit (1);
 }

 /*ttl=1;
 if (setsockopt(serverSocket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) <0){
	 perror ("setsockopt");
	 exit (1);
 }*/

 data="PING";
 n=sendto(serverSocket,(void *) data, strlen(data), 0, (struct sockaddr *)&serv_addr, len);

 //set_timeout(serverSocket,5);
 n=recvfrom(serverSocket, (void *) sendbuf, sizeof(sendbuf), 0, (struct sockaddr  *)&serv_addr, &len);
 sendbuf[n]='\0';
 printf("%s\n",sendbuf);

}