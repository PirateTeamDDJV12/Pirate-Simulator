cbuffer param
{ 
	float4x4 matWorldViewProj;  // la matrice totale 
	float4x4 matWorld;			// matrice de transformation dans le monde 
	float4 vSunLight; 			// la position de la source d'éclairage (Point)
	float4 vCamera; 			// la position de la caméra	
	
	float4 vLightPoint1;	// considering a bright light Point
	float4 vLightPoint2;	// considering a bright light Point
	float4 vLightPoint3;	// considering a bright light Point
	float4 vLightPoint4;	// considering a bright light Point
	float4 mappedLightPointPowerCoeff1; // one float corresponding to 1 lightPoint; scope of it.
	
	float4 vLightPoint5;	// considering a bright light Point
	float4 vLightPoint6;	// considering a bright light Point
	float4 vLightPoint7;	// considering a bright light Point
	float4 vLightPoint8;	// considering a bright light Point
	float4 mappedLightPointPowerCoeff2; // one float corresponding to 1 lightPoint; scope of it.
	
	float4 vAEcl; 				// la valeur ambiante de l'éclairage
	float4 vAMat; 				// la valeur ambiante du matériau
	float4 vDEcl; 				// la valeur diffuse de l'éclairage 
	float4 vDMat; 				// la valeur diffuse du matériau 
	float4 vSMat; 				// la valeur spéculaire du matériau 

	
	float puissance;
	float sunPower;
	
	float tick;  				// 0 a 359.9
	float waveAmplitude;
	float waveFrequency;

    float2 undertow;
	
    float  uselessFill;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'état de sampling

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 vDirLum : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
	
	float3 vLum1 		: TEXCOORD4;
	float3 vLum2 		: TEXCOORD5;
	float3 vLum3 		: TEXCOORD6;
	float3 vLum4 		: TEXCOORD7;
	float4 dlLight1		: TEXCOORD8;
	
	float3 vLum5 		: TEXCOORD9;
	float3 vLum6 		: TEXCOORD10;
	float3 vLum7 		: TEXCOORD11;
	float3 vLum8 		: TEXCOORD12;
	float4 dlLight2		: TEXCOORD13;
};



VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD0)  
{
	VS_Sortie sortie = (VS_Sortie)0;
	
 	sortie.Pos = Pos;
	sortie.Norm = Normale;
    sortie.coordTex = coordTex;
	
	return sortie;
}


float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	
	float4 couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);   
	
	return couleurTexture;
}

technique11 MiniPhong
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, MiniPhongVS()));
        SetPixelShader(CompileShader(ps_4_0, MiniPhongPS()));
        SetGeometryShader(NULL);
    }
}
