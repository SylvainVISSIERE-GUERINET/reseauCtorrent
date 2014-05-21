#include "ServerPublish.h"


usage(){
  printf("usage : srvecho\n");
}

int main(int argc,char* argv[]) {

	int servSocket,pid,dialogueSocket,n;
	struct sockaddr_in serv_addr, serv_addrChild, cli_addr;
	char rcvdata[BUFSIZ],rcvdataChild[BUFSIZ];
	char sendbuff[BUFSIZ];
	socklen_t len=sizeof(serv_addr);

	//on veut appele client <adresse> <port>
	if (argc != 1){
		usage();
		exit(1);
	} else {
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
			rcvdata[n]='\0';
			if(strcmp(rcvdata,"PING\0")==0)
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
					sendto(servSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
					//Client send info
					/*n=recvfrom(servSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,&len);
					rcvdataChild[n]='\0';
					//Server re-send info to user-check
					sendto(servSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);
					//Client send "INFO ACK"
					n=recvfrom(servSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,&len);
					rcvdataChild[n]='\0';
					//Server send PUBLISH ACK
					strcpy(rcvdataChild, "PUBLISH ACK");
					sendto(servSocket,(void *) rcvdataChild,sizeof(rcvdataChild),0,(struct sockaddr *)&cli_addr,len);*/


					close(dialogueSocket);
					exit(0);
				}	
			}
		}
	}
}