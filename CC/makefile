# variables
CCDIR = gcc-linaro-arm-linux-gnueabihf-raspbian-x64
CC = $(CCDIR)/bin/arm-linux-gnueabihf-gcc
WPI = target-wpi

all : play

copy:
	sshpass -p "password" scp -r play pi@192.168.1.169:~/Documents/ObjectCo 
	
play : play.c
	$(CC) "-L$(WPI)/lib" -I$(WPI)/include play.c -o play -lwiringPi

clean :
	rm play
