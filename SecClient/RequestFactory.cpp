#include <iostream>
#include "RequestFactory.h"
using namespace std;

//解码
RequestFactory::RequestFactory(string encStr)
{
	m_encStr = encStr;
	m_flag = true;
}

//编码
RequestFactory::RequestFactory(RequestInfo* info)
{
	m_info = info;
	m_flag = false;
}

 
RequestFactory::~RequestFactory()
{
	
}

//返回一个编解码的类
Codec* RequestFactory::createCodec()
{
	//true, 解码
	if (m_flag)
	{
		return new RequestCodec(m_encStr);
	}
	else//false 编码
	{
		return new RequestCodec(m_info);
	}
}