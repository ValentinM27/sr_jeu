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
	}
}

/**
 * Permet d'initialiser le jeu
 */
void initGame(int nbPlayers)
{
	/* Initiatisation du paquet */
	createCards();

	/* Création des mains des joueurs */
	initPackages(nbPlayers);

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
		printf("Ligne %d: [%d|%d]", i+1, table[i].row[0].valeur, table[i].row[0].cattleHead);

		int done = -1;
		int currentIndex = 1;
		
		// Affichage du reste de la liste
		while (done == -1) {
			if (table[i].row[currentIndex].valeur == 0) {
				done = 1;
				printf("\n");
			} else {
				printf("[%d|%d]", table[i].row[currentIndex].valeur, table[i].row[currentIndex].cattleHead);
				currentIndex ++;
			}
		}
	}
}

