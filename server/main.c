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

/* Permet dez signifier l'envoie des cartes de la table*/
#define TABLE_CARD_ARRAY "[TABLE_CARD_ARRAY]"

/* Permet de signifier l'envoie du dernier index d'une ligne de la table */
#define TABLE_LAST_INDEX_OF_ROW "[TABLE_LAST_INDEX_OF_ROW]"

/* Permet de signifier la fin des cartes de la table */
#define TABLE_CARD_ARRAY_END "[TABLE_CARD_ARRAY_END]"

/* Permet de signaler le début du round aux joueurs */
#define NEW_ROUND "[NEW_ROUND]"

/* Permet de demander à un joueur de poser une carte */
#define ASK_FOR_PLAY "[ASK_FOR_PLAY]"

/* Permet de demander à piocher une carte */
#define DRAW "[DRAW]"

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
		printf("Nombre de joueurs (min 2, max 6): ");
		scanf("%d", &nb_client_max);

		if(nb_client_max >= 2 && nb_client_max <= 6) okNbClient = 0;
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
	serverAddr.sin_addr.s_addr = INADDR_ANY;

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
			// On signal le round en cours aux joueur
			for (int i = 0; i < nb_client_connected; i++) {
				send(clients_connected[i], NEW_ROUND, sizeof(NEW_ROUND), 0);
				recv(clients_connected[i], buffer, sizeof(buffer), 0);

				if (strcmp(NEW_ROUND, buffer) == 0) {
					char temp[4];
					sprintf(temp, "%d", currentRound);

					send(clients_connected[i], temp, sizeof(temp), 0);
				}
			}

			// On envoie la table au joueur puis on lui demande de poser une carte
			for (int i = 0; i < nb_client_connected; i++) {
				// On prévient le joueur de l'envoie des cartes présentes sur la table
				send(clients_connected[i], TABLE_CARD_ARRAY, sizeof(TABLE_CARD_ARRAY), 0);
				recv(clients_connected[i], buffer, sizeof(buffer), 0);

				// On procède à l'envoie de carte
				if(strcmp(buffer, TABLE_CARD_ARRAY) == 0) {
					for (int currentRow = 0; currentRow < 4; currentRow ++) {
						send(clients_connected[i], TABLE_LAST_INDEX_OF_ROW, sizeof(TABLE_LAST_INDEX_OF_ROW), 0);
						recv(clients_connected[i], buffer ,sizeof(buffer), 0);

						if(strcmp(buffer, TABLE_LAST_INDEX_OF_ROW) == 0) {
							char indexToChar[4];
							sprintf(indexToChar, "%d", table[currentRow].currentLastIndex);

							send(clients_connected[i], indexToChar, sizeof(indexToChar), 0);
							recv(clients_connected[i], buffer, sizeof(buffer), 0);

							// Envoie de la ligne au client
							for(int currentCard = 0; currentCard <= table[currentRow].currentLastIndex; currentCard ++) {
								char valeurToChar[4];
								sprintf(valeurToChar, "%d", table[currentRow].row[currentCard].valeur);

								send(clients_connected[i], valeurToChar, sizeof(valeurToChar), 0);
								// Attente de la réponse du client avant le prochain envoi
								recv(clients_connected[i], buffer, sizeof(buffer), 0);
							}
						}
					}

				}

				// On signifie la fin de l'envoie des cartes de la table au client
				send(clients_connected[i], TABLE_CARD_ARRAY_END, sizeof(TABLE_CARD_ARRAY_END), 0);

				// On demande de poser une carte au joueur
				send(clients_connected[i], ASK_FOR_PLAY, sizeof(ASK_FOR_PLAY), 0);

				// Le joueur nous envoi l'index de la carte qu'il souhaite jouer
				recv(clients_connected[i], buffer, sizeof(buffer), 0);
				bool ok = putCardOnTable(players[i].playerCards[atoi(buffer)], i);

				if(ok) {
					send(clients_connected[i], RECEIVED, sizeof(RECEIVED), 0);
				}
			}

			// Une fois que tout le monde à joué on fini le round
			endRound();
			// On regarde si un joueur à gagné ...

			// On clear le buffer
			bzero(buffer, sizeof(buffer));
		}
		bzero(buffer, sizeof(buffer));
	}

    return 0;
}

