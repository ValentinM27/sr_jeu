# sr_jeu
Permet de jouer au jeu du 6 en ligne

## Buid l'application
- `make build`

## Démarrer le serveur
- `make server-start` : Ne démarre aucuns bots
- `./start.sh <nb joueur>` : Démarre `n` bots pour arriver à 6 joueurs

## Démarrer un client
- `make client-start [ip=...]` : Permet de se connecter au serveur, permet de spécifier une adresse IP

## Démarrer des bots
- `make bot-start nbbots=...` : Permet de démarrer `n` instance de bots
