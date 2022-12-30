#include <iostream>
#include <string>
#include "RequestCodec.h"

using namespace std;

//空对象
RequestCodec::RequestCodec()
{
}

//参数：编码后的字符串，构造出对象，用于 解码
RequestCodec::RequestCodec(string encStr)
{
	initMessage(encStr);
} 

//参数：RequestMsg信息，构造出对象，用于 编码
RequestCodec::RequestCodec(RequestInfo* info)
{
	initMessage(info);
}

RequestCodec::~RequestCodec()
{
}

// init函数给空构造准备
// 解码使用
void RequestCodec::initMessage(string encStr)
{
	m_encStr = encStr;
}

//编码使用
void RequestCodec::initMessage(RequestInfo* info)
{
	m_msg.set_cmdtype(info->cmd);
	m_msg.set_clientid(info->clientID);
	m_msg.set_data(info->data);
	m_msg.set_serverid(info->serverID);
	m_msg.set_sign(info->sign);
}


//序列化函数，返回序列化字符串
string RequestCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

//反序列函数，返回结构体/类对象 
void* RequestCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}