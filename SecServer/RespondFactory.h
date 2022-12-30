#pragma once
#include <iostream>
#include "RespondCodec.h"
#include "CodecFactory.h"
using namespace std;
class RespondFactory :
	public CodecFactory
{
public:
	RespondFactory(string encStr);
	RespondFactory(RespondInfo* info);

	~RespondFactory();

	Codec* createCodec();

private:
	bool m_flag;
	string m_encStr; 
	RespondInfo* m_info;
};

