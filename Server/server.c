// Add to linker "-lws2_32" to execute winsock2.h

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../Util/constants.c"

//#include "../Util/socketList.c"

struct sockaddr_in socketTCPAddress;
SOCKET socketTCP;

void sendResponse(SOCKET client, char *response){
	send(client , response , strlen(response) , 0);
}

char* createResponse(char *petition){
	char* response = "{'response' : 'Hello Client'}";
	char  response_len[10];
	sprintf(response_len, "%d", strlen(response));

	// Building the JSON response
	char *message = malloc (sizeof (char) * RESPONSE_LEN);
	strcat(message, "HTTP/1.1 200 OK\r\n");
	strcat(message, "Content-Type: application/json\r\n");
	strcat(message, "Content-Length: ");
	strcat(message, response_len);
	strcat(message, "\r\n\r\n");
	strcat(message, response);

	return message;
}

void receiveRequest(SOCKET client, char *petition){
	petition[strlen(petition)-1] = '\0';
	char *HTTP_REQUEST = strtok(petition, "\n");
	printf(HTTP_REQUEST);
	printf("\n");

	// Creating the response
	char *response = createResponse(HTTP_REQUEST);
	// Sending the response
	sendResponse(client, response);
}

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
	char client_petition[PETITION_LEN];
	int recv_size, c;
	SOCKET new_socket;
	struct sockaddr_in server , client;

	c = sizeof(struct sockaddr_in);

	printf("Listening for new connections in ");
	printf(PROXY_IP);
	printf(":");
	printf("%d", PORT);
	printf("\n");

	//Listen to incoming connections
	listen(socketTCP , MAX_CONNECTIONS);

	while(1) {
		// Looking for new connections
		if((new_socket = accept(socketTCP , (struct sockaddr *)&client, &c)) != INVALID_SOCKET ) {
			if ((recv_size = recv(new_socket , client_petition , PETITION_LEN , 0)) != SOCKET_ERROR){
				receiveRequest(new_socket, client_petition);
			}
		}
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
