#include "../heads/game.h"
#include "../heads/leds.h"
#include "../heads/buttons.h"
#include "../heads/buzzers.h"



#define MAX_FIGURE 2





int figure[4][4];
int tempFigure[4][4];

int figureUpScale[8][8];
int tempFigureUpScale[8][8];
    


// Vide la figure donnée en paramètre
void emptyFigure(int fig[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fig[i][j] = 0;
        }
    }
}

// Remplit la figure avec des 1 et des 0 aléatoirement
void fillFigure()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(rand() % 2 == 0)
            {
                figure[i][j] = 1;
            }
            else
            {
                figure[i][j] = 0;
            }
        }
    }
}

void addCaseToFigure(int col, int row)
{
    tempFigure[col][row] = 1;
}

// Upscale la figure donnée en paramètre
void upscaleFigure(int fig[4][4], int figUpscale[8][8])
{
    // Initialise le tableau agrandi à zéro
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            figUpscale[i][j] = 0;
        }
    }

    // Agrandit chaque cellule du tableau `[4][4]` en doublant sa taille
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            int value = fig[i][j];
            figUpscale[i * 2][j * 2] = value;
            figUpscale[i * 2 + 1][j * 2] = value;
            figUpscale[i * 2][j * 2 + 1] = value;
            figUpscale[i * 2 + 1][j * 2 + 1] = value;
        }
    }
}


// Vérifie si la case de la figure est correcte
bool checkIfCaseIsCorrect(int x,int y)
{
    if(figure[x][y] == 1)
    {
        return true;
    }
    return false;
}

// Vérifie si la figure est correcte
bool checkIfFigureIsCorrect()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(figure[i][j] != tempFigure[i][j])
            {
                return false;
            }
        }
    }
    return true;
}



void game()
{

    setupLeds();
    setupButtons();
    setupBuzzer();

    Position button = {-1, -1};
    int nbrFigure = 0;

   

    while(nbrFigure < MAX_FIGURE)
    {
        // On vide les tableux de figures
        emptyFigure(tempFigure);
        emptyFigure(figure);

        // On génère une figure aléatoire
        fillFigure();

        // On l'agrandit et on l'affiche sur la matrice de led 8x8
        upscaleFigure(figure, figureUpScale);
        displayFigure(figureUpScale);

        // On met un temps de 3s puis c'est au joueur de reproduire la figure
        printf("A vous de jouer\n");
        bcm2835_delay(3000); 
        emptyMatrice();

        // Tant que la figure n'est pas correcte, on laisse le joueur jouer
        while(!checkIfFigureIsCorrect())
        {
            button = readButtons();

            // Si le joueur presse un bouton
            if (button.row != -1 && button.col != -1) 
            {
                // Si la case est correcte on l'ajoute à la figure temporaire
                if(checkIfCaseIsCorrect(button.row, button.col))
                {
                    // Si la case a pas déjà été ajoutée
                    if(tempFigure[button.row][button.col] == 1)
                    {
                        printf("Case déjà ajoutée\n");
                        continue;
                    }
                    else 
                    {
                        printf("Case ajoutée\n");

                        emptyMatrice();

                        bcm2835_delay(100); 
                        // On affiche la figure temporaire
                        addCaseToFigure(button.row, button.col);
                        upscaleFigure(tempFigure, tempFigureUpScale);
                        displayFigure(tempFigureUpScale);
                    }
                }
                else 
                {
                    printf("Mauvaise case\n");
                    buzzerError();
                    ledError();
                    emptyFigure(tempFigure);
                    emptyMatrice();

                    bcm2835_delay(100); 

                    displayFigure(figureUpScale);
                    bcm2835_delay(3000); 
                    emptyMatrice();
                }
            } else {
                bcm2835_delay(100); 
            }
        }
        buzzerSuccess();
        ledSuccess();
        nbrFigure++;
    }

    printf("Bravo vous avez réussi\n");
    buzzerSuccess();
    ledSuccess();

    closeLeds();

    return;
}