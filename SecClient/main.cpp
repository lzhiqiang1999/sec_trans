#include <iostream>

#include "ClientOp.h"

using namespace std;

//测试一下

int show();

int main()
{
	//客户端的功能
	ClientOP op("client_config.json");

	while (1)
	{
		int choice = show();
		
		switch (choice)
		{

			case 0:
				//退出系统 
				printf("欢迎下次使用...\n");
				break;
			case 1:
				//密钥协商
				op.seckeyAgree();
				break;
			case 2:
				//密钥校验
				break;
			case 3:
				//密钥注销
				break;
			case 4:
				//密钥查看
				break;
			default:
				break;
		}
	}

	cout << "客户端退出, 欢迎下次使用..." << endl;

    getchar();
    return 0;
}

//展示界面
int show()
{
	int nSel = -1;
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n  /*     1.密钥协商                                            */");
	printf("\n  /*     2.密钥校验                                            */");
	printf("\n  /*     3.密钥注销                                            */");
	printf("\n  /*     4.密钥查看                                            */");
	printf("\n  /*     0.退出系统                                            */");
	printf("\n  /*************************************************************/");
	printf("\n  /*************************************************************/");
	printf("\n\n  选择:");

	scanf("%d", &nSel);
	while (getchar() != '\n');

	return nSel; 

}