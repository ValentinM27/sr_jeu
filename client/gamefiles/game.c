#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// En-tête
#include "game.h"

// Définition des fonctions
PLAYER you;

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
