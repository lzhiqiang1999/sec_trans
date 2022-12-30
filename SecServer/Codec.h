#pragma once
#include <iostream>

class Codec
{ 
public:
	Codec();
	//虚析构函数 --> 多态，delete父类指针时，子类也会被释放
	virtual ~Codec();
	virtual std::string encodeMsg();
	virtual void* decodeMsg();
};
