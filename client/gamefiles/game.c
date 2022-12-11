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