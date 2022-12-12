#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// En-tête
#include "game.h"

// Définition des variables
// Variables du jeu
int currentRound;
int currentCardInCards;
CARD cards[NB_CARD];
int nbPlayers;

// Table de jeu
TABLEROW table[4];

// Tableau des joueurs
PLAYER players[6];

/**
 * Permet d'initialiser les valeurs des cartes
 */
void createCards()
{
	for(int i=0; i < NB_CARD; i++)
	{
		CARD tempCard;
		tempCard.valeur = i+1;
		tempCard.cattleHead = 1;

		if(i+1 == 55) {
			tempCard.cattleHead = 7;
		} else if((i+1)%11 == 0) {
			tempCard.cattleHead = 5;
		} else if((i+1)%10 == 0) {
			tempCard.cattleHead = 3;
		} else if((i+1)%5 == 0) {
			tempCard.cattleHead = 2;
		}

		cards[i] = tempCard;
		currentCardInCards++;
	}
}

/**
 * Permet de créer un paquet de carte pour un joueur
 */
CARD pickRandomCard()
{
	srand(time(NULL));

	int randIndex = rand()%(currentCardInCards);
	CARD randomCard = cards[randIndex];

	// On insert la dernière carte du paquet à la place de la carte tirée
	cards[randIndex] = cards[currentCardInCards];
	currentCardInCards--;

	return randomCard;
}

/**
 * Permet de créer les paquets de dápart des joueurs
 */
void initPackages()
{
	for(int i = 0; i < 10; i++)
	{
		for(int y = 0; y < nbPlayers; y++)
			players[y].playerCards[i] = pickRandomCard();
	}
}

/**
 * Tirage au sort ordonné des carte de départ
 */
void initTable()
{
	// Stockage des cartes tirées
	CARD randomCard[4];

	// Tirage des cartes
	for (int i = 0; i < 4; i++) {
		randomCard[i] = pickRandomCard();
	}

	// Trie des cartes
	for (int i = 0; i < 4; i++) {
		for (int y = 0; y < 4; y++) {
			if (randomCard[i].valeur > randomCard[y].valeur) {
				CARD temp_a = randomCard[i];
				randomCard[i] = randomCard[y];
				randomCard[y] = temp_a;
			}
		}
	}

	// Création de la table de jeu
	for (int i = 0; i < 4; i++) {
		table[i].row[0] = randomCard[i];
		table[i].currentLastIndex = 0;
	}
}

/**
 * Permet d'initialiser le jeu
 */
void initGame(int nbConnectedPlayers)
{
	/* initialisation du tour en cours */
	currentRound = 0;

	/* Nombre réel de joueurs */
	nbPlayers = nbConnectedPlayers;

	/* Initiatisation du paquet */
	createCards();

	/* Création des mains des joueurs */
	initPackages();

	/* Tirage des cartes de départ */
	initTable();
}

/**
 * Affichage de la table de jeu
 */
void printTable()
{
	for (int i = 0; i < 4; i++) {

		// Affichage de la première carte de la ligne
		printf(" [Ligne %d] \t", i+1);

		// Affichage du reste de la liste
		for(int currentIndex = 0; currentIndex <= table[i].currentLastIndex; currentIndex++) {
				printf("[%d|%d]", table[i].row[currentIndex].valeur, table[i].row[currentIndex].cattleHead);

				if (currentIndex == table[i].currentLastIndex)
					printf("\n");
		}
	}
}

/**
 * Permet de placer une carte sur la table
 */
bool putCardOnTable(CARD cardToPlace, int playerIndex)
{
	bool cardPlaced = false;

	for (int i = 0; i < 4; i++) {
		if (table[i].row[table[i].currentLastIndex].valeur < cardToPlace.valeur) {
			// On insert la carte dans la ligne
			table[i].row[table[i].currentLastIndex+1].valeur = cardToPlace.valeur;
			table[i].row[table[i].currentLastIndex+1].cattleHead = cardToPlace.cattleHead;
			table[i].currentLastIndex++;

			// On retire la carte du paquet du joueur
			deleteCardFromPlayersCards(cardToPlace, playerIndex);

			// On arrête la recherche
			cardPlaced = true;
			break;
		}
	}

	return cardPlaced;
}

/**
 * Permet de supprimer une carte du paquet d'un joueur
 */
void deleteCardFromPlayersCards(CARD cardToDelete, int playerIndex)
{
	int indexOfTheDeletedCard;

	for (int i = 0; i < NB_CARD; i++) {
		if (
			players[playerIndex].playerCards[i].valeur == cardToDelete.valeur &&
			players[playerIndex].playerCards[i].cattleHead == cardToDelete.cattleHead
		) {
			indexOfTheDeletedCard = i;
			break;
		}
	}

	for (int i = indexOfTheDeletedCard; i < NB_CARD; i++) {

		// Si on arrive à la fin de la main du joueurs plus besoin d'effectuer la décallage
		if(players[playerIndex].playerCards[i+1].valeur == 0) {
			// On supprime la dernière carte de la main
			players[playerIndex].playerCards[i].valeur = 0;
			players[playerIndex].playerCards[i].cattleHead = 0;

			break;
		}

		players[playerIndex].playerCards[i] = players[playerIndex].playerCards[i+1];
	}
}

