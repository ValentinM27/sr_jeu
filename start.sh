#! /bin/bash

# Pour utiliser
# ./start <Nombre de joueur humains>
# Permet de démarre le serveur avec n bots
make server-start nbplayers=6 &

#Démarage du nombre de bots nécessaire
nbbot=$(expr 6 - $1)

# Démarrage des bots
make bot-start nbbot=$nbbot
