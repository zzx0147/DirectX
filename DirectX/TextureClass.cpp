#include "TextureClass.h"
#include "UtilMacro.h"


TextureClass::TextureClass()
{
	m_texture = nullptr;
}

TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device,WCHAR* filename)
{
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(device, filename,NULL,NULL,&m_texture,NULL);
	
	isValidReturn(FAILED(result), false);
	
	return true;
}

void TextureClass::Shutdown()
{
	isValidRelease(m_texture);
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
