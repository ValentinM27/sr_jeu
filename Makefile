# Permet de lancer le build de toute l'application
build:
	-@cd server && make build && cd ..
	-@cd client && make build && cd ..
	-@cd bots && make build

# Permet de démarrer le serveur
server-start:
	-@./server/bin/server.out $(nbplayers)

# Permet de lancer un client
client-start:
	-@./client/bin/client.out $(ip)

# Permet de lancer n instances d'un bot
bot-start:
	-@./bots/bin/bot.out $(nbbot)

