#include <iostream>
#include "RespondFactory.h"
using namespace std;

RespondFactory::RespondFactory(string encStr)
{
	m_encStr = encStr;
	m_flag = true;
}
RespondFactory::RespondFactory(RespondInfo* info)
{
	m_info = info;
	m_flag = false;
}

RespondFactory::~RespondFactory()
{ 

}

Codec* RespondFactory::createCodec()
{
	//true, 解码
	if (m_flag)
	{
		return new RespondCodec(m_encStr);
	}
	else
	{
		return new RespondCodec(m_info);
	}
}