#ifndef H_GL_GAME
#define H_GL_GAME

#define NB_CARD 105

// Définition de la structure d'une carte
typedef struct _CARD_ {
	int valeur;
	int cattleHead;
} CARD;

// Variables du jeu
int currentRound;
CARD cards[NB_CARD];

// Tableau de cartes des joueurs
CARD j0[NB_CARD];
CARD j1[NB_CARD];
CARD j2[NB_CARD];
CARD j3[NB_CARD];
CARD j4[NB_CARD];
CARD j5[NB_CARD];

// Déclaraction des fonctions
void createCards();

#endif

