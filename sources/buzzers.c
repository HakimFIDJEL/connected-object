#include "../heads/buzzers.h"



#define BUZZER 1

void setupBuzzer()
{
    wiringPiSetup();
    pinMode(BUZZER, OUTPUT);

}

void buzzerSuccess() 
{

  
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(100);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

}

void buzzerError()
{
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
}