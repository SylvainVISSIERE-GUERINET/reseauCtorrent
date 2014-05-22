usage(){
  printf("usage : ServerClient numero_port_serveur\n");
}


int main (int argc,char *argv[])

{
  int sockfd, n, newsockfd, childpid, servlen,fin;
  struct sockaddr_in  serv_addr, cli_addr;
  socklen_t clilen;
//pour utiliser le select rajouter:
  int tab_clients[FD_SETSIZE];
  fd_set rset,pset;
  int maxfdp1,nbfd,i,sockcli;

 /* Verifier le nombre de paramètre en entrée */
  if (argc != 2){
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
 serv_addr.sin_port = htons(atoi(argv[1]));
 
 
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

      //Server cherche

      //Server Envoie

      //Server close

      //traiter  requete
      exit (0); 
    }

  close(newsockfd);  
 }