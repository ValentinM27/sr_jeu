# Permet de lancer le build de l'application serveur
build: main.o game.o
	-@gcc main.o game.o -o bin/server.out
	-@rm game.o
	-@rm main.o

main.o:
	-@gcc -c main.c

game.o:
	-@gcc -c gamefiles/game.c

