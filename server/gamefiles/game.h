#ifndef H_GL_GAME
#define H_GL_GAME

// Import des libs
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

// Variables du jeu
extern int currentRound;
extern int currentCardInCards;
extern CARD cards[NB_CARD];
extern int nbPlayers;

// Table de jeu
extern TABLEROW table[4];

// Tableau des joueurs
extern PLAYER players[6];

// Déclaraction des fonctions
void createCards();
CARD pickRandomCard();
void initPackages();
void initTable();
void initGame(int nbConnectedPlayers);
void printTable();
bool putCardOnTable(CARD cardToPlace, int playerIndex);
void deleteCardFromPlayersCards(CARD cardToDelete, int playerIndex);
void printPlayerCards(int playerIndex);
void updatePlayerScore();
bool checkIfPlayerWon();
void startRound();
void endRound();
void beginGame();
void endGame();
void printPlayersScore();

#endif

