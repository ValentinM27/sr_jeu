#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

// Paramètres de connexion
#define server_PORT 8080
#define default_server_IP "127.0.0.1"

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

/* Permet à un joueur de dire s'il peut jouer */
#define CAN_PLAY "[CAN_PLAY]"

/* Permet à un joueur de dire qu'il ne peut pas jouer */
#define CANT_PLAY "[CANT_PLAY]"

/* Permet de demander à piocher une carte */
#define DRAW "[DRAW]"

/* Permet de signifier la fin de la partie */
#define END_GAME "[END_GAME]"

/* Permet de dire aux joueurs que la partie continue */
#define CONTINUE_GAME "[CONTINUE_GAME]"

/* Permet signifier une bonne reception par le client */
#define RECEIVED "RECEIVED"

/* Délai d'attentes de réponde des bots */
#define TIME 1

#define ask_player "C'est votre tour !"

/**
 * @brief bot
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
	// Liste des noms
	char liste_name[10][12] = {"Bill Gates", "Mark Zuck", "Dijkstra", "Abdenour", "Bouzouane", "XAE-12", "Eric", "Mimonnet", "Jeff Bezos", "McGregor"};

	// Descripteur du socket et buffer
	int botSocket, serverStream;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	// Création socket bot
	botSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(botSocket < 0){
		printf("[NOK] - Erreur bot \n");
		return -1;
	}

	// Allocation espace mémoire pour le socket
	memset(&serverAddr, '\0', sizeof(serverAddr));

	// Définition des paramètres serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(default_server_IP);

	// Connexion server
	serverStream = connect(botSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(serverStream < 0) {
		printf("[NOK] - Erreur de connexion au serveur \n");
	}
	printf("[OK] - Connecté au serveur \n");

	// I/O Bot/Server
	while(1){
		printf("[Attente serveur] \n");
		recv(botSocket, buffer, sizeof(buffer), 0);

		// Demande du nom
		if(strcmp(buffer, ASK_NAME) == 0) {
			srand(time(NULL));
			int randomIndexName = rand() % 10;
			strcpy(you.name, liste_name[randomIndexName]);
			sleep(TIME);
			send(botSocket, you.name, sizeof(you.name), 0);
			printf("[nom envoyé] \t");
		}
		// Receptions de cartes
		else if(strcmp(CARD_ARRAY, buffer) == 0) {
			send(botSocket, CARD_ARRAY, sizeof(CARD_ARRAY), 0);

			bool isEnd = false;
			int currentIndex = 0;

			while(!isEnd) {
				recv(botSocket, buffer, sizeof(buffer), 0);

				if(strcmp(buffer, END_CARD_ARRAY) == 0) {
					isEnd = true;
					printBotCard();
				} else {
					// Convertion en int
					int valeur = atoi(buffer);

					// On créer la carte
					you.playerCards[currentIndex] = createCard(valeur);
					currentIndex ++;

					// Confirmation de la reception au serveur
					send(botSocket, RECEIVED, sizeof(RECEIVED), 0);
				}
			}
		}
		// Début du round
		else if(strcmp(buffer, NEW_ROUND) == 0) {
			send(botSocket, NEW_ROUND, sizeof(NEW_ROUND), 0);
			recv(botSocket, buffer, sizeof(buffer), 0);
			currentRound = atoi(buffer);

			printf("\t ---------------------- \n");
			printf("\t -- Début du tour %d -- \n", currentRound);
			printf("\t ---------------------- \n");

			// Affichage des cartes du joueur
			printBotCard();
		}
		// Reception de l'état de la table
		else if(strcmp(buffer, TABLE_CARD_ARRAY) == 0) {
			send(botSocket, TABLE_CARD_ARRAY, sizeof(TABLE_CARD_ARRAY), 0);

			// Reception ligne par ligne
			for (int currentRow = 0; currentRow < 4; currentRow ++) {
				recv(botSocket, buffer, sizeof(buffer), 0);
				send(botSocket, TABLE_LAST_INDEX_OF_ROW, sizeof(TABLE_LAST_INDEX_OF_ROW), 0);

				// Reception dernier index de la ligne
				recv(botSocket, buffer, sizeof(buffer), 0);
				table[currentRow].currentLastIndex = atoi(buffer);
				send(botSocket, RECEIVED, sizeof(RECEIVED), 0);

				// Construction de la ligne
				for(int currentCard = 0; currentCard <= table[currentRow].currentLastIndex; currentCard ++) {
					recv(botSocket, buffer, sizeof(buffer), 0);
					table[currentRow].row[currentCard] = createCard(atoi(buffer));
					send(botSocket, RECEIVED, sizeof(RECEIVED), 0);
				}
			}

			recv(botSocket, buffer, sizeof(buffer), 0);
			if(strcmp(buffer, TABLE_CARD_ARRAY_END) == 0)
				printTable();
		}
		// Poser carte
		else if (strcmp(buffer, ASK_FOR_PLAY) == 0) {
			// Si le bot peut poser une carte
			if(canPlay()) {
				send(botSocket, CAN_PLAY, sizeof(CAN_PLAY), 0);
				recv(botSocket, buffer, sizeof(buffer), 0);

				if(strcmp(buffer, RECEIVED) == 0) {
					// Index de la meilleure carte à jouer
					int indexOfCard = searchBestCardToPlay();

					char indexOfCardToChar[4];
					sprintf(indexOfCardToChar, "%d", indexOfCard);
					sleep(TIME);
					send(botSocket, indexOfCardToChar, sizeof(indexOfCardToChar), 0);

					recv(botSocket, buffer, sizeof(buffer), 0);

					if(strcmp(buffer, RECEIVED) == 0) {
						// On supprime la carte de la main du bot
						deleteCardFromHand(you.playerCards[indexOfCard]);
					}
				}
			}
			// Sinon le bot doit selectionner une ligne à ramasser
			else {
				// On signale que l'on peut pas jouer au serveur
				send(botSocket, CANT_PLAY, sizeof(CANT_PLAY), 0);
				recv(botSocket, buffer, sizeof(buffer), 0);

				if(strcmp(buffer, DRAW) == 0) {
					printf("\t - Le bot ne peut pas jouer - \n");

					int choice= findBestLineToDraw();

					char choiceToChar[4];
					sprintf(choiceToChar, "%d", choice-1);

					sleep(TIME);
					send(botSocket, choiceToChar, sizeof(choiceToChar), 0);
					recv(botSocket, buffer, sizeof(buffer), 0);

					if(strcmp(buffer, RECEIVED) == 0)
						takeLigne(choice-1);

					printBotCard();
				}
			}
		}
		// Gestion de la fin du round
		else if (strcmp(buffer, CONTINUE_GAME) == 0) {
			printf("-- fin du round -- \n");
			printBotScore();

			send(botSocket, RECEIVED, sizeof(RECEIVED), 0);
		}
		// Gestion fin de partie
		else if (strcmp(buffer, END_GAME) == 0) {
			printf("\t *** fin de la partie ! *** \n");
			printBotScore();

			send(botSocket, RECEIVED, sizeof(RECEIVED), 0);

			// Reception du nom du vainqueur
			recv(botSocket, buffer, sizeof(buffer), 0);
			printf("************************************");
			printf("* Vainqueur de la partie : %s      *", buffer);
			printf("************************************");

			send(botSocket, RECEIVED, sizeof(RECEIVED), 0);

			// Fermeture de la connexion
			close(botSocket);
			exit(0);
		}
		// Autres messages
		else {
			printf("[Server] : %s\n", buffer);
		}
	}

	return 0;
}
