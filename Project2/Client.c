//FIXME: compile this with -lssl and -lcrypto
/****************************************************/
/*						INCLUDES					*/
/****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "StringEncoder.c"

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
	char s[256];
	printf("enter the string : ");
	scanf("%s",s);

//	size_t length = sizeof(s);
//	SHA1(s, length, hash);
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(s, strlen(s), hash);

	//For debugging -- fixed issue of mismatching hashes for same string input
	PrintHash(hash);//<-- message digest

	stringToEncodedAscii(hash);//<-- message signature
	printf("\n");

//TODO: send original message and the message signature to the server

	return 0;
}
