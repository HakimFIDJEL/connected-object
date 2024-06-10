all: librairies archives serveur client
	# clear
	# @echo "Compilation terminée avec succès."

serveur: archives
	gcc mainSrv.c -o mainSrv -L./archives -lUNIX 
client: archives
	gcc mainClt.c -o mainClt -L./archives -lUNIX 

librairies:
	gcc -c ./sources/data.c -o ./libs/data.o
	gcc -c ./sources/session.c -o ./libs/session.o
	gcc -c ./sources/users.c -o ./libs/users.o
	gcc -c ./sources/game.c -o ./libs/game.o 


archives: librairies
	mkdir -p archives
	mkdir -p libs
	ar rcs ./archives/libUNIX.a ./libs/data.o ./libs/session.o ./libs/users.o ./libs/game.o

clean:
	rm -f mainSrv mainClt ./libs/* ./archives/*
	clear
	@echo "Nettoyage terminé avec succès."


