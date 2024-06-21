#ifndef LEDS_H
#define LEDS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <bcm2835.h>



void displayFigure(int fig[8][8]);
void emptyMatrice();
void setupLeds();
void Write_Max7219(unsigned char reg, unsigned char col);
unsigned char rowToBinary(int rowIndex, int fig[8][8]);
void closeLeds();
void ledSuccess();
void ledError();
void ledWait();
void ledCountDown(int x);
#endif
