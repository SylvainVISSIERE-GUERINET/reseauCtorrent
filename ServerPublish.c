#include "ServerPublish.h"


usage(){
  printf("usage : srvecho\n");
}

int main(int argc,char* argv[]) {

	int servSocket,pid,dialogueSocket,n,nbClef;
	struct sockaddr_in serv_addr, serv_addrChild, cli_addr;
	char rcvdata[BUFSIZ],rcvdataChild[BUFSIZ];
	char sendbuff[BUFSIZ],info[BUFSIZ], infoAsplit[BUFSIZ];
	char nom[BUFSIZ], type[BUFSIZ], liste[BUFSIZ], sha[BUFSIZ], ip[BUFSIZ],path[BUFSIZ],cliIP[15];
	char* test[5];
	char* error, *saveptr,token;
	char** tablMotsClefs;
	socklen_t len=sizeof(serv_addr);
	FILE* fd;

	//on veut appele client <adresse> <port>
	if (argc != 1){
		usage();
		exit(1);
	} else {

		if(opendir("data") == NULL) {
    		system("mkdir data");
    	} 

		if ((servSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
		   perror("servPublish : Probleme socket serv\n");
		   exit (2);
 		}

		/*
		 * Lier l'adresse  locale à la socket
		 */
		memset( (char*) &serv_addr,0, sizeof(serv_addr) );
		serv_addr.sin_family = PF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(PORT);

		memset(&cli_addr,0,sizeof(cli_addr));

		if(bind(servSocket,(struct sockaddr *) &serv_addr,sizeof(serv_addr) )<0) {
			printf("erreur serveur: erreur bind!\n");
			exit(1);
		}

		while(1) {
			//le client send un PING et ensuite on commence a causer dans le fils.
			n=recvfrom(servSocket,(void *) rcvdata,sizeof(rcvdata),0,(struct sockaddr *)&cli_addr,&len);
			// gaffe "\0" correspond a 2 carac alors que '\0' correspond au carac  de fin de chaine
			//rcvdata[n]='\0';
			//strcat(rcvdata, ".txt");
			printf("%s\n",rcvdata);
			if(strcmp(rcvdata,"PING")==0)
			{
				printf("PING RECU! \n");
				pid=fork();
				if (pid<0)
				{
					perror("servPublish : erreur fork\n");
					exit(1);
				}

				if(pid==0) 
				{ //je suis un enfant
					close(servSocket);
					if ((dialogueSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) 
					{
						   perror("servPublish : Probleme creation socket fils\n");
						   exit (2);
			 		}

			 		memset( (char*) &serv_addrChild,0, sizeof(serv_addrChild) );
					serv_addrChild.sin_family = PF_INET;
					serv_addrChild.sin_addr.s_addr = htonl (INADDR_ANY);
					serv_addrChild.sin_port = htons(0);

					if(bind(dialogueSocket,(struct sockaddr *) &serv_addrChild,sizeof(serv_addrChild))<0)
					{
						perror("servPublish : Probleme bind fils\n");
						exit(2);
					}
					strcpy(rcvdataChild,"PING ACK");
					//Server send "PING ACK"
					sendto(dialogueSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
					//Client send nbClef
					
					while(strcmp(rcvdataChild, "INFO ACK")) {
						//Client send info
						n=recvfrom(dialogueSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,&len);
						printf("rcvdataChild: %s\n", rcvdataChild);
						strcpy(info, rcvdataChild);
						strcpy(cliIP,inet_ntoa(cli_addr.sin_addr));
						printf("%s\n",cliIP);
						strcat(info,"|");
						strcat(info, cliIP);
						//Server re-send info to user-check
						sendto(dialogueSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
						//Client send "INFO ACK"
						recvfrom(dialogueSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,&len);
					}
					printf("info: %s\n", info);
					//sscanf(info, "%s|%s|%s|%s|%s", nom, type, liste, sha, ip);
					strcpy(infoAsplit,info);
					strcpy(nom,strtok(info,"|"));
					strcpy(type,strtok(NULL, "|"));
					strcpy(liste,strtok(NULL, "|"));
					strcpy(sha,strtok(NULL, "|"));
					strcpy(ip,strtok(NULL, "|"));

					tablMotsClefs=com_split(liste, ';', &nbClef);
					int j;
					for(j=0;j<nbClef;j++){
						sprintf(path, "data/%s.txt", tablMotsClefs[j]);
						if ((fd=fopen(path, "a"))==NULL) {
							sprintf(error, "ServerPublish : erreur ouverture fichier %s \n", path);
							perror("ServerPublish : erreur ouverture fichier \n");
							exit(2);
						}
						fprintf(fd, "%s\n",infoAsplit);
						fclose(fd);
					}

					//Server send PUBLISH ACK
					strcpy(rcvdataChild, "PUBLISH ACK");
					sendto(dialogueSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
					printf("Publish ACK sended\n");

					close(dialogueSocket);
					exit(0);
				}	
			}
		}
	}
}