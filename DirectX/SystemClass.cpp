#include "SystemClass.h"
#include "UtilMacro.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);//윈도우 초기화

	m_Input = new InputClass;
	isNotValidReturn(m_Input, false);

	m_Input->Initialize();

	m_Graphics = new GraphicsClass();
	isNotValidReturn(m_Graphics, false);

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	isNotValidReturn(result, false);

	return true;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

void SystemClass::Shutdown()
{
	isValidShutdown(m_Graphics);

	isValidDelete(m_Input);

	ShutdownWindows();

	return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		m_Input->KeyDown((unsigned int)wparam);
		return 0;

	case WM_KEYUP:
		m_Input->KeyUp((unsigned int)wparam);
		return 0;

	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

bool SystemClass::Frame()
{
	bool result;

	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;//윈도우 구조체
	DEVMODE dmScreenSettings;//컴퓨터 장치의 정보를 저장하는 구조체(ex 프린터, 모니터 등) 여기서는 스크린 세팅에 사용
	int posX, posY;//윈도우의 왼쪽 위 좌표

	ApplicationHandle = this;//SystemClass 타입의 스태틱 변수에 자신을 저장(생성자를 막지는 않았지만 싱글톤 같은 사용)

	m_hinstance = GetModuleHandle(NULL);//현재 프로세스의 hinstance를 가져온다, 본래는 인자로 들어온 이름에 맞는 exe또는 dll의 HMODULE을 반환

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;//윈도우의 스타일을 설정, 왼쪽부터, 가로 크기 변경시 다시 화면 그림, 세로 크기 변경시 다시 화면 그림, 윈도우에서 이 프로그램만을 위한 DC를 만든다
	wc.lpfnWndProc = WndProc;//윈도우 콜백 메세지를 받는 함수를 지정한다
	wc.cbClsExtra = 0;//예약공간 사용하지 않음
	wc.cbWndExtra = 0;//예약공간 사용하지 않음
	wc.hInstance = m_hinstance;//현재 프로그램의 hinstance 설정
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);//아이콘 설정
	wc.hIconSm = wc.hIcon;//작은 아이콘 설정
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//커서 설정
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//백그라운드 색 결정//어차피 다이렉트x에서 다시 그리니까 무의미
	wc.lpszMenuName = NULL;//프로그램이 사용하는 메뉴 지정(없으므로 NULL)
	wc.lpszClassName = m_applicationName;//윈도우 이름 설정
	wc.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX의 사이즈를 저장(WNDCLASS,WNDCLASSEX를 구분하기 위함인 듯)

	RegisterClassEx(&wc);//윈도우 등록

	screenWidth = GetSystemMetrics(SM_CXSCREEN);//현재 컴퓨터의 화면 크기를 구해옴
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)//풀스크린 설정이 켜져있을 경우
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));//초기화
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);//사이즈 저장
		dmScreenSettings.dmPanningWidth = (unsigned long)screenWidth;//윈도우즈상에서 화면 해상도 변경(프로그램 해상도를 유지하면서 전체화면으로 만들기 위해)
		dmScreenSettings.dmPanningHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;//bit per pixel 픽셀당 몆 비트의 값을 가지는지 설정??
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);//디스플레이 세팅을 변경함

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;//화면 크기를 800x600으로 설정
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;//윈도우가 화면 정 중앙에 오도록 함
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}


	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);//윈도우 생성
	
	
	ShowWindow(m_hwnd, SW_SHOW);//윈도우를 화면에 보이게 함
	SetForegroundWindow(m_hwnd);//윈도우를 최상위로 올림
	SetFocus(m_hwnd);//이 윈도우에 포커싱을 함

	ShowCursor(false);//커서를 안보이게 함

	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
