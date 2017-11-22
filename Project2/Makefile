#CFLAGS = -g -Wall -Werror

server:
	gcc Server.c -o server -lssl -lcrypto

client:
	gcc Client.c -o client -lssl -lcrypto

clean:
	rm -rf client *.o *.dSYM
