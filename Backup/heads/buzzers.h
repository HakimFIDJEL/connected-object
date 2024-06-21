#ifndef BUZZERS_H
#define BUZZERS_H

#include <stdio.h>

#ifdef HIGH
#undef HIGH
#endif

#ifdef LOW
#undef LOW
#endif

#ifdef delayMicroseconds
#undef delayMicroseconds
#endif

#include <wiringPi.h>

void setupBuzzer();
void buzzerSuccess();
void buzzerError();

#endif
