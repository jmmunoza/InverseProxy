// Add to linker "-lws2_32" to execute winsock2.h

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../Util/constants.c"

//#include "../Util/socketList.c"

struct sockaddr_in socketProxyAddress;
SOCKET socketProxy;

void sendResponse(SOCKET client, char *response){
	send(client , response , strlen(response) , 0);
}

void receiveRequest(SOCKET client, char *petition){
	// --------------------------------------------------------------------
	// Connect to WEBAPP
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serverAddress;
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_port        = htons(SERVER_1_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_1_IP);

	if (connect(serverSocket , (struct sockaddr *)&serverAddress , sizeof(serverAddress)) >= 0) {
       
		// Sending a message
		if(send(serverSocket , petition , strlen(petition) , 0) >= 0) {
			char server_reply[RESPONSE_LEN]; 
			int recv_size;

			if ((recv_size = recv(serverSocket , server_reply , RESPONSE_LEN, 0)) != SOCKET_ERROR) {
				server_reply[recv_size] = '\0';
				printf(server_reply);
				sendResponse(client, server_reply);
				
				if ((recv_size = recv(serverSocket , server_reply , RESPONSE_LEN, 0)) != SOCKET_ERROR) {
					server_reply[recv_size] = '\0';
					sendResponse(client, server_reply);
				}
			}

			closesocket(serverSocket);
		}
    } 
}

void initSocketProxy(){
	// Starting SOCKETS API
	WSADATA WsaData;
	WORD wVersionRequerida = MAKEWORD (2, 2);
	WSAStartup (wVersionRequerida, &WsaData);

	// TCP Socket
	socketProxy = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Socket address
    socketProxyAddress.sin_family      = AF_INET;
    socketProxyAddress.sin_port        = htons(PORT);
    socketProxyAddress.sin_addr.s_addr = inet_addr(PROXY_IP);

	printf("Listening for new connections in ");
	printf(PROXY_IP);
	printf(":");
	printf("%d", PORT);
	printf("\n");
} 

void bindSocketProxy(){
	if( bind(socketProxy ,(struct sockaddr *)&socketProxyAddress , sizeof(socketProxyAddress)) == SOCKET_ERROR) {
		exit(EXIT_FAILURE);
	}
}

int indexOf(char *string, char e){
	char *a = strchr(string, e);
	return (int)(a - string);
}

void listenSocketProxy(){
	SOCKET newClient;
	struct sockaddr_in newClientAddress;
	int newClientSize = sizeof(struct sockaddr_in);




	




	FILE    *textfile;
    char    line[1000];
     
    textfile = fopen("Server/server_config.txt", "r");
    while(fgets(line, 1000, textfile)){
		int init = indexOf(line, '=');
		int finale = indexOf(line, '\n');
		

        char subbuff[finale-init];
		memcpy( subbuff, &line[init + 1], finale );
		subbuff[finale] = '\0';
		printf("%d", atoi(subbuff));
    }
     
    fclose(textfile);




	//Listen to incoming connections
	listen(socketProxy , MAX_CONNECTIONS);

	while(1) {
		// Looking for new connections
		if((newClient = accept(socketProxy , (struct sockaddr *)&newClientAddress, &newClientSize)) != INVALID_SOCKET ) {

			// New client has arrived
			char client_petition[PETITION_LEN];
			if (recv(newClient , client_petition , PETITION_LEN , 0) != SOCKET_ERROR) {

				// Receiving client's petition
				receiveRequest(newClient, client_petition);
				closesocket(newClient);
			}
		}
	}

	closesocket(socketProxy);
	WSACleanup();
}

int main(int argc, char *argv[]) {
	initSocketProxy();
	bindSocketProxy();
	listenSocketProxy();
    
	return 0;
}