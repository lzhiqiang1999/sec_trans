#pragma once

#include <string>
#include <json/json.h>
using namespace std;
using namespace Json;

class JsonHelper
{
public:
	JsonHelper();
	~JsonHelper();

	//解析json文件 
	Value filetoJson(string jsonFile);

	//保存json格式到文件
	void jsonToFile(string jsonFile, const Value& root);


private:

};
