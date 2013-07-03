//
//  Sim_AES.h
//  SimRsa
//
//  Created by sim life on 12-1-6.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef SimRsa_Sim_AES_h
#define SimRsa_Sim_AES_h

#include "openssl/aes.h"

//#define OPENSSL_AES_BUFFBLOCK 16

const int AES_bits = 128;


class Sim_AES
{
public:
    Sim_AES();
    Sim_AES(char* key);
    ~Sim_AES();
    void setKey(char* key);
    void Encrypt(const char* fileInPath,const char* fileOutPath);
    void Decrypt(const char* fileInPath,const char* fileOutPath);
    void EncryptString(const char* plainText, char* cipherText);
    void DecryptString(const char* cipherText, char* plainText);
    static void RemoveFile(char* path);
private:
    void SetEncryptKey(); // 设置AES加密密钥    
    void SetDecryptKey(); // 设置AES解密密钥
    
    char* my_encrypt(char *str,AES_KEY* key);
    char* my_decrypt(char *str,AES_KEY* key);
    unsigned char AESKey[AES_BLOCK_SIZE];
    unsigned char AESKey_Backup[AES_BLOCK_SIZE];
    AES_KEY m_key;                              // AES密钥
    int datalen;
    

};



#endif
