#pragma once

#include "TcpServer.h"
#include "TcpSocket.h"
#include "JsonHelper.h"
#include "RespondFactory.h"
#include "RequestFactory.h"
#include "Codec.h"
#include "Message.pb.h"
#include "DBUtils.h"
#include "SecKeyShm.h"

#include <json/json.h>
#include <pthread.h>
#include <string>
#include <map>

using namespace std;


struct ServerInfo
{
	string ServerID;
	unsigned short Port;
};


//处理客户端请求
class ServerOP
{
public:

	enum KeyLen {Len16=32, Len24=24, Len32=32};

	ServerOP(string jsonFile);
	~ServerOP();

	//启动服务器
	void startServer();

	//密钥协商
	string seckeyAgree(RequestMsg* reqMsg);
	

	//服务器工作函数
	friend void* workHard(void* arg);

private:
	string getRandKey(KeyLen len);
	 
private:
	ServerInfo m_info;

	ConnInfo db_info;
	DBUtils *db_utils = NULL;

	SecKeyShm* m_shm = NULL;

	//每个线程都对应一个TcpSocket
	map<pthread_t, TcpSocket*> m_map;

	TcpServer* m_server = NULL;
};

void* workHard(void* arg);