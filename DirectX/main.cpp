#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)//프로그램 시작
{	
	SystemClass* System;
	bool result;

	System = new SystemClass();
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();//시스템을 초기화
	if (result)
	{
		System->Run();//런, 안에서 루프를 돌며 작업을 수행(런에서 블락)
	}

	System->Shutdown();//메모리 정리
	delete System;
	System = 0;

	return 0;
}