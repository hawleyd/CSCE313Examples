#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>


int padding = RSA_PKCS1_PADDING;

RSA * createRSAWithFilename(char * filename, int publicKey)
{
    FILE * fp = fopen(filename,"rb");

    if(fp == NULL)
    {
        printf("Unable to open file %s \n",filename);
        return NULL;    
    }
    RSA *rsa= RSA_new() ;

    if(publicKey)
    {
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
    }

    return rsa;
}

RSA * createRSA(unsigned char * key, int publicKey)
{
    
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create key BIO\n");
        return 0;
    }
    if(publicKey)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA\n");
        return 0;
    }

    return rsa;
}

int publicEncryptWithFilename(unsigned char * data,int data_len, char *filename, unsigned char *encrypted)
{
    RSA * rsa = createRSAWithFilename(filename, 1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

int publicEncrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

int main(int argc, char * argv[]){

 
 unsigned char publicKey[]= "-----BEGIN PUBLIC KEY-----\n"\
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtiPcdrobsSvtzK8Wa5Eu\n"\
"FIfXZlVlj9uMohGelaJK38doA5Q0Wy57qW8RzsGZQgiB4zP8OoktBRPRDKbjoplm\n"\
"nh7jBRXwA4T5PiQn1gu0adVcDwNIHtSK5aFAJYGGMEPmQaxSU1eE4wCKeGkW1vPK\n"\
"jAPPFE6S7n2KAJG8aAbprMkgW/iPKtcsrN/89WHI9vA9tsC+cRWB2ZvfWeayLqvo\n"\
"7XruXuHuxSdSPSjJwEOidDoLS9q5i4kHLtRUBPut14C0J+OqJJ29xY2ZsxR8iP91\n"\
"9GIbZmHLUrobP4uZ0sf08KI6UcSE1tq1O9eoYh7+PivqIhIj/g/A6YgD4v6CKAuG\n"\
"hwIDAQAB\n"\
"-----END PUBLIC KEY-----\n";
 
unsigned char  encrypted[4098]={};
unsigned char plainData[sizeof(argv[2])];

char publicKeyFileName[] = "pubkey.pem";

memcpy(plainData, argv[2], strlen(argv[2]));
//printf("encrypting\n");


//int encrypted_length= publicEncryptWithFilename(plainData,sizeof(argv[2]),publicKeyFileName,encrypted);
int encrypted_length= publicEncrypt(plainData,strlen(argv[2]),publicKey,encrypted);
if(encrypted_length == -1)
{
    printf("Encryption error\n");
    exit(0);
}
printf("Encrypted length =%d\n",encrypted_length);
FILE *fileDesc = fopen(argv[1], "w+");

int bytesRead = fwrite(encrypted, encrypted_length,1, fileDesc);
fclose(fileDesc);
printf("File %s now contains \"", argv[1]);
printf("%s\" encrypted\n", argv[2]);
}