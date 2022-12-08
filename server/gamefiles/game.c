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
void initPackages(int nbPlayers)
{
	for(int i = 0; i < nbPlayers; i++)
	{
		for(int y = 0; y < 10; y++)
			players[i].playerCards[y] = pickRandomCard();
	}
}

