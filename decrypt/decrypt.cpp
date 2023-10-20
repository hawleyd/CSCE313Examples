#include <iostream>
#include <fstream>
#include <string>
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include <sha.h>
#include <md5.h>

std::string readFileFromEncryptedJSON(const std::string& filePath, const std::string& password) {
    std::string jsonString;

    CryptoPP::SecByteBlock aesKey(CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::SecByteBlock aesIV(CryptoPP::MD5::DIGESTSIZE);

    CryptoPP::SHA256().CalculateDigest(aesKey, reinterpret_cast<const byte*>(password.data()), password.size());
    CryptoPP::MD5().CalculateDigest(aesIV, reinterpret_cast<const byte*>(password.data()), password.size());

    CryptoPP::AES::Encryption aesEncryption(aesKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, aesIV);

    std::ifstream fileStream(filePath, std::ios::binary);
    std::string encryptedData((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    CryptoPP::StringSource(encryptedData, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(jsonString)
        )
    );

    return jsonString;
}

int main() {
    std::string filePath = "test.txt";
    std::string password = "happy";
    std::string decryptedData = readFileFromEncryptedJSON(filePath, password);

    std::cout << "Decrypted JSON data: " << decryptedData << std::endl;

    return 0;
}
