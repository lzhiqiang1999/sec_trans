#include "DBUtils.h"

DBUtils::DBUtils()
{
	//初始化数据库
	mysql_init(&m_mysql);

	//设置字符集
	mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
}

DBUtils::~DBUtils()
{
}

int DBUtils::connectDB(ConnInfo* info)
{
	char *ip = info->ip;
	char* username = info->username;
	char* password = info->password;
	char* db_name = info->db_name;
	int port = info->port;

	cout << ip << endl;
	cout << username << endl;
	cout << password << endl;
	cout << db_name << endl;
	cout << port << endl;

	if (mysql_real_connect(&m_mysql, ip, username, password, db_name, port, NULL, 0) == NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int DBUtils::getKeyID()
{
	// 查询数据库
	// for update: 临时对数据表加锁
	string sql = "select ikeysn from SEC_TANS_DB.KEYSN for update";
	int ret = mysql_query(&m_mysql, sql.data());
	//获取结果集
	MYSQL_RES* res = mysql_store_result(&m_mysql);
	MYSQL_ROW row;
	int keyID = -1;
	if (row = mysql_fetch_row(res))
	{
		keyID = atoi(row[0]);
	}

	//释放结果集资源
	mysql_free_result(res);

	return keyID;
}

int DBUtils::getKeyIDByClientIDAndServerID(string clientID, string serverID)
{
	return 0;
}

int DBUtils::updateKeyID(int keyID)
{
	// 更新数据库
	string sql = "update SEC_TANS_DB.KEYSN set ikeysn = " + to_string(keyID);

	int ret = mysql_query(&m_mysql, sql.data());
	if (ret < 0)
	{
		cout << "密钥ID更新失败..." << endl;
		return -1;
	}
	else
	{
		cout << "密钥ID更新成功..." << endl;
		return 0;
	}

}

int DBUtils::writeSecKey(NodeSeckeyInfo* info)
{
	char sql[1024] = { 0 };
	sprintf(sql, "Insert Into SEC_TANS_DB.SECKEYINFO(clientid, serverid, keyid, createtime, state, seckey) \
					values ('%s', '%s', %d, '%s' , %d, '%s') ",
		info->clientID, info->serverID, info->seckeyID,
		getCurTime().data(), 1, info->seckey);
	 
	//cout << sql << endl;

	int ret = mysql_query(&m_mysql, sql);
	if (ret)
	{
		cout << "密钥添加失败..." << endl;
		return -1;
	}
	else
	{
		cout << "密钥添加成功..." << endl;
		return 0;
	}	

}

void DBUtils::closeDB()
{
	mysql_close(&m_mysql);
}

string DBUtils::getCurTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));

	return tmp;
}

