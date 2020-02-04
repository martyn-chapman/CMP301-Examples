#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class TextureShader : public BaseShader
{
public:
	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture0, ID3D11ShaderResourceView* texture1);
	void loadBlendedPixelShader(const wchar_t* filename);
	void renderBlend(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);
	void initShader(const wchar_t* vsFilename, const wchar_t* psFilename, const wchar_t* psFilename_blend);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11PixelShader* blendedPixelShader;
};

