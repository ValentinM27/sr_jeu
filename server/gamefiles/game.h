#ifndef H_GL_GAME
#define H_GL_GAME

// Import des libs
#include <stdbool.h>

// Import des libs et définition des paramètres
#define NB_CARD 105

/* Définition de la structure d'une carte */
typedef struct _CARD_ {
	int valeur;
	int cattleHead;
} CARD;

/* Définition de la structure d'un joueur */
typedef struct _PLAYER_ {
	char name[12];
	CARD playerCards[NB_CARD];
} PLAYER;

/* Définition d'un rangé de carte sur la table */
typedef struct _TABLEROW_ {
	CARD row[NB_CARD];
	int currentLastIndex;
} TABLEROW;

// Variables du jeu
int currentRound;
int currentCardInCards;
CARD cards[NB_CARD];

// Table de jeu
TABLEROW table[4];

// Tableau des joueurs
PLAYER players[6];

// Déclaraction des fonctions
void createCards();
CARD pickRandomCard();
void initPackages();
void initTable();
void initGame();
void printTable();
bool putCardOnTable(CARD cardToPlace, int playerIndex);
void deleteCardFromPlayersCards(CARD cardToDelete, int playerIndex);
void printPlayerCards(int playerIndex);

#endif

