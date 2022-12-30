#pragma once
#include <iostream>
#include "RequestCodec.h"
#include "CodecFactory.h"
using namespace std;
class RequestFactory:
	public CodecFactory
{
public:
	RequestFactory(string encStr);
	RequestFactory(RequestInfo* info);

	~RequestFactory();

	Codec* createCodec();

private: 
	bool m_flag;
	string m_encStr;
	RequestInfo* m_info;
};
