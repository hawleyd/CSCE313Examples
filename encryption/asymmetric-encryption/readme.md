command used to create private key
openssl genrsa -aes128 -passout pass:hello -out privkey.pem 2048

command used to create public key
openssl rsa -in privkey.pem -passin pass:hello -pubout > pubkey.pem

compile with
g++ -o encrypt encrypt.cpp -lssl -lcrypto
and
g++ -o decrypt decrypt.cpp -lssl -lcrypto

test with
./encrypt encryptedFile.txt "enter some cool text here"
./decrypt encryptedFile.txt
