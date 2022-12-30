#include <iostream>
#include "RespondCodec.h"

//空对象
RespondCodec::RespondCodec()
{
}

//参数：编码后的字符串，构造出对象，用于 解码
RespondCodec::RespondCodec(string encStr)
{
	initMessage(encStr);
}

//参数：RequestMsg信息，构造出对象，用于 编码
RespondCodec::RespondCodec(RespondInfo* info)
{
	initMessage(info);
}

RespondCodec::~RespondCodec()
{
}

// init函数给空构造准备
// 解码使用
void RespondCodec::initMessage(string encStr)
{
	m_encStr = encStr;
}

//编码使用
void RespondCodec::initMessage(RespondInfo* info)
{
	m_msg.set_clientid(info->clientID);
	m_msg.set_data(info->data);
	m_msg.set_serverid(info->serverID);
	m_msg.set_seckeyid(info->seckeyID);
	m_msg.set_status(info->status);
}


//序列化函数，返回序列化字符串
string RespondCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

//反序列函数，返回结构体/类对象 
void* RespondCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}