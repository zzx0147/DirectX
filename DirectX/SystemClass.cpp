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

	InitializeWindows(screenWidth, screenHeight);//������ �ʱ�ȭ

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
	WNDCLASSEX wc;//������ ����ü
	DEVMODE dmScreenSettings;//��ǻ�� ��ġ�� ������ �����ϴ� ����ü(ex ������, ����� ��) ���⼭�� ��ũ�� ���ÿ� ���
	int posX, posY;//�������� ���� �� ��ǥ

	ApplicationHandle = this;//SystemClass Ÿ���� ����ƽ ������ �ڽ��� ����(�����ڸ� ������ �ʾ����� �̱��� ���� ���)

	m_hinstance = GetModuleHandle(NULL);//���� ���μ����� hinstance�� �����´�, ������ ���ڷ� ���� �̸��� �´� exe�Ǵ� dll�� HMODULE�� ��ȯ

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;//�������� ��Ÿ���� ����, ���ʺ���, ���� ũ�� ����� �ٽ� ȭ�� �׸�, ���� ũ�� ����� �ٽ� ȭ�� �׸�, �����쿡�� �� ���α׷����� ���� DC�� �����
	wc.lpfnWndProc = WndProc;//������ �ݹ� �޼����� �޴� �Լ��� �����Ѵ�
	wc.cbClsExtra = 0;//������� ������� ����
	wc.cbWndExtra = 0;//������� ������� ����
	wc.hInstance = m_hinstance;//���� ���α׷��� hinstance ����
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);//������ ����
	wc.hIconSm = wc.hIcon;//���� ������ ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//Ŀ�� ����
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//��׶��� �� ����//������ ���̷�Ʈx���� �ٽ� �׸��ϱ� ���ǹ�
	wc.lpszMenuName = NULL;//���α׷��� ����ϴ� �޴� ����(�����Ƿ� NULL)
	wc.lpszClassName = m_applicationName;//������ �̸� ����
	wc.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX�� ����� ����(WNDCLASS,WNDCLASSEX�� �����ϱ� ������ ��)

	RegisterClassEx(&wc);//������ ���

	screenWidth = GetSystemMetrics(SM_CXSCREEN);//���� ��ǻ���� ȭ�� ũ�⸦ ���ؿ�
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)//Ǯ��ũ�� ������ �������� ���
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));//�ʱ�ȭ
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);//������ ����
		dmScreenSettings.dmPanningWidth = (unsigned long)screenWidth;//��������󿡼� ȭ�� �ػ� ����(���α׷� �ػ󵵸� �����ϸ鼭 ��üȭ������ ����� ����)
		dmScreenSettings.dmPanningHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;//bit per pixel �ȼ��� �p ��Ʈ�� ���� �������� ����??
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);//���÷��� ������ ������

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;//ȭ�� ũ�⸦ 800x600���� ����
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;//�����찡 ȭ�� �� �߾ӿ� ������ ��
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}


	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);//������ ����
	
	
	ShowWindow(m_hwnd, SW_SHOW);//�����츦 ȭ�鿡 ���̰� ��
	SetForegroundWindow(m_hwnd);//�����츦 �ֻ����� �ø�
	SetFocus(m_hwnd);//�� �����쿡 ��Ŀ���� ��

	ShowCursor(false);//Ŀ���� �Ⱥ��̰� ��

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
