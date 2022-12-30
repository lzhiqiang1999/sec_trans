#include "ClientOp.h"
#include "JsonHelper.h"
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "RsaCrypto.h"
#include "TcpSocket.h"
#include "Message.pb.h"
#include "Hash.h"


#include <sstream>
#include <fstream>
#include <json/json.h>



ClientOP::ClientOP(string jsonFile)
{
	//解析json配置文件
	JsonHelper jhelper;
	Value root = jhelper.filetoJson(jsonFile);
	
	m_info.ClientID = root["ClientID"].asString();
	m_info.ServerID = root["ServerID"].asString();
	m_info.IP = root["IP"].asString();
	m_info.Port = root["Port"].asUInt();

	//加载共享内存配置文件
	string shmKey = root["KeyPath"].asString();
	int maxNode = root["MaxNode"].asUInt();

	m_shm = new SecKeyShm(shmKey, maxNode);
}

ClientOP::~ClientOP()
{
	if (m_shm != NULL)
	{
		delete m_shm;
	}
}

/*
	密钥协商 测试
*/
bool ClientOP::seckeyAgree()
{
//0. 生成密钥对，用于密钥交换
	RsaCrypto * rsaCrypto = new RsaCrypto();
	rsaCrypto->generateRsakey(1024);
	
	//从文件中读取公钥
	ifstream ifs("public.pem");
	stringstream str;
	str << ifs.rdbuf();
	ifs.close();
	
//1. 初始化信息
	//编码的类
	RequestInfo info;
	info.clientID = m_info.ClientID;
	info.serverID = m_info.ServerID;
	info.cmd = 1;//密钥协商
	info.data = str.str();//非对称加密的公钥
	//cout << "公钥：" << info.data << endl;

	//先对公钥进行哈希
	Hash hash(T_SHA1);
	hash.addData(str.str());
	info.sign = rsaCrypto->rsaSign(hash.result());//用私钥对公钥的hash签名
	
	//序列化
	CodecFactory* factory = new RequestFactory(&info);
	Codec* c = factory->createCodec();
	string encMsg = c->encodeMsg();

	//释放内存
	delete c;
	delete factory;


//3. 套接字通信 当前是客户端
	TcpSocket* socket = new TcpSocket();
	//3.1 连接服务器
	int ret = socket->connectToHost(m_info.IP, m_info.Port);
	if (ret != 0)
	{
		cout << "服务器连接失败..." << endl;
		return false;
	}
	cout << "服务器连接成功..." << endl;
	//3.2 发送数据
	ret = socket->sendMsg(encMsg);
	if (ret != 0)
	{
		cout << "数据发送失败..." << endl;
	}
	cout << "数据发送成功..." << endl;

	//3.3 接收数据
	string str_recv = socket->recvMsg();
	if (str_recv.empty())
	{
		cout << "数据接收失败..." << endl;
	}
	cout << "数据接收成功..." << endl;

	//3.4 反序列化
	factory = new RespondFactory(str_recv);
	c = factory->createCodec();
	RespondMsg *resMsg = (RespondMsg*)c->decodeMsg();
	
	//查看服务器返回的信息
	if (!resMsg->status())//状态为false
	{
		cout << "密钥协商失败..." << endl;
		return false;
	}

	//3.5 获取服务器对称加密公钥，用私钥解密
	string key = rsaCrypto->rsaPriKeyDecrypt(resMsg->data());
	
	cout << "服务器对称加密的密钥：" << key << endl;

	//将服务器发送来的公钥写入共享内存
	NodeSHMInfo shm_info;
	strcpy(shm_info.clientID, m_info.ClientID.data());
	strcpy(shm_info.serverID, m_info.ServerID.data());
	strcpy(shm_info.seckey, key.data());
	shm_info.seckeyID = resMsg->seckeyid();
	shm_info.status = true;

	m_shm->shmWrite(&shm_info);

	//释放资源
	delete c;
	delete factory;
	delete rsaCrypto;

	//短连接，不需要频繁通信
	socket->disConnect();
	delete socket;

	return true;
}

void ClientOP::seckeyCheck()
{
}

void ClientOP::seckeyZhuXiao()
{
}
