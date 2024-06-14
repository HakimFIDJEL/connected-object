# variables
CCDIR = gcc-linaro-arm-linux-gnueabihf-raspbian-x64
CC = $(CCDIR)/bin/arm-linux-gnueabihf-gcc
WPI = target-wpi

all : play led

copy:
	sshpass -p "password" scp -r led pi@192.168.1.169:~/Documents/ObjectCo 

	
play : play.c
	$(CC) "-L$(WPI)/lib" -I$(WPI)/include play.c -o play -lwiringPi

led : led.c
	$(CC) -std=c99 "-L$(WPI)/lib" -I$(WPI)/include led.c -o led -lwiringPi

clean :
	rm play
	rm led
