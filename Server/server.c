// Add to linker "-lws2_32" to execute winsock2.h

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../Util/constants.c"
#include "../Util/socketList.c"

struct sockaddr_in socketTCPAddress;
SOCKET socketTCP;
struct SocketNode* clients; 

void initSocket(){
	// Starting SOCKETS API
	WSADATA WsaData;
	WORD wVersionRequerida = MAKEWORD (2, 2);
	WSAStartup (wVersionRequerida, &WsaData);

	// TCP Socket
	socketTCP = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Socket address
    socketTCPAddress.sin_family      = AF_INET;
    socketTCPAddress.sin_port        = htons(PORT);
    socketTCPAddress.sin_addr.s_addr = inet_addr(PROXY_IP);
} 

void bindSocket(){
	if( bind(socketTCP ,(struct sockaddr *)&socketTCPAddress , sizeof(socketTCPAddress)) == SOCKET_ERROR) {
		exit(EXIT_FAILURE);
	}
} 

void listenSocket(){
	char client_petition[1000];
	int recv_size, c;
	SOCKET new_socket;
	struct sockaddr_in server , client;
	char *message;
	
	c = sizeof(struct sockaddr_in);

	//Listen to incoming connections
	listen(socketTCP , 3);


	while(1) {

		// Looking for new connections
		if((new_socket = accept(socketTCP , (struct sockaddr *)&client, &c)) != INVALID_SOCKET ) {
			if(clients == NULL) {
				// First client
				clients = initSocketList(new_socket);
			} else {
				// More clients
				appendSocket(clients, new_socket);
			}

			printSocketList(clients);
			
			//Reply to the client
			message = "Hello Client , I have received your connection. But I have to go now, bye\n";
			send(new_socket , message , strlen(message) , 0);
		}

		// Receiving petitions
		if ((recv_size = recv(socketTCP , client_petition , 1000 , 0)) != SOCKET_ERROR){
			client_petition[recv_size] = '\0';
			printf(client_petition);
		}
	}
	
	
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
		return 1;
	}

	closesocket(socketTCP);
	WSACleanup();
	
    
}

int main(int argc, char *argv[]) {
	initSocket();
	bindSocket();
	listenSocket();
    
	return 0;
}
