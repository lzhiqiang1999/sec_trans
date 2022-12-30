#include <iostream>
#include "RespondCodec.h"

//�ն���
RespondCodec::RespondCodec()
{
}

//�������������ַ������������������ ����
RespondCodec::RespondCodec(string encStr)
{
	initMessage(encStr);
}

//������RequestMsg��Ϣ��������������� ����
RespondCodec::RespondCodec(RespondInfo* info)
{
	initMessage(info);
}

RespondCodec::~RespondCodec()
{
}

// init�������չ���׼��
// ����ʹ��
void RespondCodec::initMessage(string encStr)
{
	m_encStr = encStr;
}

//����ʹ��
void RespondCodec::initMessage(RespondInfo* info)
{
	m_msg.set_clientid(info->clientID);
	m_msg.set_data(info->data);
	m_msg.set_serverid(info->serverID);
	m_msg.set_seckeyid(info->seckeyID);
	m_msg.set_status(info->status);
}


//���л��������������л��ַ���
string RespondCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

//�����к��������ؽṹ��/����� 
void* RespondCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}