cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
	float3 sunCoeff;
	
	float uselessFill;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0;
};

VS_Sortie MiniPhongSkyVS(float4 Pos : POSITION, float2 coordTex: TEXCOORD)  
{
VS_Sortie sortie = (VS_Sortie)0;
	
	sortie.Pos = mul(Pos, matWorldViewProj); 

    sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'état de sampling

float4 MiniPhongSkyPS( VS_Sortie vs ) : SV_Target
{
	float4 couleurTexture;  

	// Échantillonner la couleur du pixel à partir de la texture  
	couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);  

	couleurTexture.x *= sunCoeff.x;
	couleurTexture.y *= sunCoeff.y;
	couleurTexture.z *= sunCoeff.z;
	
	return couleurTexture;
}

technique11 MiniPhongSky
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, MiniPhongSkyVS()));
        SetPixelShader(CompileShader(ps_4_0, MiniPhongSkyPS()));
        SetGeometryShader(NULL);
    }
}
