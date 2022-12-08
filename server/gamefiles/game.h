#ifndef H_GL_GAME
#define H_GL_GAME

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

// Variables du jeu
int currentRound;
int currentCardInCards;
CARD cards[NB_CARD];

// Tableau des joueurs
PLAYER players[6];

// Déclaraction des fonctions
void createCards();
CARD pickRandomCard();
void initPackages();

#endif

