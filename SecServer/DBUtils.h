#pragma once

#include <iostream>
#include <string>
#include "NodeSeckeyInfo.h"
#include <mysql/mysql.h>

using namespace std;

struct ConnInfo
{
	char username[20];
	char password[20];
	int port;
	char ip[20];
	char db_name[20];
};

class DBUtils
{

public:
	DBUtils();
	~DBUtils();

	int connectDB(ConnInfo *info);
	
	//获取KeyID
	int getKeyID();

	int getKeyIDByClientIDAndServerID(string clientID, string serverID);
	//更新KeyID
	int updateKeyID(int keyID);
	//添加密钥信息
	int writeSecKey(NodeSeckeyInfo *info);

	void closeDB();

private:
	// 获取当前时间, 并格式化为字符串
	string getCurTime();

private:
	MYSQL m_mysql;    //一个数据库结构体  
};
