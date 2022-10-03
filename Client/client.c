// Add to linker "-lws2_32" to execute winsock2.h

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../Util/constants.c"

struct sockaddr_in socketTCPAddress;
SOCKET socketTCP;

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

void connectSocket(){
    if (connect(socketTCP , (struct sockaddr *)&socketTCPAddress , sizeof(socketTCPAddress)) < 0) {
        printf("Connect error \n");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected \n");
}

void listenSocket(){
	int recv_size, c;
	char server_reply[10000];    
	char message[] = "";

	// message != QUIT
	while (strcmp(message,"QUIT") != 0){
		// Writting a message
		printf("Type a message: \n");
		scanf("%s", message);

		// Sending a message
		if(send(socketTCP , message , strlen(message) , 0) < 0) {
			printf("Send failed");	
		}

		// Receiving a message
		if ((recv_size = recv(socketTCP , server_reply , 10000 , 0)) != SOCKET_ERROR){
			server_reply[recv_size] = '\0';
			puts(server_reply);
		}
	}
}

int main(int argc, char *argv[]) {
	initSocket();
	connectSocket();
	listenSocket();
    
	return 0;
}