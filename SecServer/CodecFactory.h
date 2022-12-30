#pragma once
#include "Codec.h"

class CodecFactory
{
public:
	CodecFactory();
	~CodecFactory();

	virtual Codec* createCodec();

};
