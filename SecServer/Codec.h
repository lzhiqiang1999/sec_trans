#pragma once
#include <iostream>

class Codec
{ 
public:
	Codec();
	//���������� --> ��̬��delete����ָ��ʱ������Ҳ�ᱻ�ͷ�
	virtual ~Codec();
	virtual std::string encodeMsg();
	virtual void* decodeMsg();
};
