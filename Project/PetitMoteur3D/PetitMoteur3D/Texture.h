#pragma once


namespace PM3D
{

class CTexture
{
public:
	CTexture(void);
	virtual ~CTexture(void);

	CTexture(const wchar_t* filename_in,CDispositifD3D11* pDispositif);

	const wchar_t* GetFilename() {return filename;}

	ID3D11ShaderResourceView* GetD3DTexture() {return pTexture;}


protected:
	wchar_t filename[60];
	ID3D11ShaderResourceView* pTexture;

};

}