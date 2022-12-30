#pragma once
#include "BaseShm.h"
#include <string.h>
#include "NodeSeckeyInfo.h"



class SecKeyShm : public BaseShm
{
public:
	SecKeyShm(int key, int maxNode);
	SecKeyShm(string pathName, int maxNode);
	~SecKeyShm();

	void shmInit();
	int shmWrite(NodeSeckeyInfo* pNodeInfo);
	NodeSeckeyInfo shmRead(string clientID, string serverID);

private:
	int m_maxNode;
};

