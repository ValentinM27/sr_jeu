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

// Définitions du protocole de communication
/* Permet de demander le nom du joueur */
#define ASK_NAME "[ASK_NAME]"

/* Permet d'envoyer une liste de carte */
#define CARD_ARRAY "[CARD_ARRAY]"

/* Permet de signifier la fin de l'envoie */
#define END_CARD_ARRAY "[END_CARD_ARRAY]"

/* Permet signifier une bonne reception par le client */
#define RECEIVED "RECEIVED"

// Paramètres du serveur
#define server_PORT 8080
#define server_IP "127.0.0.1"

// Contenu pour le client
#define ask_player "C'est votre tour !"
#define game_start "-- Début de la partie --"

/* Déclaration des prototypes des fonctions */

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
		printf("Il faut encore %d joueurs \n", nb_client_max - nb_client_connected);
		newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);

		if(newSocket < 0) {
			printf("[NOK] - Erreur de connexion d'un client \n");
			return -1;
		}

		printf("Connexion d'un nouveau joueur \n");
		clients_connected[nb_client_connected] = newSocket;
		nb_client_connected ++;
	}

	// Reception des messages du client
	initGame(nb_client_connected);

	while(1) {
		if(currentRound == 0) {
			for(int i = 0; i < nb_client_connected; i++) {
				// Demande du psœudo du joueur
				send(clients_connected[i], ASK_NAME, sizeof(ASK_NAME), 0);
				recv(clients_connected[i], buffer, 1024, 0);
				// Copie nom
				strcpy(players[i].name, buffer);

				//On prévient le joueur qu'il va recevoir ses cartes
				send(clients_connected[i], CARD_ARRAY, sizeof(CARD_ARRAY), 0);
				recv(clients_connected[i], buffer, sizeof(buffer), 0);

				if (strcmp(CARD_ARRAY, buffer) == 0) {
					// On envoie les cartes du joueur
					bool isEnd = false;
					int currentIndex = 0;

					// On envoit les cartes avec une valeur qui n'est pas à 0
					while (!isEnd) {
						if (players[i].playerCards[currentIndex].valeur == 0) {
							send(clients_connected[i], END_CARD_ARRAY, sizeof(END_CARD_ARRAY), 0);

							// On clear le buffer pour le joueur suivant
							bzero(buffer, sizeof(buffer));
							isEnd = true;
						} else {
							char temp[4];
							sprintf(temp, "%d", players[i].playerCards[currentIndex].valeur);

							send(clients_connected[i], temp, sizeof(temp), 0);
							recv(clients_connected[i], buffer, sizeof(buffer), 0);

							if(strcmp(RECEIVED, buffer) == 0)
								currentIndex ++;
						}
					}
				}
			}
			beginGame();
		} else {
			for(int i = 0; i < nb_client_connected; i++) {
				send(clients_connected[i], ask_player, sizeof(ask_player), 0);
				recv(clients_connected[i], buffer, 1024, 0);
				printf("[%s] : %s\n", players[i].name, buffer);

				// Diffussion à tout les joueurs des messages écrits
				for(int y = 0; y < nb_client_connected; y++) {
					send(clients_connected[y], buffer, sizeof(buffer), 0);
				}
			}
		}
		bzero(buffer, sizeof(buffer));
	}

    return 0;
}

