#pragma once

namespace PM3D
{

	enum INFODISPO_TYPE
	{
		ADAPTATEUR_COURANT
	};

class CInfoDispositif
{
public:

	CInfoDispositif( int NoAdaptateur );
	CInfoDispositif::CInfoDispositif(DXGI_MODE_DESC modeDesc );

	~CInfoDispositif(void);

	void GetDesc(DXGI_MODE_DESC& modeDesc){modeDesc = mode;}


protected:
	CInfoDispositif(void);

	bool valide;
	int largeur;
	int hauteur;
	int memoire;
	wchar_t nomcarte[100];
	DXGI_MODE_DESC mode;


};

}