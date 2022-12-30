#include "ServerOP.h"
//#include "TcpServer.h"
//#include "TcpSocket.h"
//#include "JsonHelper.h"
//#include "RespondFactory.h"
//#include "RequestFactory.h"
//#include "Codec.h"
//#include "Message.pb.h"
//
//#include <json/json.h>
//#include <pthread.h>

#include "RsaCrypto.h"
#include "Hash.h"



#include <ctime>
#include <unistd.h>
#include <fstream>
#include <stdio.h>

ServerOP::ServerOP(string jsonFile)
{
	//读磁盘文件，获取配置信息
	JsonHelper jhelper;
	Value root = jhelper.filetoJson(jsonFile);
	m_info.ServerID = root["ServerID"].asString();
	m_info.Port = root["Port"].asUInt();

	cout << "111111" << endl;

	//数据库信息
	strcpy(db_info.username, root["DB_UserName"].asCString());
	
	strcpy(db_info.password, root["DB_Password"].asCString());
	
	strcpy(db_info.ip, root["DB_IP"].asCString());
	
	strcpy(db_info.db_name, root["DB_Name"].asCString());

	db_info.port=root["DB_Port"].asInt();


	//数据库操作类
	db_utils = new DBUtils();
	int ret = db_utils->connectDB(&db_info);
	if (ret == 0)
	{
		cout << "数据库连接成功..." << endl;
	}
	else
	{
		cout << "数据库连接失败..." << endl;
	}
	

	//加载共享内存配置文件
	string shmKey = root["KeyPath"].asString();
	int maxNode = root["MaxNode"].asUInt();

	m_shm = new SecKeyShm(shmKey, maxNode);

	//初始化互斥锁
	//pthread_mutex_init();
}


ServerOP::~ServerOP()
{
	if (m_server != NULL)
	{
		delete m_server;
	}

	if (db_utils != NULL)
	{
		delete db_utils;
	}

	if (m_shm != NULL)
	{
		delete m_shm;
	}


}

void ServerOP::startServer()
{
	m_server = new TcpServer();

	//设置(socket, listen, bind）
	m_server->setListen(m_info.Port);

	//监听
	while (1)
	{
		cout << "等待客户端连接..." << endl;
		TcpSocket* socket = m_server->acceptConn();
		
		if (socket == NULL)
		{
			continue;
		}

		pthread_t pid;
		//主线程监听，子线程复制 逻辑处理
		pthread_create(&pid, NULL, workHard, this);
		//记录子线程需要处理的 socket通信
		/*
			注意：需要对m_map加一个互斥锁，因为可能子线程取socket时，还没有插入
		*/
		m_map.insert(make_pair(pid, socket)); 
	}
	

	
}

string ServerOP::seckeyAgree(RequestMsg *reqMsg)
{
	//0. 签名验证
	//得到公钥
	ofstream ofs("public.pem");
	ofs << reqMsg->data();//客户端发过来的公钥 测试
	ofs.close();

	RsaCrypto* rsa = new RsaCrypto("public.pem", false);
	//cout << "111111" << endl;sss

	//验证
	RespondInfo resInfo;

	//验证公钥的哈希值
	Hash hash(T_SHA1);
	hash.addData(reqMsg->data());
	
	bool is_verified = rsa->rsaVerify(hash.result(), reqMsg->sign());
	//cout << "222222" << endl;

	if (!is_verified)
	{
		cout << "密钥协商失败..." << endl;
		resInfo.status = false;
	}
	else
	{
		cout << "数字签名验证成功..." << endl;
		//1. 生成随机字符串 对称加密密钥, 该密钥有长度要求，要根据不同的对称加密算法来判断
		//使用aes算法，长度为16，24，32
		string key = getRandKey(Len16);
		//2. 使用客户端公钥加密
		string encKey = rsa->rsaPubKeyEncrypt(key);
		//3. 初始化数据
		
		resInfo.clientID = reqMsg->clientid();
		resInfo.data = encKey;
		resInfo.seckeyID = db_utils->getKeyID();//需要数据库操作
		resInfo.serverID = m_info.ServerID;
		resInfo.status = true;

		//将密钥信息写入数据库
		NodeSeckeyInfo sec_info;
		strcpy(sec_info.clientID, reqMsg->clientid().data());
		strcpy(sec_info.serverID, reqMsg->serverid().data());
		strcpy(sec_info.seckey, key.data());
		sec_info.status = 1;
		sec_info.seckeyID = db_utils->getKeyID();
	
		int ret = db_utils->writeSecKey(&sec_info);
		if (ret == 0)
		{
			//成功
			db_utils->updateKeyID(sec_info.seckeyID + 1);

			//将密钥信息写入共享内存
			m_shm->shmWrite(&sec_info);

		}
		else
		{
			//失败
			resInfo.status = false;
		}

		db_utils->closeDB();
	}

	//4. 序列化
	CodecFactory* factory = new RespondFactory(&resInfo);
	Codec* c = factory->createCodec();
	string encMsg = c->encodeMsg();

	//释放资源
	delete rsa;
	delete factory;
	delete c;

	//5. 发送信息
	return encMsg;
	
}

string ServerOP::getRandKey(KeyLen len)
{

	//设置随机数种子，保证每次启动服务器得到的都是不同的 随机数
	srand(time(NULL));
	
	//生成小写字母，大写字母，0-9数字，特殊字符
	int flag = 0;
	char* chs = "~!@#$%^&*()_+|\{}[]'";
	string randKey;
	for (int i = 0; i < len; i++)
	{
		flag = rand() % 4;
		switch (flag)
		{
			case 0://生成小写
				randKey.append(1, 'a' + rand() % 26);
				break;
			case 1:
				randKey.append(1, 'A' + rand() % 26);
				break;
			case 2:
				randKey.append(1, '0' + rand() % 10);
				break;
			case 3:
				randKey.append(1, chs[rand()%strlen(chs)]);
				break;
			default:
				break;
		}
	}

	return randKey;
}

void* workHard(void* arg)
{
	//让父线程先执行
	sleep(1);

	string data;

	ServerOP* op = (ServerOP*)arg;
	map<pthread_t, TcpSocket*> m_map = op->m_map;
	TcpSocket* socket = m_map[pthread_self()];

	//1.接收客户端消息
	string str_recv = socket->recvMsg();
	//2.反序列化
	CodecFactory *factory = new RequestFactory(str_recv);
	Codec *c = factory->createCodec();
	RequestMsg *reqMsg = (RequestMsg*)c->decodeMsg();
	
	int cmdType = reqMsg->cmdtype();
	
	//3.根据客户端的请求类型做相应发逻辑
	switch (cmdType)
	{
		case 1:
			//密钥协商
			data = op->seckeyAgree(reqMsg);
			break;
		case 2:
			//密钥
		case 3:
			////密钥注销
		default:
			break;
	}

	//发送数据
	socket->sendMsg(data);

	//释放资源
	delete factory;
	delete c;

	//释放socket
	socket->disConnect();
	m_map.erase(pthread_self());
	delete socket;

	return NULL;
}
