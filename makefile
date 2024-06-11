CCDIR = CC/gcc-linaro-arm-linux-gnueabihf-raspbian-x64
CC = $(CCDIR)/bin/arm-linux-gnueabihf-gcc
WPI = CC/target-wpi


all: librairies archives serveur client
	# clear
	# @echo "Compilation terminée avec succès."



serveur: archives
	gcc mainSrv.c -o mainSrv -L./archives -lUNIX -lpthread
client: archives
	gcc mainClt.c -o mainClt -L./archives -lUNIX -lpthread




librairies:
	gcc -c ./sources/data.c -o ./libs/data.o
	gcc -c ./sources/session.c -o ./libs/session.o
	gcc -c ./sources/users.c -o ./libs/users.o

archives: librairies librairiesPi
	mkdir -p archives
	mkdir -p libs
	mkdir -p libsPi
	ar rcs ./archives/libUNIX.a ./libs/data.o ./libs/session.o ./libs/users.o 
	ar rcs ./archives/libUNIXPi.a ./libsPi/data.o ./libsPi/session.o ./libsPi/game.o


librairiesPi:
	$(CC) -c ./sources/data.c -o ./libsPi/data.o
	$(CC) -c ./sources/session.c -o ./libsPi/session.o
	$(CC) -c ./sources/game.c -o ./libsPi/game.o


clientPi : archives
	$(CC) mainClt.c -o mainCltPi -L./archives -lUNIXPi -lpthread





copy: clientPi
	sshpass -p "password" scp -r mainCltPi pi@192.168.1.169:~/Documents/ObjectCo 

 
clean:
	rm -f mainSrv mainClt ./libs/* ./archives/*
	clear
	@echo "Nettoyage terminé avec succès."


