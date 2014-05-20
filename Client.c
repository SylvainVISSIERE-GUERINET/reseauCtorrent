#include "Client.h"

usage(){
  printf("usage : cliecho adresseIP_serveur(x.x.x.x)  numero_port_serveur\n");
}

//fonction qui fera la requete afin de récupérer la liste des clients qui possède des fichiers correspondant aux mots clé
void search(char* motsCles[])  {

}


void publish(char* nomFichier, char* motsCles[]) {

}

int main(int argc,char* argv[]) {

	//on veut appele client <adresse> <port>
	if (argc != 3){
		usage();
		exit(1);
	}

}