#include "StdAfx.h"
#include "ChargeurOBJ.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// fonctions de conversion  ref:CodeGURU
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

template <class T>
bool Extraire(std::istringstream& iss, T& f1)
{
  return !(iss >> dec >> f1).fail();
}

template <class T>
bool Extraire(std::istringstream& iss, T& f1, T& f2 )
{
  return !(iss >> dec >> f1 >> dec >> f2).fail();
}

template <class T>
bool Extraire(std::istringstream& iss, T& f1, T& f2, T& f3 )
{
   return !(iss >> dec >> f1 >> dec >> f2 >> dec >> f3).fail();
}

template <class T>
void ExtraireSommet(const std::string& s, T& f1, T& f2, T& f3, bool bTC, bool bN )
{
	std::istringstream iss(s);
	iss >> dec >> f1;
	if ( bTC || bN )
	{
		iss.get();
		if (bTC) iss >> dec >> f2;
		if (bN)
		{
			iss.get();
			iss >> dec >> f3;
		}
	}
}



namespace PM3D
{


CChargeurOBJ::CChargeurOBJ()
{

}

CChargeurOBJ::~CChargeurOBJ(void)
{
	Position.clear();
	CoordTex.clear();
	Normale.clear();
	tabVertexInfo.clear();
	tabIndex.clear();
	materialName.clear();
	SubsetIndex.clear();
	material.clear();
}

void CChargeurOBJ::Chargement( const CParametresChargement& param )
{
	// Les indices de OBJ commencent à 1
	Position.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	CoordTex.push_back(XMFLOAT2(0.0f, 0.0f));
	Normale.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));

	NombreSubset = 0;
	NombreMateriaux = 0;
	bNormales = false;
	bCoordTex = false;
	bInverserCulling = param.bInverserCulling;
	bMainGauche = param.bMainGauche;

	// On suppose que tout est au même endroit 
	cheminModele = param.NomChemin;

	LireFichier(cheminModele + param.NomFichier);

	if (OBJMaterialLib!="")
		LireFichierMateriel();
}

void CChargeurOBJ::GetMaterial(int _i,
					string& _NomFichierTexture,
					string& _NomMateriau,
					XMFLOAT4& _Ambient,
					XMFLOAT4& _Diffuse,
					XMFLOAT4& _Specular,
					float& _Puissance)
{
	_NomFichierTexture = material[_i].NomFichierTexture;
	_NomMateriau = material[_i].NomMateriau;
	_Ambient = material[_i].Ambient;
	_Diffuse = material[_i].Diffuse;
	_Specular = material[_i].Specular;
	_Puissance = material[_i].Puissance;
}

bool CChargeurOBJ::LireFichier(const string& nomFichier)
{
std::ifstream fichier;

	// **** Partie 1 - Lire le fichier en mémoire tampon
	int dimension = 0;
	fichier.open( nomFichier, std::ifstream::in );

	if (!fichier.is_open()) 
		return false;

	fichier.seekg( 0, std::ios::end );
	dimension = (int)fichier.tellg();
	fichier.seekg(0, std::ios::beg);
	if( dimension <= 0 )
		return false;

	char* tampon = new char[dimension];

	if( tampon == 0 )
		return false;

	memset(tampon, '\0', dimension);

	fichier.read(tampon, dimension);

	// **** Partie 2 - Extraire les informations
	string str;
	int position=0;

	while (position<dimension)
	{ 
		// Le (les) caractere(s) de fin de ligne peuvent changer selon les formats
		while ( position<dimension && tampon[position]!='\n' )
		{
			str.push_back(tampon[position]);
			position++;
		}

		str.push_back('\0');
		position++;
	
		if (str.length()>0) TraiterLigneOBJ(str);
		str.clear();
	}

	SubsetIndex.push_back(tabIndex.size());

	fichier.close();
	delete[] tampon;

	return true;
}

bool CChargeurOBJ::LireFichierMateriel()
{
std::ifstream fichier;

	// Partie 1 - Lecture du fichier materiel en tampon
	fichier.open(OBJMaterialLib, std::ifstream::in);
	if (!fichier.is_open()) 
		return false;

	fichier.seekg( 0, std::ios::end );
	int dimension = (int)fichier.tellg();
	fichier.seekg(0, std::ios::beg);
	if( dimension <= 0 )
		return false;

	char* tampon = new char[dimension];

	if( tampon == 0 )
		return false;

	memset(tampon, '\0', dimension);

	fichier.read(tampon, dimension);

	// **** Partie 2 extraire les informations
	string str;
	int position=0;

	while (position<dimension)
	{ 
		// Le (les) caractere(s) de fin de ligne peuvent changer selon les formats
		while ( position<dimension && tampon[position]!='\n' )
		{
			str.push_back(tampon[position]);
			position++;
		}

		str.push_back('\0');
		position++;
	
		if (str.length()>0) TraiterLigneMAT(str);
		str.clear();
	}

	fichier.close();
	delete[] tampon;

	return true;
}

