
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const int nbCartes = 104 ;

int main()
{   char pseudo = 0 ;
    printf("***    Bienvenue dans le jeu du 6 qui prend    ***\nVeuillez entrez votre pseudo :  ");
    scanf("%s",&pseudo);
    printf("La partie va commencer :\n Distribution de cartes en cours...\n ");
    
    

int tabJ[10];
int c, d, n ,w;
bool itIsNew;


srand(time(NULL));
//generer 10 cartes pour le joueur(jai pas reussis a mettre a chaque fois des nombres differents)
for (c = 1; c <= 10;)
{
    n = rand()%100 + 1;
    itIsNew = true;

    for(d = 1; d <= c; d++)
    {
        if(n == tabJ[d - 1])
        {
            itIsNew = false;
            break;
        }       
    }
    if(itIsNew)
    {
        tabJ[c - 1] = n;
        c++;
    }
   
}
 printf("vos cartes sont:\n");
    for(w=0;w<10;w++)
    {   
        printf("%d\t",tabJ[w]);
    }
    
    
//creer nbre de tete de boeuf a mettre sur chaque carte    
    int  i, j=1 , k , z=1 ;
    int nbBoeufs[7] ;
      for (k = 1 ; k < 8 ; k++)
    {
        nbBoeufs[k]=z ;
        z++ ;
    } 
    
//étiqueter chaque carte avec son numero
    int numCartes[nbCartes];
      for (i = 1 ; i < 105 ; i++)
    {
        numCartes[i]= j;
        j++ ;
    }
    
    
    

    return 0;
}
