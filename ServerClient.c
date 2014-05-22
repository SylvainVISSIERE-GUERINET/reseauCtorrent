#include "ServerClient.h"

usage(){
  printf("usage : ServerClient\n");
}


int main (int argc,char *argv[])

{
  int sockfd, n, newsockfd, childpid, servlen,fin, nbread;
  struct sockaddr_in  serv_addr, cli_addr;
  socklen_t clilen;
//pour utiliser le select rajouter:
  int tab_clients[FD_SETSIZE];
  fd_set rset,pset;
  int maxfdp1,nbfd,i,sockcli;
  char rcvbuffer[BUFSIZ], path[BUFSIZ];

 /* Verifier le nombre de paramètre en entrée */
  if (argc != 1){
    usage();
    exit(1);}


/*
 * Ouvrir une socket (a TCP socket)
 */
if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) <0) {
   perror("ServerClient : Probleme socket\n");
   exit (2);
 }

 
/*
 * Lier l'adresse  locale à la socket
 */
 memset( (char*) &serv_addr,0, sizeof(serv_addr) );
 serv_addr.sin_family = PF_INET;
 serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
 serv_addr.sin_port = htons (PORT);
 
 
 if (bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr) ) <0) {
   perror ("ServerClient : erreur bind\n");
   exit (1);
 }

/* Paramètrer le nombre de connexion "pending" */
 if (listen(sockfd, SOMAXCONN) <0) {
   perror ("ServerClient : erreur listen\n");
   exit (1);
 }

 for (;;) {
   
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  &clilen);
   if (newsockfd < 0) {
    perror("ServerClient : erreur accept\n");
    exit (1);
   }


  if ( (childpid = fork() ) < 0) {
    perror ("ServerClient: fork error\n");
    exit (1);
  }
  else
    if (childpid == 0) {       
      close (sockfd);
      //Client send  nom et type du fichier a recup en un seul coup, client send nom.extension
      read(newsockfd,rcvbuffer, sizeof(rcvbuffer));
      //Server cherche
      printf("%s\n",rcvbuffer);

      sprintf(path,"Partage/%s",rcvbuffer);
      FILE* fd=fopen(path,"rb");

      if(fd==NULL) {
        perror("ServerClient : fichier  non trouver\n");
        exit(2);
      }
      
      //Server Envoie
      int n;
      nbread=fread(rcvbuffer, 1, sizeof(rcvbuffer), fd);
      printf("%s\n",rcvbuffer);
      n=write(newsockfd,rcvbuffer,nbread);
      while(nbread>0) {
        printf("coucou \n");
        printf("%s\n",rcvbuffer);
        n=send(newsockfd,rcvbuffer,nbread,0);
        nbread=fread(rcvbuffer, 1, sizeof(rcvbuffer), fd);
      }

      //Server close
      close(newsockfd);
      //traiter  requete
      exit (0); 
    }

  close(newsockfd);  
 }
}