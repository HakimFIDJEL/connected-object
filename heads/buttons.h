#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <bcm2835.h>

typedef struct {
    int row;
    int col;
} Position;

void setupButtons();
Position readButtons();

#endif // BUTTONS_H