void CChargeurOBJ::TraiterLigneOBJ(const string& str)
{
	OBJVertexInfo faceVertex;
	int nbrSlash = 0;
	string sPoly;   

	char leCar;
	istringstream iss(str);
	
	leCar = iss.get();
	switch (leCar)
	{		
	case '#':					// Commentaires
		// On ne fait rien
		break;

	case 'v':	//  v?
		leCar = iss.get();
		if(leCar == ' ')	  // v - position de sommet
		{
			float x, y, z;
			
			Extraire(iss, x, y, z);

			// Si le modèle est main droite, on inverse le Z
			Position.push_back(XMFLOAT3( x, y, bMainGauche? z : z * -1.0f));
		}

		if(leCar == 't')	//vt - coordonnées d'application de texture
		{			
			float tu, tv;
			
			iss.get(); 
			Extraire(iss, tu, tv);

			// Si le modèle est main droite, on inverse le tv
			CoordTex.push_back(XMFLOAT2(tu, bMainGauche? tv : 1.0f-tv));

			bCoordTex = true;	
		}	

		if(leCar == 'n')	// vn - normale
		{
			XMFLOAT3 v;
			
			iss.get(); 
			Extraire(iss, v.x, v.y, v.z);

			XMVECTOR vt = XMLoadFloat3(&v);
			vt = XMVector3Normalize(vt);
			XMStoreFloat3( &v, vt);

			Normale.push_back(XMFLOAT3( v.x, v.y, bMainGauche ? v.z : v.z * -1.0f ));	

			bNormales = true;	
		}
		break;	

	// Groupe  (Subset)
	case 'g':	// g - definir un groupe
		leCar = iss.get();
		
		// On ne prend pas en note le nom du groupe (il est optionnel)
		// ... Mais ça pourrait être intéressant pour des animations spécifiques.
		SubsetIndex.push_back(tabIndex.size());  // Point de départ des indices pour ce subset
		NombreSubset++;
		materialName.push_back(string("")); // On s'assure d'avoir un nom de materiau pour chaque subset
		break;

	// Les sommets et index des polygones (Faces)
	case 'f':	// f - pour face (en réalité un triangle - presque toujours!!!)
		leCar = iss.get();		// Sauter le blanc
		leCar = iss.get();      // Premier caractère
			
		while(leCar != '\0')
		{
			sPoly += leCar;				// Construire la chaine polygone
			if(leCar == '/') nbrSlash++;// Compter les /
			leCar = iss.get();			// Caractère suivant
		}

		if (nbrSlash>6)
		{
			throw 25; // Pas un triangle
		}

		// Identifier les 3 sommets
		int tabIndexTemp[3];
		for (int i=0; i<3; ++i)
		{
			ExtraireSommet(  sPoly, 
				             faceVertex.PositionIndex, 
							 faceVertex.TextCoordIndex, 
							 faceVertex.NormalIndex,
							 bCoordTex, bNormales);
			
			// Les indices peuvent être relatifs à la fin
			if (faceVertex.PositionIndex<0)
			{
				faceVertex.PositionIndex = Position.size() + faceVertex.PositionIndex;
			}
			if (faceVertex.NormalIndex<0)
			{
				faceVertex.NormalIndex = Normale.size() + faceVertex.NormalIndex;
			}
			if (faceVertex.TextCoordIndex<0)
			{
				faceVertex.TextCoordIndex = CoordTex.size() + faceVertex.TextCoordIndex;
			}

			int pos = sPoly.find(' ');
			sPoly = sPoly.substr(pos+1);

			int index = TrouverVertex(faceVertex);
			if (index==-1) 
			{
				tabVertexInfo.push_back(faceVertex);
				index = tabVertexInfo.size() - 1;
			}

			tabIndexTemp[i]=index;
		}

		if (bInverserCulling) 
		{
			int temp = tabIndexTemp[0];
			tabIndexTemp[0] = tabIndexTemp[2];
			tabIndexTemp[2] = temp;
		}

		for (int i=0; i<3; ++i) tabIndex.push_back(tabIndexTemp[i]);

		break;

		case 'm':	// mtllib - nom du fichier MATERIAL
			{
				string test;
				iss >> test;
				if (test == "tllib") iss >> OBJMaterialLib;
				if (OBJMaterialLib[0]=='.') OBJMaterialLib.erase(0,1);
				if (OBJMaterialLib[0]=='/') OBJMaterialLib.erase(0,1);

				OBJMaterialLib = cheminModele + OBJMaterialLib;
			}
	
			break;

		case 'u':	//usemtl - which material to use
			{
				string str;
				iss >> str;
				if (str == "semtl") 
				{
					iss >> str;
					materialName[NombreSubset-1] = str;
				};
			}

			break;

		default:				
			break;
	}
}

