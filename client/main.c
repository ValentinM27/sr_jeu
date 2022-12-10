#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Paramètres de connexion
#define server_PORT 8080
#define server_IP "127.0.0.1"

// Import des fichiers du jeu
#include "gamefiles/game.h"

/* Permet d'envoyer une liste de carte */
#define ASK_NAME "[ASK_NAME]"
#define CARD_ARRAY "[CARD_ARRAY]"

#define ask_player "C'est votre tour !"

/**
 * @brief Client
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
	// Descripteur du socket et buffer
	int clientSocket, serverStream;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	// Création socket client
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(clientSocket < 0){
		printf("[NOK] - Erreur client \n");
		return -1;
	}

	// Allocation espace mémoire pour le socket
	memset(&serverAddr, '\0', sizeof(serverAddr));

	// Définition des paramètres serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(server_IP);

	// Connexion server
	serverStream = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(serverStream < 0){
		printf("[NOK] - Erreur de connexion au serveur \n");
		return -1;
	}
	printf("[OK] - Connecté au serveur \n");

	// I/O Client/Server
	while(1){
		// Tant que ce n'est pas son tour, le joueur doit attendre
		while(strcmp(buffer, ask_player) != 0) {
			recv(clientSocket, buffer, 1024, 0);

			// Demande de nom
			if(strcmp(ASK_NAME, buffer) == 0) {
				printf("Saisisez votre nom : ");
				scanf("%s", &buffer[0]);
				strcpy(you.name, buffer);
				send(clientSocket, buffer, strlen(buffer), 0);
			}
			// Reception des cartes
			else if(strcmp(CARD_ARRAY, buffer) == 0) {
				recv(clientSocket, buffer, 1024, 0);
				printf("Vos cartes sont : %s \n", buffer);
			}
			// Autres messages
			else {
				printf("[Server] : %s\n", buffer);
			}
		}

		printf("[%s] - Saisir un message : ", you.name);
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		recv(clientSocket, buffer, 1024, 0);
		printf("[Server] : %s\n", buffer);

		bzero(buffer, sizeof(buffer));
	}

	return 0;
}

