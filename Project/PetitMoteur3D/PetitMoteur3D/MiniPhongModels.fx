cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vLumiere; 		// la position de la source d'éclairage (Point)
	float4 vCamera; 			// la position de la caméra
	float4 vAEcl; 			// la valeur ambiante de l'éclairage
	float4 vAMat; 			// la valeur ambiante du matériau
	float4 vDEcl; 			// la valeur diffuse de l'éclairage 
	float4 vDMat; 			// la valeur diffuse du matériau 
	float4 vSEcl; 			// la valeur spéculaire de l'éclairage 
	float4 vSMat; 			// la valeur spéculaire du matériau 
	float puissance;
	float sunPower;
	float2 remplissage;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 vDirLum : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
	float isSunUnderGround : TEXCOORD4;
};

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex: TEXCOORD)  
{
VS_Sortie sortie = (VS_Sortie)0;
	
	sortie.Pos = mul(Pos, matWorldViewProj); 
	sortie.Norm = normalize(mul(Normale, matWorld)); 
	
 	float3 PosWorld = mul(Pos, matWorld);

	if(vLumiere.z < 0.0f) {
		sortie.isSunUnderGround = -1.0f;
		sortie.vDirLum = normalize(vLumiere); 
	}
	else {
		sortie.isSunUnderGround = 1.0f;
		sortie.vDirLum.x = 0.0f;
		sortie.vDirLum.y = 0.0f;
		sortie.vDirLum.z = 0.0f; 
	}
	sortie.vDirCam = normalize(vCamera - PosWorld); 

    // Coordonnées d'application de texture
    sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'état de sampling

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{

	float4 couleur; 
	float4 couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);   
	
	float4 textureColorUnderDiffuse = couleurTexture * vDEcl;
	float3 NDouble = 2.f * vs.Norm;
	
	
	/********************AMBIENT***********************/
	// Valeur de la composante diffuse
	float4 diff = saturate(dot(vs.Norm, -vs.vDirLum)) + vAEcl; 
	
	float3 RCoeff = diff * NDouble;
	
	// R = 2 * (N.L) * N – L
	float3 R = normalize(RCoeff - vs.vDirLum) ;
    
	// Calcul de la spécularité 
	float4 S = pow(saturate(dot(R, vs.vDirCam)), puissance) * vAEcl; 
	
	if(vs.isSunUnderGround < 0.0f) 
	{
		// I = A + D * N.L + (R.V)n
		couleur =  couleurTexture * vAEcl * sunPower + 
				   textureColorUnderDiffuse * diff +
				   vSMat * S;
	}	
	else {
		couleur =  couleurTexture * vAEcl +
				   textureColorUnderDiffuse * diff;
	}
			   
	return couleur;
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
