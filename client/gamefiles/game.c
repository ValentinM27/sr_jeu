#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// En-tête
#include "game.h"

// Définition des fonctions
PLAYER you;
int currentRound;
TABLEROW table[4];

/**
 * Permet d'affciher les cartes du joueur
 */
void printPlayerCard()
{
	bool end = false;
	int currentIndex = 0;

	printf("-- Vos cartes -- \n");

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
 * Permet de savoir si le joueur peux poser une carte
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

				// Si le joueur peux jouer cette carte, on arrête la recherche
				isEnd = canPlay;

				if(isEnd) break;
			}
		}
	}

	return canPlay;
}

/**
 * Permet de vérifier si le joueur peux poser la carte sélectionnée
 */
bool checkCanPlayThisCard(CARD cardToPlay)
{
	bool canPlay = false;

	for (int i = 0; i < 4; i++) {
		canPlay = table[i].row[table[i].currentLastIndex].valeur < cardToPlay.valeur;

		// Si le joueur peux jouer, on quitte la boucle
		if(canPlay) break;
	}

	return canPlay;
}

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
* Permet d'ajouter une carte à la main du joueur
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

/**
 * Permet de calculer le score du joueur
 */
void calculateScore()
{
bool endOfHand = false;
	int currentCard = 0;

	// Réinitialisation du score du joueur
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
 * Permet d'afficher le score du joueur
 */
void printYourScore()
{
	calculateScore();

	printf("\t ------------------------\n");
	printf("\t Votre score : %d points \n", you.score);
	printf("\t ------------------------\n");
}
