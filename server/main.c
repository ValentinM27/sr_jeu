#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Import des fichiers du jeu
#include "gamefiles/game.h"

// Paramètres du serveur
#define server_PORT 8080
#define server_IP "127.0.0.1"

#define ask_player "C'est votre tour !"

/**
 * @brief Serveur
 *
 * @return int
 */
int main(void)
{
	// Descripteur du serveur
	int serverSocket, inputStream;
	int nb_client_connected = 0;
	struct sockaddr_in serverAddr;

	// Structure de stockage des clients
	int nb_client_max;
	int okNbClient = -1;

	while (okNbClient == -1) {
		printf("Nombre de joueurs (max 6): ");
		scanf("%d", &nb_client_max);

		if(nb_client_max <= 6) okNbClient = 0;
	}

	int clients_connected[nb_client_max];

	// Gestion des connexions multiples
	socklen_t addr_size;

	int newSocket;
	struct sockaddr_in newAddr;

	char buffer[1024];
	pid_t childpid;

	// Création du scoket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0) {
		printf("[NOK] - Erreur de création du socket \n");
		return -1;
	}

	// Allocation espace mémoire
	memset(&serverAddr, '\0', sizeof(serverAddr));

	// Définition des paramètres serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(server_IP);

	// Écoute du port
	inputStream = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(inputStream < 0){
		printf("[NOK] - Erreur d'écoute du port \n");
		return -1;
	}

	if(listen(serverSocket, 10) == 0){
		printf("[Server] - En attente de flux \n");
	} else {
		printf("[NOK] - Erreur d'eçoute du port \n");
		return -1;
	}

	// Gestion des nouveaux clients
	while(nb_client_connected < nb_client_max){
		newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);

		if(newSocket < 0) {
			printf("[NOK] - Erreur de connexion d'un client \n");
			return -1;
		}

		printf("Connexion d'un nouveau joueur \n");
		clients_connected[nb_client_connected] = newSocket;
		nb_client_connected ++;
	}

	// Initialisation de la partie
	printf("----- Début de la partie ----- \n");
	initGame(nb_client_connected);
	printTable();

	// Carte dans la mains de J1
	printf("--- Carte de J1 --- \n");
	printPlayerCards(0);
	
	updatePlayerScore(nb_client_connected);
	printf("\t -- Score : %d \n", players[0].score);


	// Test d'ajout d'une carte sur la table
	printf("----- J1 pose une carte sur la table ----- \n");
	putCardOnTable(players[0].playerCards[2], 0);
	printTable();
	
	// Carte dans la mains de J1
	printf("--- Carte de J1 --- \n");
	printPlayerCards(0);
	
	updatePlayerScore(nb_client_connected);
	printf("\t -- Score : %d \n", players[0].score);

	// Reception des messages du client
	while(1) {
		for(int i = 0; i < nb_client_connected; i++) {
			send(clients_connected[i], ask_player, sizeof(ask_player), 0);
			recv(clients_connected[i], buffer, 1024, 0);
			printf("[Client] : %s\n", buffer);

			// Diffussion à tout les joueurs des messages écrits
			for(int y = 0; y < nb_client_connected; y++) {
				send(clients_connected[y], buffer, sizeof(buffer), 0);
			}

			bzero(buffer, sizeof(buffer));
		}
	}

    return 0;
}

