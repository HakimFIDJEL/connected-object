#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void emptyFigure(int fig[4][4]);
void fillFigure();
void upscaleFigure(int fig[4][4], int figUpscale[8][8]);
bool checkIfCaseIsCorrect(int x,int y);
bool checkIfFigureIsCorrect();
void addCaseToFigure(int col, int row);
void game();

#endif