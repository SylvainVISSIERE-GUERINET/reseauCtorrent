#include "ServerSearch.h"

void usage()
{
	printf("Usage : nombre d'arguments invalide.");
}

int main(int argc, char* argv[]){
	int servSocket, pid, dialogSocket, n;
	struct sockaddr_in serv_addr, serv_addrChild, cli_addr;
	char rcvdata[BUFSIZ],rcvdataChild[BUFSIZ];
	char sendbuff[BUFSIZ];
	socklen_t len=sizeof(serv_addr);

	// on verifie le nombre d arguments
	if (argc != 1)
	{
		usage();
		exit(1);
	}
	// si on a le bon nombre d'arguments
	else
	{
		if ((servSocket = socket(PF_INET, SOCK_DGRAM, 0))<0)
		{
			perror("ServerSearch : probleme de la socket du serveur.\n");
			exit(2);
		}

		// lier l'adresse locale à la socket
		memset((char*) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = PF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(PORT);

		memset(&cli_addr,0,sizeof(cli_addr));

		// bind : ceci est mon adresse et tous les messages recus sur cette adresse doivent m'etre delivres
		if (bind(servSocket,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		{
			printf("ServerSearch : erreur bind");
			exit(1);
		}

		// tant qu'on ne coupe pas le serveur
		while(1)
		{
			// on recupere un message de client qu'on stocke dans rcvdata
			n = recvfrom(servSocket,(void *) rcvdata, sizeof(rcvdata),0,(struct sockaddr *)&cli_addr, &len);
			rcvdata[n] = '\0';
			// la premiere chose que sera le client est de demander le ping du serveur pour savoir s'il existe. On teste alors le message recu

			// s'il s'agit bien de la demande de PING
			if (strcmp(rcvdata, "PING\0")==0)
			{
				printf("ServerSearch : PING RECU! \n");
				// on fait un fork : le fils s'occupera du client, le pere continuera d'attendre
				pid = fork();
				if (pid<0)
				{
					perror("ServerSearch : erreur fork.");
					exit(1);
				}

				// si on est l'enfant
				else if (pid == 0)
				{
					// on ferme la socket du serveur
					close(servSocket);
					// et on ouvre une socket de dialogue
					dialogSocket = socket(PF_INET, SOCK_DGRAM,0);
					// si il y a une erreur dans l ouverture de la socket
					if (dialogSocket<0)
					{
						perror("ServerSearch : probleme de creation de socket de dialogue dans le fils.");
						exit(2);
					}

					// on lie l'adresse locale à la socket
					memset((char*) &serv_addrChild,0,sizeof(serv_addrChild));
					serv_addrChild.sin_family = PF_INET;
					serv_addrChild.sin_addr.s_addr = htonl(INADDR_ANY);
					serv_addrChild.sin_port = htons(0);

					// bind
					if(bind(dialogSocket,(struct sockaddr *) &serv_addrChild,sizeof(serv_addrChild))<0)
					{
						perror("ServerSearch : probleme de bind du fils.");
						exit(2);
					}

					// on envoie le ACK du PING
					strcpy(rcvdataChild, "PING ACK");
					sendto(dialogSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);

					// le client envoie un mot clef des fichiers // requete SEARCH
					n = recvfrom(dialogSocket,(void *) rcvdataChild, sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr, &len);
					//rcvdataChild[n] = '\0';

					printf("ServerSearch - mot clef recu : %s \n", rcvdataChild);
					// rechercher la liste des sources (pairs) disposant des fichiers correspondant au mot clef
					char * nomFichier;
					nomFichier = strcat(rcvdataChild,".txt");
					// on ouvre le fichier du nom de rcvdataChild.txt
					FILE *fichier = NULL;
					fichier = fopen(nomFichier,"r");
					// s'il n'existe pas
					if (fichier == NULL)
					{
						printf("Nous n'avons pas trouve le fichier correspondant a ce mot clef.");
						// on retourne qu'aucun fichier ne correspond à ce mot clef
						strcpy(rcvdataChild, "Aucun fichier ne correspond a votre mot clef.");
						sendto(dialogSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
						// on ferme la socket de dialogue
						close(dialogSocket);
						exit(0);
						// retour au choix du debut : publish ou search, bisous Sylvain
					}
					else
					{
						// on repond avec le resultat de la recherche : nom type du fichier, liste des autres mots clefs, hash du contenu, @IPsource
						// on recupere dans le fichier
						char dansFichier[BUFSIZ];
						int nbLignes = 0;
						int timeoutOuNon = -1; // on l'initialise a -1 afin de rentrer dans la boucle
						// on boucle sur le nombre de fichiers concernes par le mot clef
						while (fgets(dansFichier, BUFSIZ, fichier)!=NULL)
						{
							timeoutOuNon = -1;
							nbLignes = nbLignes +1;
							// on fait une boucle dans laquelle on envoie le fichier toutes les 5 secondes tant qu'on a pas recu de ACKFICHIER
							while(timeoutOuNon == -1)
							{
								// la chaine de caracteres est envoyee au client, qui la parsera
								strcpy(rcvdataChild,dansFichier);
								sendto(dialogSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
								printf("Envoi du fichier%d\n", nbLignes);

								// setTimeOut : uniquement sur les rcvd
								// si timeout expiré : zappe le rcvd
								// il faut donc checker apres le rcvd si il a été débloqué par une réception ou par un timeout
								set_timeout(dialogSocket, 5);
								// on attend a chaque fois de recevoir un "ACKFICHIER" de la part du client avant de lancer un nouveau fichier
								timeoutOuNon=recvfrom(dialogSocket,(void *) rcvdata, sizeof(rcvdata),0,(struct sockaddr *)&cli_addr, &len);
								rcvdata[n] = '\0';
								
								if (timeoutOuNon == -1)
								{
									printf("ServerSearch : Timeout du fichier%d : %d\n",nbLignes, n);
								}

								if (strcmp(rcvdata,"ACKFICHIER\n"))
								{
									printf("ServerSearch : Erreur dans la reception du ACKFICHIER");
									exit(2);
								}
							}
						}
						// a la fin, on indiquera au client que les fichiers ont tous ete envoyes
						strcpy(rcvdataChild,"Fin fichier");
						sendto(dialogSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
						printf("%d fichiers ont ete envoyes.", nbLignes);

						// on attend une reponse du client qui nous retourne le nombre de lignes avant de fermer la socket -> TODO si on a le temps			

						// on ferme la socket de dialogue
						close(dialogSocket);
						exit(0);
					}
				}
			}		
		}
	}
}
