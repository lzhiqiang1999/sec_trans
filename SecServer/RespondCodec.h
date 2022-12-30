#pragma once
#include <iostream>
#include "Codec.h"
#include "Message.pb.h"
using namespace std;

struct RespondInfo
{
	bool status ;
	int seckeyID;
	string clientID;
	string serverID;
	string data;
};

class RespondCodec :
	public Codec
{
public: 
	//空对象
	RespondCodec();

	//参数：编码后的字符串，构造出对象，用于 解码
	RespondCodec(string encStr);

	//参数：RequestMsg信息，构造出对象，用于 编码
	RespondCodec(RespondInfo* info);

	~RespondCodec();

	// init函数给空构造准备
	// 解码使用
	void initMessage(string encStr);

	//编码使用
	void initMessage(RespondInfo* info);

	//重写父类函数
	//序列化函数，返回序列化字符串
	string encodeMsg();
	//反序列函数，返回结构体/类对象 
	void* decodeMsg();


private:
	//序列化后的字符串
	string m_encStr;

	//要序列化的数据都在这个类
	RespondMsg m_msg;
};
