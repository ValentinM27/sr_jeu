#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// En-tête
#include "game.h"

// Variable du jeu
PLAYER you;
int currentRound;
TABLEROW table[4];

/**
 * Permet d'affciher les cartes du bot
 */
void printBotCard()
{
	bool end = false;
	int currentIndex = 0;

	printf("-- cartes en mains -- \n");

	while (!end) {
		if (you.playerCards[currentIndex].valeur == 0) {
			printf("\n");
			end = true;
		} else {
			if(currentIndex%3 == 0) printf("\n");

			printf("%d : [%d|%d] \t",
					currentIndex,
					you.playerCards[currentIndex].valeur,
					you.playerCards[currentIndex].cattleHead);

			currentIndex++;
		}
	}
}

/**
 * Permet d'associer la bonne valeur de tête de boeuf à une carte
 */
CARD createCard(int valeur)
{
	CARD createdCard;

	createdCard.valeur = valeur;
	createdCard.cattleHead = 1;

	if(valeur == 55) {
		createdCard.cattleHead = 7;
	} else if((valeur)%11 == 0) {
		createdCard.cattleHead = 5;
	} else if((valeur)%10 == 0) {
		createdCard.cattleHead = 3;
	} else if((valeur)%5 == 0) {
		createdCard.cattleHead = 2;
	}

	return createdCard;
}

/**
 * Affichage de la table de jeu
 */
void printTable()
{
	// Titre
	printf("\t ---------------------- \n");
	printf("\t - Carte sur la table - \n");
	printf("\t ---------------------- \n");

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
 * Permet de savoir si le bot peux poser une carte
 */
bool canPlay()
{
	bool canPlay = false;

	bool isEnd = 0;
	int currentIndex = 0;

	while(!isEnd) {
		if(you.playerCards[currentIndex].valeur == 0) isEnd = true;

		else {
			// On compare la carte à la dernière carte de chaque ligne
			for (int i = 0; i < 4; i++)
			{
				canPlay = table[i].row[table[i].currentLastIndex].valeur < you.playerCards[currentIndex].valeur;
				currentIndex++;

				// Si le bot peux jouer cette carte, on arrête la recherche
				isEnd = canPlay;

				if(isEnd) break;
			}
		}
	}

	return canPlay;
}

/**
 * Retourne l'index de la meilleure carte à jouer pour le bot
 *
 * Le bot essaye de placer la carte avec la plus petite valeur disponible
 */
int searchBestCardToPlay()
{
	// Correspond à la plus petite carte sur la table
	CARD smallestCardOfTable = table[0].row[table[0].currentLastIndex];

	// Variables pour parcourir les cartes du bot
	bool isEnd = false;
	int currentIndex = 0;

	// Correspond à la meilleure carte que peux jouer le bot
	int indexOfSmallestCardOfBot = -1;
	int valeurOfSmallestCardOfBot = NB_CARD + 1;

	// On recherche la plus petite carte sur la table
	for(int i = 1; i < 4; i++) {
		if(smallestCardOfTable.valeur > table[i].row[table[i].currentLastIndex].valeur)
			smallestCardOfTable = table[i].row[table[i].currentLastIndex];
	}

	// On recherche la carte la plus petite que peux jouer le bot
	while (!isEnd) {
		if(you.playerCards[currentIndex].valeur == 0)
			isEnd = true;

		else {
			if(valeurOfSmallestCardOfBot > you.playerCards[currentIndex].valeur
				&& smallestCardOfTable.valeur < you.playerCards[currentIndex].valeur
			) {
				valeurOfSmallestCardOfBot = you.playerCards[currentIndex].valeur;
				indexOfSmallestCardOfBot = currentIndex;
			}

			currentIndex ++;
		}
	}

	return indexOfSmallestCardOfBot;
}


/**
 * Permet de supprimer une carte de la main du joueur
 */
void deleteCardFromHand(CARD cardToDelete)
{
	int indexOfTheDeletedCard;

	for (int i = 0; i < NB_CARD; i++) {
		if (
			you.playerCards[i].valeur == cardToDelete.valeur &&
			you.playerCards[i].cattleHead == cardToDelete.cattleHead
		) {
			indexOfTheDeletedCard = i;
			break;
		}
	}

	for (int i = indexOfTheDeletedCard; i < NB_CARD; i++) {

		// Si on arrive à la fin de la main du joueurs plus besoin d'effectuer la décallage
		if(you.playerCards[i+1].valeur == 0) {
			// On supprime la dernière carte de la main
			you.playerCards[i].valeur = 0;
			you.playerCards[i].cattleHead = 0;

			break;
		}

		you.playerCards[i] = you.playerCards[i+1];
	}
}

/**
 * Permet de calculer le score du bot
 */
void calculateScore()
{
bool endOfHand = false;
	int currentCard = 0;

	// Réinitialisation du score du bot
	you.score = 0;

	while (!endOfHand) {
		if (you.playerCards[currentCard].valeur == 0) {
			endOfHand = true;
		} else {
			you.score += you.playerCards[currentCard].cattleHead;
			currentCard ++;
		}
	}
}

/**
 * Permet d'afficher le score du bot
 */
void printBotScore()
{
	calculateScore();

	printf("\t ------------------------\n");
	printf("\t    score : %d points \n", you.score);
	printf("\t ------------------------\n");
}

/**
* Retour l'index de la meilleure ligne à ramasser
*/
int findBestLineToDraw()
{
	// Correspond à l'index de la meilleure ligne à ramasser
	int bestCattleHead = -1;
	int indexBestLine = 0;

	// On parcours chaque ligne et on calculs la valeur des tête de boeufs
	for(int i = 0; i < 4; i ++) {
		int tempCattleHead = 0;

		// Somme des têtes de boeufs
		for(int currentCard = 0; currentCard < table[i].currentLastIndex; currentCard ++) {
			tempCattleHead += table[i].row[currentCard].cattleHead;
		}

		if(i == 0) {
			bestCattleHead = tempCattleHead;
			indexBestLine = i;
		} else {
			if (bestCattleHead > tempCattleHead) {
				bestCattleHead = tempCattleHead;
				indexBestLine = i;
			}
		}
	}

	return indexBestLine;
}

/**
 * Permet au joueur de récupérer une ligne
 */
void takeLigne(int choice)
{
	// On récupère la carte la plus petite du joueur
	int indexOfSmallestCard = -1;
	int smallestValue = NB_CARD+1;
	int currentIndex = 0;
	bool isEnd = false;

	while (!isEnd) {
		if(you.playerCards[currentIndex].valeur == 0) isEnd=true;

		else {
			if (you.playerCards[currentIndex].valeur < smallestValue) {
				smallestValue = you.playerCards[currentIndex].valeur;
				indexOfSmallestCard = currentIndex;
			}

			currentIndex++;
		}
	}

	deleteCardFromHand(you.playerCards[indexOfSmallestCard]);

	// On lui donne les cartes de la ligne choisie
	for (int i = 0; i <= table[choice].currentLastIndex; i++) {
		addCard(table[choice].row[i]);
	}
}

/**
* Permet d'ajouter une carte à la main du bot
*/
void addCard(CARD cardToAdd)
{
	bool isEnd = false;
	int currentIndex = 0;

	while (!isEnd) {
		if(you.playerCards[currentIndex].valeur == 0) {
			you.playerCards[currentIndex] = cardToAdd;
			isEnd = true;
		}

		currentIndex ++;
	}
}
