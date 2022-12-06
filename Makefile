# Permet de lancer le build de toute l'application
build:
	-@cd server && make build && cd ..
	-@cd client && make build