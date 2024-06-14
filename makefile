# variables
CCDIR = CC/gcc-linaro-arm-linux-gnueabihf-raspbian-x64
CC = $(CCDIR)/bin/arm-linux-gnueabihf-gcc
WPI = CC/target-wpi
BCM2835_INCLUDE = /usr/local/include
BCM2835_LIB = /usr/local/lib

all: librairies archives serveur client copy

serveur: archives
	gcc mainSrv.c -o mainSrv -L./archives -lUNIX -lpthread

librairies:
	gcc -c ./sources/data.c -o ./libs/data.o
	gcc -c ./sources/session.c -o ./libs/session.o
	gcc -c ./sources/users.c -o ./libs/users.o

archives: librairies librairiesPi
	mkdir -p archives
	mkdir -p libs
	mkdir -p libsPi
	ar rcs ./archives/libUNIX.a ./libs/data.o ./libs/session.o ./libs/users.o 
	ar rcs ./archives/libUNIXPi.a ./libsPi/data.o ./libsPi/session.o ./libsPi/game.o ./libsPi/buttons.o ./libsPi/leds.o ./libsPi/buzzers.o

librairiesPi:
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/data.c -o ./libsPi/data.o
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/session.c -o ./libsPi/session.o
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/leds.c -o ./libsPi/leds.o
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/buttons.c -o ./libsPi/buttons.o
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/game.c -o ./libsPi/game.o
	$(CC) -std=c99 -I$(WPI)/include -I$(BCM2835_INCLUDE) -c ./sources/buzzers.c -o ./libsPi/buzzers.o

client: archives
	$(CC) -std=c99 -L$(WPI)/lib -I$(WPI)/include -I$(BCM2835_INCLUDE) mainClt.c -o mainClt -L./archives -L$(BCM2835_LIB) -lUNIXPi -lpthread -lwiringPi -lbcm2835

copy:
	sshpass -p "password" scp -r mainClt pi@192.168.1.169:~/Documents/ObjectCo 

clean:
	rm -f mainSrv mainClt mainCltPi ./libs/* ./archives/*
	clear
	@echo "Nettoyage terminé avec succès."
