//FIXME: compile this with -lssl and -lcrypto
/****************************************************/
/*						INCLUDES					*/
/****************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "StringEncoder.c"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

/****************************************************/
/*						PROTOTYPES					*/
/****************************************************/
void PrintHash(unsigned char *hash){
	if(hash == NULL){
		printf("HASH NULL\n");
		return;
	}
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++){
		printf("%02X", hash[i]);
	}
	printf("\n");
}

/****************************************************/
/*							MAIN					*/
/****************************************************/
int main(){

	//STEPS:
	//1 -- Get user input (scan single line from the terminal)

	//2 -- Hash user input using SHA1
	while(1){
		char s[256];
		printf("enter the string : ");
		fgets(s, 256, stdin);
		char signature[256] = "";

		unsigned char hash[SHA_DIGEST_LENGTH];
		size_t length = strlen(s);
		//printf("length %d\n", length);
		SHA1(s, length,hash);

		//For debugging -- fixed issue of mismatching hashes for same string input
		PrintHash(hash);//<-- message digest

		//stringToEncodedAscii(hash);//<-- message signature
		strcat(signature,stringToEncodedAscii(hash));
		printf("\n");
		//printf("signature :: %s\n",signature);

		int clientSocket;
		char buffer[1024];
		struct sockaddr_in serverAddr;
		socklen_t addr_size;
		int valread;
		clientSocket = socket(PF_INET, SOCK_STREAM, 0);
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(8000);
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
		addr_size = sizeof serverAddr;
		connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
		//SEND THE STRING FIRST
		send(clientSocket, s, strlen(s),0);
		valread = read(clientSocket, buffer, 1024);
		if(valread < 0){
			printf("no response\n");
		}else{
			printf("%s\n",buffer);
			send(clientSocket,signature,strlen(signature),0);
		}
		//read the response
		char matchBuf[256];
		valread = read(clientSocket, matchBuf, 256);
		printf("%s\n",matchBuf);
	}
	return 0;
}