/**
 * Permet d'afficher les cartes d'un joueur
 */
void printPlayerCards(int playerIndex)
{
	bool end = false;
	int currentIndex = 0;

	while (!end) {
		if (players[0].playerCards[currentIndex].valeur == 0) {
			printf("\n");
			end = true;
		} else {
			printf("[%d|%d]",
					players[playerIndex].playerCards[currentIndex].valeur,
					players[playerIndex].playerCards[currentIndex].cattleHead);

			currentIndex++;
		}
	}
}

/**
 * Permet de mettre à jour le score des joueurs
 */
void updatePlayerScore()
{
	for (int currentPlayer = 0; currentPlayer < nbPlayers; currentPlayer++) {
		bool endOfHand = false;
		int currentCard = 0;

		// Réinitialisation du score du joueur
		players[currentPlayer].score = 0;

		while (!endOfHand) {
			if (players[currentPlayer].playerCards[currentCard].valeur == 0) {
				endOfHand = true;
			} else {
				players[currentPlayer].score += players[currentPlayer].playerCards[currentCard].cattleHead;
				currentCard ++;
			}
		}
	}
}


/**
 * Permet de vérifier si un joueur a gagné
 */
bool checkIfPlayerWon()
{
	bool won = false;

	for (int i = 0; i < nbPlayers; i++) {
		if(players[i].score == 0) won = true;
	}

	return won;
}

/**
 * Permet de commencer une nouveau tour
 */
void startRound()
{
	updatePlayerScore();
	currentRound ++;

	printf("\t -- Début du tour %d -- \n", currentRound);
	printTable();
}

/**
 * Permet de terminer un round
 */
void endRound()
{
	updatePlayerScore();
	printf("\t -- Fin du tour %d -- \n", currentRound);
	printTable();

	printPlayersScore();

	// Si un joueur a gagné, on arrête la partie
	if (checkIfPlayerWon()) {
		endGame();
	} else {
		startRound();
	}
}


/**
 * Permet démarer la partie
 */
void beginGame()
{
	printf("----- Début de la partie ----- \n");
	startRound();
}

/**
 * Permet de terminer la partie
 */
void endGame()
{
	printf("\t ---------------------\n");
	printf("\t | Partie terminée ! |\n");
	printf("\t ---------------------\n");

	/* Affichage des scores de tout les joueur */
	printPlayersScore();

	/* Fermeture du serveur */
	exit(0);
}

/**
 * Permet d'afficher le score de tout les joueur
 */
void printPlayersScore()
{
	printf("\t ---------------------\n");
	printf("\t --      Scores     --\n");
	printf("\t ---------------------\n");

	for (int i = 0; i < nbPlayers; i++) {
		if(players[i].score == 0) {
			printf("\t %s : %d points => Vainqeur ! \n", players[i].name, players[i].score);
		} else {
			printf("\t %s : %d points \n", players[i].name, players[i].score);
		}
	}
}

/**
 * Permet de ramasser une ligne et de la mettre dans la main d'un joueur
 */
void takeLigne(int choice, int playerIndex)
{
	// On récupère la carte la plus petite du joueur
	int indexOfSmallestCard = -1;
	int smallestValue = NB_CARD+1;
	int currentIndex = 0;
	bool isEnd = false;

	while (!isEnd) {
		if(players[playerIndex].playerCards[currentIndex].valeur == 0) isEnd=true;

		else {
			if (players[playerIndex].playerCards[currentIndex].valeur < smallestValue) {
				smallestValue = players[playerIndex].playerCards[currentIndex].valeur;
				indexOfSmallestCard = currentIndex;
			}

			currentIndex++;
		}
	}

	// On retire cette carte de la main du joueur
	CARD tempCard = players[playerIndex].playerCards[indexOfSmallestCard];
	deleteCardFromPlayersCards(players[playerIndex].playerCards[indexOfSmallestCard], playerIndex);

	// On lui donne les cartes de la ligne choisie
	for (int i = 0; i <= table[choice].currentLastIndex; i++) {
		addCard(table[choice].row[i], playerIndex);
	}

	// On remplace la ligne choisie par la carte du joueur
	TABLEROW tempRow;
	tempRow.currentLastIndex = 0;
	tempRow.row[0] = tempCard;

	// On remplace la ligne de la table
	table[choice] = tempRow;


	printf("\t -- %s à ramassé la ligne %d -- \n", players[playerIndex].name, choice);
	printTable();
}

/**
* Permet d'ajouter une carte à la main du joueur
*/
void addCard(CARD cardToAdd, int playerIndex)
{
	bool isEnd = false;
	int currentIndex = 0;

	while (!isEnd) {
		if(players[playerIndex].playerCards[currentIndex].valeur == 0) {
			players[playerIndex].playerCards[currentIndex] = cardToAdd;
			isEnd = true;
		}

		currentIndex ++;
	}
}
