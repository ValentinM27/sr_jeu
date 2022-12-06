#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Paramètres du serveur
#define server_PORT 8080
#define server_IP "127.0.0.1"

/**
 * @brief Serveur
 *
 * @return int
 */
int main(void)
{
	// Descripteur du serveur
	int serverSocket, inputStream;
	struct sockaddr_in serverAddr;

	// Gestion des connexions multiples
	socklen_t addr_size;

	int newSocket;
	struct sockaddr_in newAddr;

	char buffer[1024];
	pid_t childpid;

	// Création du scoket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0) {
		printf("[NOK] - Erreur de création du socket");
		return -1;
	}

	// Allocation espace mémoire
	memset(&serverAddr, '\0', sizeof(serverAddr));

	// Définition des paramètres serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(server_IP);

	// Écoute du port
	inputStream = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(inputStream < 0){
		printf("[NOK] - Erreur d'eçoute du port");
		return -1;
	}

	if(listen(serverSocket, 10) == 0){
		printf("[Server] - En attente de flux");
	} else {
		printf("[NOK] - Erreur d'eçoute du port");
		return -1;
	}

	// Gestion des nouveaux clients
	while(1){
		newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addr_size);

		if(newSocket < 0) {
			printf("[NOK] - Erreur de connexion d'un client");
			return -1;
		}
		printf("Connexion d'un nouveau joueur");

		// Création d'un nouveau processus pour le nouveau client
		if((childpid = fork()) == 0){
			// fermeture du socket dans le nouveau processus (Éviter fork bomb)
			close(serverSocket);
			// Reception des messages du client
			while(1) {
				recv(newSocket, buffer, 1024, 0);
				printf("[Client] : %s\n", buffer);
				send(newSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
			}
		}
	}

    return 0;
}