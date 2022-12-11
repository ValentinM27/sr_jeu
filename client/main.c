#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

// Paramètres de connexion
#define server_PORT 8080
#define server_IP "127.0.0.1"

// Import des fichiers du jeu
#include "gamefiles/game.h"

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
			recv(clientSocket, buffer, sizeof(buffer), 0);

			// Demande de nom
			if(strcmp(ASK_NAME, buffer) == 0) {
				printf("Saisisez votre nom : ");
				scanf("%s", &buffer[0]);
				strcpy(you.name, buffer);
				send(clientSocket, buffer, strlen(buffer), 0);
			}
			// Reception des cartes
			else if(strcmp(CARD_ARRAY, buffer) == 0) {
				send(clientSocket, CARD_ARRAY, sizeof(CARD_ARRAY), 0);

				bool isEnd = false;
				int currentIndex = 0;

				while(!isEnd) {
					recv(clientSocket, buffer, sizeof(buffer), 0);

					if (strcmp(buffer, END_CARD_ARRAY) == 0) {
						isEnd = true;
						printPlayerCard();
					} else {
						// Convertiuon en int
						int valeur = atoi(buffer);

						// On créer la carte
						you.playerCards[currentIndex] = createCard(valeur);
						currentIndex ++;

						// Confirme la reception au serveur
						send(clientSocket, RECEIVED, sizeof(RECEIVED), 0);
					}
				}
			}
			// Début de round
			else if(strcmp(buffer, NEW_ROUND) == 0) {
				send(clientSocket, NEW_ROUND, sizeof(NEW_ROUND), 0);
				recv(clientSocket, buffer, sizeof(buffer), 0);
				currentRound = atoi(buffer);

				printf("\t ---------------------- \n");
				printf("\t -- Début du tour %d -- \n", currentRound);
				printf("\t ---------------------- \n");

				// Affichage des cartes du joueur
				printPlayerCard();
			}
			// Reception de l'état de la table
			else if(strcmp(buffer, TABLE_CARD_ARRAY) == 0) {
				send(clientSocket, TABLE_CARD_ARRAY, sizeof(TABLE_CARD_ARRAY), 0);

				// Reception ligne par ligne
				for (int currentRow = 0; currentRow < 4; currentRow ++) {
					recv(clientSocket, buffer, sizeof(buffer), 0);
					send(clientSocket, TABLE_LAST_INDEX_OF_ROW, sizeof(TABLE_LAST_INDEX_OF_ROW), 0);

					// Reception dernier index de la ligne
					recv(clientSocket, buffer, sizeof(buffer), 0);
					table[currentRow].currentLastIndex = atoi(buffer);
					send(clientSocket, RECEIVED, sizeof(RECEIVED), 0);

					// Construction de la ligne
					for(int currentCard = 0; currentCard <= table[currentRow].currentLastIndex; currentCard ++) {
						recv(clientSocket, buffer, sizeof(buffer), 0);
						table[currentRow].row[currentCard] = createCard(atoi(buffer));
						send(clientSocket, RECEIVED, sizeof(RECEIVED), 0);
					}
				}

				recv(clientSocket, buffer, sizeof(buffer), 0);
				if(strcmp(buffer, TABLE_CARD_ARRAY_END) == 0)
					printTable();
			}
			// Poser carte
			else if (strcmp(buffer, ASK_FOR_PLAY) == 0) {
				int indexOfCard;
				printf("Quelle carte voulez-vous poser ? ");
				scanf("%d", &indexOfCard);

				char indexOfCardToChar[4];
				sprintf(indexOfCardToChar, "%d", indexOfCard);
				send(clientSocket, indexOfCardToChar, sizeof(indexOfCardToChar), 0);

				recv(clientSocket, buffer, sizeof(buffer), 0);

				if(strcmp(buffer, RECEIVED) == 0) {
					// On supprime la carte de la main du joueur
					deleteCardFromHand(you.playerCards[indexOfCard]);
				}
			}
			// Autres messages
			else {
				printf("[Server] : %s\n", buffer);
			}
		}

		bzero(buffer, sizeof(buffer));
	}

	return 0;
}

