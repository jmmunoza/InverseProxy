#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

struct SocketNode {
    SOCKET socket;
    struct SocketNode *next;
};

struct SocketNode* clients;

// Append Socket to the list
void appendSocket(struct SocketNode* listHead, SOCKET *newSocket) {
    // Creating new node
    struct SocketNode* newSocketNode = (struct SocketNode*)malloc(sizeof(struct SocketNode));;
    newSocketNode->socket = *newSocket;


    newSocketNode->next = listHead;
    listHead = newSocketNode;
}

struct SocketNode* initSocketList(SOCKET *socket){
    struct SocketNode* socketList = (struct SocketNode*)malloc(sizeof(struct SocketNode));
    socketList->socket = *socket;
    socketList->next   = NULL;
    return socketList;
}

void printSocketList(struct SocketNode *listHead) {
    struct SocketNode* node = listHead;
    printf("%d", sizeof(node));

    while (node != NULL) {
        printf("fd");
        node = node->next;
    }
}

void initSocket(){
    SOCKET socketTCP = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clients == NULL) {
        // First client
        clients = initSocketList(&socketTCP);
    } else {
        // More clie&&nts
        appendSocket(clients, &socketTCP);
    }

    printSocketList(clients);
}


int main(){
    int a;

    // Starting SOCKETS API
    WSADATA WsaData;
    WORD wVersionRequerida = MAKEWORD (2, 2);
    WSAStartup (wVersionRequerida, &WsaData);

    while (1) {
        scanf("%d", &a);
        initSocket();
    }

    return 0;
}