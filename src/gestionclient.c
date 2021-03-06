#include "common.h"
#include "gestionclient.h"

/* Lit les donnees d'une socket et l'insere dans une chaine de caractere
*/
int readmsg(int currentsocket, char *msg){
	int size = 0;
	
	memset(msg, 0, REQUESTSIZE);

	size=recv(currentsocket, msg, REQUESTSIZE, 0);

	printf("%s\n", msg);

	return size;
}

/* Convertis une request HTTP en un structure de donnees plus facile a exploiter
*/
void readmsgHTTP(char *msg, char **request){
	
	int i = 0,j = 0, k = 0;
	char buffer[REQUESTSIZE];
	
	memset(buffer, 0, REQUESTSIZE);
	
	printf("\n******HTTP Request******\n%s************************\n", msg);

	for (i = 0; i < REQUESTSIZE; i++){

		buffer[k] = msg[i];

		k++;

		if(msg[i] == '\n'){
			if (k == 1) buffer[k-1] = '\0';
			else buffer[k-2] = '\0';

			strcpy(request[j], buffer);		

			j++;
			memset(buffer, 0, REQUESTSIZE);
			k=0;
		}

	}

}

/* Obtient le type de request HTTP d'apres celle-ci
*/
void gettypemsg(char *header, char *type){
	int i = 0;
	
	while(header[i] != ' ' && i < REQUESTSIZE){
		type[i] = header[i];	
		i++;
	}

	type[i] = '\0';

}

//Renvoie une socket vers le nom de domaine pour le service demande
int creategetsocket(char *hostname, char *port){
	
	int error, getsocket = 0;
	struct addrinfo hint, *cursor, *res;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	if ((error = getaddrinfo(hostname, port, &hint, &res)) < 0){
		perror("Probleme getaddrinfo socketget");
		printf("%s\n", gai_strerror(error));
		exit(1);
	}
	
	cursor = res;

	if((getsocket = socket(cursor->ai_family, cursor->ai_socktype, cursor-> ai_protocol)) < 0){
		printf("Problème initialisation socketget\n");
		exit(1);
	}

	if(connect (getsocket, cursor->ai_addr, cursor->ai_addrlen)  < 0){
		printf("Problème connect pour la socketget\n");
		exit(1);
	}

	freeaddrinfo(res);

	return getsocket;
	
}

//Renvoie l'indice de la derniere socket ayant le meme hostname
int searchHostname(char *hostname, char **hostnames) {
	//Variable de retour de la fonction
	int rep = -1, i = 0;

	for (i = 0; i < FD_SETSIZE; i++) {
		if (strcmp(hostname, hostnames[i]) == 0) {
			rep = i;
		}
	}

	return rep;
}

//Renvoie l'indice de la derniere socket ayant le meme numero de port
int searchService(int service, int *services) {
	//Variable de retour de fonction
	int rep = -1,i = 0;

	for (i = 0; i < FD_SETSIZE; i ++) {
		if (services[i] == service) {
			rep = i;
		}
	}

	return rep;
}
