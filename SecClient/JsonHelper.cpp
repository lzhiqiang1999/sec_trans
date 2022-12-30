#include "JsonHelper.h"

#include <fstream>

JsonHelper::JsonHelper()
{
}

JsonHelper::~JsonHelper()
{
}

//json文件解析  
Value JsonHelper::filetoJson(string jsonFile)
{
	ifstream ifs(jsonFile);

	Reader r;

	Value root;
	r.parse(ifs, root);

	return root;
}

//保存json文件
void JsonHelper::jsonToFile(string jsonFile, const Value& root)
{
	string str_json = root.toStyledString();

	ofstream  ofs(jsonFile);

	ofs << str_json;
}