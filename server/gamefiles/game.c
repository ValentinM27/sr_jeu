#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// En-tête
#include "game.h"

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

		if((i+1)%11 == 0) {
			tempCard.cattleHead = 5;
		} else if((i+1)%10 == 0) {
			tempCard.cattleHead = 3;
		} else if((i+1)%5 == 0) {
			tempCard.cattleHead = 2;
		}

		cards[i] = tempCard;
	}
}

