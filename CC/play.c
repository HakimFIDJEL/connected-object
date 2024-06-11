#include <stdio.h>
#include <wiringPi.h>


// TODO : jouer un son lors de l'appui sur le bouton
// dont le numéro est passé en paramètre

#define BUZZER 1
#define BOUTON 6 // physical 22 BOUTON LEFT

// pullup : par défaut, état HAUT
// si appuyé, état bas
#define APPUYE 0

int main(int argc, char *argv[]) 
{
  int pinBtn = BOUTON;
  
  if(argc == 2)
  {
    pinBtn = atoi(argv[1]);
  }

  printf("Bouton : %d\n", pinBtn);

  wiringPiSetup();
  pinMode(BUZZER, OUTPUT);
  pinMode(pinBtn, INPUT);
  pullUpDnControl(pinBtn, PUD_UP);

  while(1)
  {
    if(digitalRead(pinBtn) == APPUYE)
    {
      printf("Bouton appuyé\n");
      digitalWrite(BUZZER, HIGH);
    }
    else
    {
      digitalWrite(BUZZER, LOW);
    }
  }

  return 0;

}