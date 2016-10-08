
#pragma once
using namespace DirectX;
class CSommetBloc
{
public:
	CSommetBloc(){};
	CSommetBloc(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _coordTex = XMFLOAT2(0,0));
	

public:
	static UINT numElements;
	static D3D11_INPUT_ELEMENT_DESC layout[];

protected:
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 coordTex;
};