
/************************************************/
/*					INCLUDES					*/
/************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <openssl/sha.h>
#include "StringEncoder.c"

/************************************************/
/*					PROTOTYPE					*/
/************************************************/


/************************************************/
/*						MAIN					*/
/************************************************/
int main(){
  int welcomeSocket, newSocket;
  int valread;
  char buffer[1024];
	char hashBuffer[256];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;


  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8000);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(listen(welcomeSocket,5)==0){
    printf("Listening\n");
  }else{
    printf("Error\n");
  }

while(1){
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
  pid_t pid = fork();
  if(pid==0){

	valread = read(newSocket, buffer, 1024);
	if(valread<0){
		printf("error no string send\n");
	}else{
		printf("%s\n",buffer);

		char *test = "string received\n";
		send(newSocket, test, strlen(test), 0);
	
		//read in the signature
		valread = read(newSocket, hashBuffer, 1024);
		printf("encoded string from client:: %s\n",hashBuffer);
	
		printf("\n");

		unsigned char encode[SHA_DIGEST_LENGTH];
		size_t len = strlen(buffer);
		SHA1(buffer,len,encode);

		char serverSign[256]="";
		strcat(serverSign, stringToEncodedAscii(encode));
		printf("encoded string upon server:: %s\n",serverSign);

		int cmp = strcmp(serverSign, hashBuffer);
		if(cmp == 0){
			char *result = "true";
			send(newSocket, result, strlen(result),0);
		}else{

			char *result = "false";
			send(newSocket, result, strlen(result),0);
		}
	}
  }
	sleep(1);
}
  return 0;
}
