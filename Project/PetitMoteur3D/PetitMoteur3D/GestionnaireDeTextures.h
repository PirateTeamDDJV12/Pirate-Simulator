#pragma once
#include <vector>
#include "texture.h"

namespace PM3D
{

class CGestionnaireDeTextures
{
public:
	CGestionnaireDeTextures(void);
	~CGestionnaireDeTextures(void);
	CTexture* const GetNewTexture(const wchar_t* filename_in);
	CTexture* const GetTexture(const wchar_t* filename_in);
	void Cleanup();

protected:
	// Le tableau de textures
	std::vector<CTexture*> ListeTextures;

};

}


