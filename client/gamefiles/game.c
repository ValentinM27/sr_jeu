#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// En-tête
#include "game.h"

// Définition des fonctions
PLAYER you;
int currentRound;

/**
 * Permet d'affciher les cartes du joueur
 */
void printPlayerCard()
{
	bool end = false;
	int currentIndex = 0;

	printf("\t -- Vos cartes -- \n");

	while (!end) {
		if (you.playerCards[currentIndex].valeur == 0) {
			printf("\n");
			end = true;
		} else {
			printf("[%d|%d]",
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
