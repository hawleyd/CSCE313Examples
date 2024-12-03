#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int padding = RSA_PKCS1_PADDING;
const int MAX_PASSWORD_LENGTH = 50;
char globalPassword[MAX_PASSWORD_LENGTH];

int pem_password_callback(char *buf, int size, int rwflag, void *userdata) {
    
    int password_len = strlen(globalPassword);

    if (password_len <= 0) {
        return 0; // No password available
    }

    if (size < password_len + 1) {
        return 0; // Buffer is too small
    }

    // Copy the password to the buffer
    strncpy(buf, globalPassword, password_len);
    buf[password_len] = '\0';

    return password_len;
}

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
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,pem_password_callback, NULL);
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
        printf( "Failed to create key BIO");
        return 0;
    }
    if(publicKey)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,pem_password_callback, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA");
    }

    return rsa;
}

int publicEncryptWithFilename(unsigned char * data,int data_len, char *filename, unsigned char *encrypted)
{
    RSA * rsa = createRSAWithFilename(filename, 1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

int privateDecryptWithFilename(unsigned char * enc_data,int data_len, char * filename, unsigned char *decrypted)

{
    RSA * rsa = createRSAWithFilename(filename,0);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}

int publicEncrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

int privateDecrypt(unsigned char * enc_data,int data_len, unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key, 0);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}

int main(int argc, char *argv[]){


 //used with the private key created previously, below
 char passPhrase[] = "hello";


 unsigned char privateKey[]="-----BEGIN RSA PRIVATE KEY-----\n"\
"Proc-Type: 4,ENCRYPTED\n"\
"DEK-Info: AES-128-CBC,EDF1B7C313924B446029CC49887C0A83\n"\
"\n"\
"z5apveHgWOO8RsYIiRAZW2Odb5txkH5W0t0z4y/pVJylvtx64bxOEbyj/3vdYLs2\n"\
"rm/MFhqHYdXDCh5nRozPNFiMFH9FuWkV/oyFnj4azIqjxlugn9bSXNEiU2EHx8hu\n"\
"7STTq0SUv1RGqOM6wEOjWr0pW1vQ+uXxQri0egt9BTdKYeubN+nPNN1TKGah9/aW\n"\
"WxxNm4z1ygyIaSgrAuEfVwJBzYV/KJf25OKWBsq2/jZJgRUl0GH2MQUqKE31/KwS\n"\
"CSiYeH/1BYAMCALbzaoFdNwrd7XEhPGGsAeklVs516zTCMzyGvMZKvKm4ozYwAdG\n"\
"HcyNwrSVtQutGjMOu7L6QsKdqLyCWH1Dbn6S0p63JHBdM5qfrnuIVrflA2GlCVrc\n"\
"9cTFzeSxI3C6MBp4h/V8F1vAT6veTSDtQOhuK4rD3r2E8friEo7F2AUpGR9cp+CB\n"\
"kygFXVINPtA5kM3qOcpyPK7Bf+H6U9PZl4vHp+mj8ZQ1Ww4NuGBO/RoFWEQwDdpx\n"\
"2Bl4zwvPc6QKKgDzkkOSQ5Kr9sEFc3Td2ONI4hAyiOKz2SkBruG4HHBTai7BmEXI\n"\
"ejmB+pxr5WnVVqrnmi3zKJE7aYp4aDACvezaZoeKPzt6AvKNX3aWWDPBo6tZWsjh\n"\
"50oG9bxpYY6IpPpV5f9Qip+I3VnyI86gzGYTl1VT/kjfQdN3NE0wg+eBa901F9Zf\n"\
"bcykqEc78grCcWAMZUqSZL5jh5EBmV3eW2eePirEyPfVGv2GXS8u2GcCxNPZlXZD\n"\
"467LStXPywYPcetrDNw87MmsXFV87UPO12nh/YrXmL75+5JBrG7m++shgdW8wpNj\n"\
"caI7TBXx0MAHakyNSGdWvdod0jZlzqKR+05JLTdCnmsQ41NR6kzK1171QVNBXH4g\n"\
"PRZ6MpBC2KeFoxJIRKNlMQV0deQ0ObzERfpuVdqz0NBFbJocLZpB4lEaqNTWZRKS\n"\
"Pp6SDvFDu/XWvmMTZBbSl6Enfzk10vK2km6EDk2BobkbSZ4hBANa58k/4/6ywTom\n"\
"pnf4yp8AooLs6rlif7LvywvFiF0A0yIEXYsiC0/XkWfjUR0DEROK+RaBHgQEHLN8\n"\
"4dVW27+WA36ZnzDijdHvF6ydIT/BI6zNR1JeAIdsWrm2kot7C1A75uZFL88DADot\n"\
"pRDYeGC6/SXpDgAQfZ2tw9LA5XTZ5riBwFqa0mIkExgGo+70+J3NUXtPLZylDWDJ\n"\
"uhyfip4gyYdTCUSeOgjmIF1tj3Y5nNQHqZquYZ4jiyy8CKCQEcttzhxd21YD5Tyq\n"\
"DgI+Wcw0AMNilgHNmWsVG0qaJ9p7OsY76UxzcUKzBMkR+slgYPXolV6XcbUXQmN3\n"\
"CqAQfy4mRWlku8Vyzz0z2jSVW5ipECuua64fbm9rpYHRpqFWVHLLguYXyVHNVm+G\n"\
"I3sffsP1MUOovHE58kaOPQKn/vBVM/43tZGyrccCvpuUOfpN2nz8QqAmZQT6BdAd\n"\
"ToxJ1D3TkW1/MPPpbyx3dp65+kEWklwR6EY1JICuGpcBrqlSmdkDIq3NV3+F0YLM\n"\
"VDEcwOouSUwoWKyPhBDkd4i5Z3rKNZgbhpFle2JEpGJqimac4y5t9rWx/Ay2ylrf\n"\
"-----END RSA PRIVATE KEY-----\n";
   
unsigned char decrypted[4098]={};

char privateKeyFileName[] = "privkey.pem";
char publicKeyFileName[] = "pubkey.pem";

FILE *fileDesc = fopen(argv[1], "r");
fseek(fileDesc, 0L, SEEK_END);
long fileSize = ftell(fileDesc);
fseek (fileDesc, 0L, SEEK_SET);
unsigned char encrypted[fileSize];

int bytesRead = fread(encrypted, fileSize,2, fileDesc);
fclose(fileDesc);


strcpy(globalPassword, passPhrase);

//int decrypted_length = privateDecryptWithFilename(encrypted,fileSize, privateKeyFileName, decrypted);

int decrypted_length = privateDecrypt(encrypted,fileSize, privateKey, decrypted);
if(decrypted_length == -1)
{
    printf("Decryption error\n");
    exit(0);
}

printf("Decrypted Text from %s = %s\n",argv[1], decrypted);
//printf("Decrypted Length =%d\n",decrypted_length);


}