void CChargeurOBJ::TraiterLigneMAT(const string& str)
{
	char leCar;
	istringstream iss(str);

	leCar = iss.get();
	switch (leCar)
	{		
	case '#':		// Commentaires
		// On ne fait rien
		break;

	case 'd':   // d - Tranparence , peut aussi être Tr
		{
			leCar = iss.get(); // Oter le blanc
			float Transparency;
			iss >> Transparency;

			material[NombreMateriaux-1].Ambient.w = Transparency;
			material[NombreMateriaux-1].Diffuse.w = Transparency;
			material[NombreMateriaux-1].Specular.w = Transparency;

			if(Transparency < 1.0f)
				material[NombreMateriaux-1].Transparent = true;
		}
		break;
	
	case 'K':
		leCar = iss.get();
		
		if(leCar == 'a')	// Couleur ambiante
		{					
			leCar = iss.get();	// blanc de trop
			iss >> material[NombreMateriaux-1].Ambient.x;
			iss >> material[NombreMateriaux-1].Ambient.y;
			iss >> material[NombreMateriaux-1].Ambient.z;
		}		
		
		if(leCar == 'd')	// Couleur diffuse
		{
			leCar = iss.get();	// blanc de trop
			iss >> material[NombreMateriaux-1].Diffuse.x;
			iss >> material[NombreMateriaux-1].Diffuse.y;
			iss >> material[NombreMateriaux-1].Diffuse.z;
		}
		
		if(leCar == 's')	// Couleur speculaire
		{					
			leCar = iss.get();	// blanc de trop
			iss >> material[NombreMateriaux-1].Specular.x;
			iss >> material[NombreMateriaux-1].Specular.y;
			iss >> material[NombreMateriaux-1].Specular.z;
		}
		break;

	case 'm':   // mapKD
		{
		string str;
		iss >> str;
			
		if (str == "ap_Kd") 
		{
			material[NombreMateriaux-1].NomFichierTexture = cheminModele;
			iss.get();	// oter blanc

			// Nom du fichier (et chemin) Lecture «car. par car.» parce qu'il peut y avoir des blancs
			leCar = iss.get();

			while(!iss.eof())
			{
				material[NombreMateriaux-1].NomFichierTexture += leCar;
				leCar = iss.get();
			}
		}

		if (str == "ap_d") 
		{
			material[NombreMateriaux-1].Transparent = true;
		}

		}
		break;

	case 'n':	// newmtl - Nouveau matériau
		{
		string str;
		iss >> str;
			
		if (str == "ewmtl") 
		{
			leCar = iss.get();
	
			// Nouveau matériau
			OBJMaterial tempMat;
			material.push_back(tempMat);
			iss >> material[NombreMateriaux].NomMateriau;
			NombreMateriaux++;
		}
				
		}
		break;

	case 'N':     // Nd - Puissance de spécularité
		{
		leCar = iss.get();
			
		if (leCar == 'd') 
		{
			iss >> material[NombreMateriaux].Puissance;
		}

		}
		break;

	case 'T':   // Tr - Tranparence , peut aussi être d
		leCar = iss.get();
		if(leCar == 'r')
		{
			leCar = iss.get();	// Oter le blanc
			float Transparency;
			iss >> Transparency;

			material[NombreMateriaux-1].Ambient.w = Transparency;
			material[NombreMateriaux-1].Diffuse.w = Transparency;
			material[NombreMateriaux-1].Specular.w = Transparency;

			if(Transparency > 0.0f)
				material[NombreMateriaux-1].Transparent = true;
		}
		break;

	default:
		break;
	}

}

int CChargeurOBJ::TrouverVertex(const OBJVertexInfo& vertex)
{
	int index = 0;  // Pas trouvé
	bool trouve = false;

	while (index < tabVertexInfo.size() && !trouve )
	{
		if (tabVertexInfo[index].PositionIndex == vertex.PositionIndex &&
			tabVertexInfo[index].NormalIndex == vertex.NormalIndex)
			trouve = true;
		else
			index++;
	}
	if (!trouve) index = -1;

	return index;
}


}