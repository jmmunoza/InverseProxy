// Add to linker "-lws2_32" to execute winsock2.h
#define  N_WEB_SERVERS 3

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../Util/constants.c"
#include "log.c"
#include "cache.c"

// Proxy
struct sockaddr_in socketProxyAddress;
SOCKET socketProxy;

// AWS
int   serverInUse = 0;
char  serversIP[N_WEB_SERVERS][20];
int   serversPort[N_WEB_SERVERS];

int indexOf(char *string, char *e){
	char *a = strstr(string, e);
	return (int)(a - string);
}

void sendResponse(SOCKET client, char *response){
	send(client , response , strlen(response) , 0);
}

void receiveRequest(SOCKET client, char *petition){
	if(serverInUse == N_WEB_SERVERS){
		serverInUse = 0;
	}

	// Connect to WEBAPP

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serverAddress;
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_port        = htons(serversPort[serverInUse]);
    serverAddress.sin_addr.s_addr = inet_addr(serversIP[serverInUse]);

	appendLog(petition);
	if (connect(serverSocket , (struct sockaddr *)&serverAddress , sizeof(serverAddress)) >= 0) {
		// Sending a message
		if(send(serverSocket , petition , strlen(petition) , 0) >= 0) {
			char server_reply[RESPONSE_LEN]; 
			int recv_size;
			if ((recv_size = recv(serverSocket , server_reply , RESPONSE_LEN, 0)) != SOCKET_ERROR) {
				server_reply[recv_size] = '\0';
				appendLog(server_reply);
				sendResponse(client, server_reply);
				
				if ((recv_size = recv(serverSocket , server_reply , RESPONSE_LEN, 0)) != SOCKET_ERROR) {
					server_reply[recv_size] = '\0';
					appendLog(server_reply);
					
					// Cache ----
					saveResponse(petition, server_reply);
					
					// Cache ----
					sendResponse(client, server_reply);
				}
			}
			
		}
	} else {
		// Response is in cache
		char *responseInCache = getResponse(petition);
		sendResponse(client, "HTTP/1.1 200 OK\t\n");
		appendLog("HTTP/1.1 200 OK\t\n");
		sendResponse(client, responseInCache);
		appendLog(responseInCache);
	}
	
	closesocket(serverSocket);
	checkCacheTimeout();
	serverInUse += 1;
}

void initSocketProxy(){
	// Starting SOCKETS API
	WSADATA WsaData;
	WORD wVersionRequerida = MAKEWORD (2, 2);
	WSAStartup (wVersionRequerida, &WsaData);

	// Reading settings file
	FILE    *textfile;
    char    line[20];
	int     lineNum = 0;
     
    textfile = fopen("Server/server_config.txt", "r");
    while(fgets(line, 20, textfile)){
		// Getting IP and port
		line[strlen(line) - 1] = '\0';
		char IPaddress[20];
		strcpy(IPaddress, line);

		fgets(line, 20, textfile);
		line[strlen(line) - 1] = '\0';
		int port;
		sscanf(line, "%d", &port);

		if(lineNum == 0) { // Proxy address and port
			// TCP Socket
			socketProxy = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

			// Socket address
			socketProxyAddress.sin_family      = AF_INET;
			socketProxyAddress.sin_port        = htons(port);
			socketProxyAddress.sin_addr.s_addr = inet_addr(IPaddress);

			printf("Listening for new connections in ");
			printf(IPaddress);
			printf(":");
			printf("%d", port);
			printf("\n");
		} else {           // AWS servers

			strcpy(serversIP[lineNum - 1], IPaddress);
			serversPort[lineNum - 1] = port;
		}

		lineNum += 1;
    }
     
    fclose(textfile);	
} 

void bindSocketProxy(){
	if( bind(socketProxy ,(struct sockaddr *)&socketProxyAddress , sizeof(socketProxyAddress)) == SOCKET_ERROR) {
		exit(EXIT_FAILURE);
	}
}

void listenSocketProxy(){
	//Listen to incoming connections
	listen(socketProxy , MAX_CONNECTIONS);

	while(1) {
		SOCKET newClient;
		struct sockaddr_in newClientAddress;
		int newClientSize = sizeof(struct sockaddr_in);
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
	// Creating the log if it doesn't exist
	createLogFile();

	// Creating the cache if it doesn't exist
	clearCache();

	// Setting the socket
	initSocketProxy();
	bindSocketProxy();
	listenSocketProxy();
    
	return 0;
}