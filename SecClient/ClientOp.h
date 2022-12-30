#pragma once

#include <iostream>
#include <string>
#include "SecKeyShm.h"

using namespace std;

struct ClientInfo
{
	string ClientID;
	string ServerID;
	string IP;
	unsigned short Port;
};

class ClientOP
{
public: 
	ClientOP(string jsonFile);
	~ClientOP();

	// 秘钥协商 
	bool seckeyAgree();

	// 秘钥校验
	void seckeyCheck();

	// 秘钥注销
	void seckeyZhuXiao();

private:
	ClientInfo m_info;

	SecKeyShm* m_shm = NULL;
	
};
