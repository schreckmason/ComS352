1. brew update
2. brew install openssl
3. cd /usr/local/include
4. ln -s ../opt/openssl/include/openssl
5. #include <openssl/sha.h>
6. compile like "gcc Client.c -lssl -lcrypto"
7. COMPILER ==== HAPPY

