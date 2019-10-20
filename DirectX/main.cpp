#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)//���α׷� ����
{	
	SystemClass* System;
	bool result;

	System = new SystemClass();
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();//�ý����� �ʱ�ȭ
	if (result)
	{
		System->Run();//��, �ȿ��� ������ ���� �۾��� ����(������ ���)
	}

	System->Shutdown();//�޸� ����
	delete System;
	System = 0;

	return 0;
}