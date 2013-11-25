//
//  Pt_AES.h
//  PtAES
//
//  Created by xianbei1987 on 13-7-4.
//  Copyright (c) 2013年 xianbei1987. All rights reserved.
//

#ifndef PtAES_Pt_AES_h
#define PtAES_Pt_AES_h

#include <openssl/aes.h>
#include "string.h"
#include <iostream>
using namespace std;

#define MD5_BLOCK_BUF 1024
#define AES_BLOCK_BITS 128

class Pt_AES
{
public:
    static Pt_AES* sharePtAESTool(char* key=NULL);
    Pt_AES();
    Pt_AES(char* key);
    ~Pt_AES();
    void setKey(char* key);
    void EncryptFile(const char* fileInPath,const char* fileOutPath);
    void DecryptFile(const char* fileInPath,const char* fileOutPath);
    void EncryptString(const char* plainText, char* cipherText);
    void DecryptString(const char* cipherText, char* plainText);
    const char* EncryptString(const char* plainText);
    const char* DecryptString(const char* cipherText);
    
    static bool checkFileMD5(const char* fileName,unsigned char* md5Code);
    static bool checkStringMD5(string str,unsigned char* md5Code);
    static const char* getFileMD5(const char* fileName);
    static const char* getStringMD5(const char* strData);
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
