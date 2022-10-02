// Add to linker "-lws2_32" to execute winsock2.h

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../Consts/constants.c"

int main(int argc, char *argv[]) {
	
	// Starting SOCKETS API
	WSADATA WsaData;
	WORD wVersionRequerida = MAKEWORD (2, 2);
	WSAStartup (wVersionRequerida, &WsaData);

	// TCP Socket
	SOCKET socketTCP = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Socket address
	struct sockaddr_in socketTCPAddress;
    socketTCPAddress.sin_family      = AF_INET;
    socketTCPAddress.sin_port        = htons(PORT);
    socketTCPAddress.sin_addr.s_addr = inet_addr(PROXY_IP);
    
    printf(PROXY_IP);
    
    // Connecting to the Socket
    if (connect(socketTCP , (struct sockaddr *)&socketTCPAddress , sizeof(socketTCPAddress)) < 0) {
        printf("Connect error \n");
        return 1;
    }
    
    printf("Connected \n");
    
	return 0;
}
