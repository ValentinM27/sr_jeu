# Permet de lancer le build de toute l'application
build:
	-@cd server && make build && cd ..
	-@cd client && make build

# Permet de démarrer le serveur
server-start:
	-@./server/bin/server.out 

# Permet de lancer un client
client-start:
	-@./client/bin/client.out $(ip)

