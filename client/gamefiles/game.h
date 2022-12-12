#ifndef H_GL_GAME
#define H_GL_GAME

#include <stdbool.h>

// Définition des paramètres
#define NB_CARD 105

/* Définition de la structure d'une carte */
typedef struct _CARD_ {
	int valeur;
	int cattleHead;
} CARD;

/* Définition de la structure d'un joueur */
typedef struct _PLAYER_ {
	char name[12];
	int score;
	CARD playerCards[NB_CARD];
} PLAYER;

/* Définition d'un rangé de carte sur la table */
typedef struct _TABLEROW_ {
	CARD row[NB_CARD];
	int currentLastIndex;
} TABLEROW;

// Varibles du jeu
extern PLAYER you;
extern int currentRound;

// Table de jeu
extern TABLEROW table[4];

// Export des fonctions
void printPlayerCard();
CARD createCard(int valeur);
void printTable();
void deleteCardFromHand(CARD cardToDelete);
bool canPlay();
bool checkCanPlayThisCard(CARD cardToPlay);
void takeLigne();
void addCard(CARD cardToAdd);
void calculateScore();
void printYourScore();

#endif
