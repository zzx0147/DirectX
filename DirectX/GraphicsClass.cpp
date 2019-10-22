#include "GraphicsClass.h"
#include "UtilMacro.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_TextureShader = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass;//D3DClass »ı¼º
	isNotValidReturn(m_D3D, false);

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED,hwnd,FULL_SCREEN,SCREEN_DEPTH,SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3d", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	isNotValidReturn(m_Camera, false);


	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_Model = new ModelClass;
	isNotValidReturn(m_Model, false);

	result = m_Model->Initialize(m_D3D->GetDevice(),L"../DirectX/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	isNotValidReturn(m_TextureShader,false);

	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	isValidShutdown(m_TextureShader);
	isValidShutdown(m_Model);
	isValidShutdown(m_D3D);
	isValidDelete(m_Camera);

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	isNotValidReturn(result, false);

	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetPorjectionMatrix(projectionMatrix);

	m_Model->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,m_Model->GetTexture());
	isNotValidReturn(result,false);

	m_D3D->EndScene();
	return true;
}
