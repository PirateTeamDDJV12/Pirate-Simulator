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

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD0, float originAngle : TEXCOORD1)  
{
	VS_Sortie sortie = (VS_Sortie)0;
	
 	sortie.Pos = Pos;

	//float2 coord;
	// coord.x = sortie.Pos.x / 256;
	// coord.y = sortie.Pos.z / 256;
	// sortie.Pos.y = sortie.Pos.y + sinValue.Sample(SampleState, coord);
	
	
	// Version sans tableau
	float wiseComputationParam = (waveFrequency * (originAngle + tick));
	
	float sinusCoeff = sin(wiseComputationParam + sortie.Pos.x);
	float cosinusCoeff = cos(wiseComputationParam + sortie.Pos.z / 6.f);
	
	sortie.Pos.y = sortie.Pos.y + waveAmplitude * (sinusCoeff + cosinusCoeff);
	
	sortie.Pos = mul(sortie.Pos, matWorldViewProj);

	sortie.Norm = mul(Normale, matWorld); 
	
 	float3 PosWorld = mul(Pos, matWorld);

	sortie.vDirLum = vSunLight; 
	sortie.vLum1 = vLightPoint1 - PosWorld;
	sortie.vLum2 = vLightPoint2 - PosWorld;
	sortie.vLum3 = vLightPoint3 - PosWorld;
	sortie.vLum4 = vLightPoint4 - PosWorld;
	sortie.vLum5 = vLightPoint5 - PosWorld;
	sortie.vLum6 = vLightPoint6 - PosWorld;
	sortie.vLum7 = vLightPoint7 - PosWorld;
	sortie.vLum8 = vLightPoint8 - PosWorld;
	
	sortie.dlLight1.x = (sortie.vLum1.x * sortie.vLum1.x + sortie.vLum1.y * sortie.vLum1.y + sortie.vLum1.z * sortie.vLum1.z);
	sortie.dlLight1.y = (sortie.vLum2.x * sortie.vLum2.x + sortie.vLum2.y * sortie.vLum2.y + sortie.vLum2.z * sortie.vLum2.z);
	sortie.dlLight1.z = (sortie.vLum3.x * sortie.vLum3.x + sortie.vLum3.y * sortie.vLum3.y + sortie.vLum3.z * sortie.vLum3.z);
	sortie.dlLight1.w = (sortie.vLum4.x * sortie.vLum4.x + sortie.vLum4.y * sortie.vLum4.y + sortie.vLum4.z * sortie.vLum4.z);
	sortie.dlLight2.x = (sortie.vLum5.x * sortie.vLum5.x + sortie.vLum5.y * sortie.vLum5.y + sortie.vLum5.z * sortie.vLum5.z);
	sortie.dlLight2.y = (sortie.vLum6.x * sortie.vLum6.x + sortie.vLum6.y * sortie.vLum6.y + sortie.vLum6.z * sortie.vLum6.z);
	sortie.dlLight2.z = (sortie.vLum7.x * sortie.vLum7.x + sortie.vLum7.y * sortie.vLum7.y + sortie.vLum7.z * sortie.vLum7.z);
	sortie.dlLight2.w = (sortie.vLum8.x * sortie.vLum8.x + sortie.vLum8.y * sortie.vLum8.y + sortie.vLum8.z * sortie.vLum8.z);
	
	sortie.dlLight1 = sqrt(sortie.dlLight1);
	sortie.dlLight2 = sqrt(sortie.dlLight2);
	
	sortie.dlLight1.x = 1.f / sortie.dlLight1.x;
	sortie.dlLight1.y = 1.f / sortie.dlLight1.y;
	sortie.dlLight1.z = 1.f / sortie.dlLight1.z;
	sortie.dlLight1.w = 1.f / sortie.dlLight1.w;
	sortie.dlLight2.x = 1.f / sortie.dlLight2.x;
	sortie.dlLight2.y = 1.f / sortie.dlLight2.y;
	sortie.dlLight2.z = 1.f / sortie.dlLight2.z;
	sortie.dlLight2.w = 1.f / sortie.dlLight2.w;
	
	sortie.vDirCam = vCamera - PosWorld; 

    // Coordonnées d'application de texture
    sortie.coordTex = coordTex;
	
	return sortie;
}


float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	// Normaliser les paramètres
	float3 N = normalize(vs.Norm);
 	float3 L = normalize(vs.vDirLum);
	float3 V = normalize(vs.vDirCam);
	
	float2 glitch = vs.coordTex + undertow;
	// Échantillonner la couleur du pixel à partir de la texture  
	float4 couleurTexture = textureEntree.Sample(SampleState, glitch);   
	
	float4 textureColorUnderDiffuse = couleurTexture * vDEcl;
	
	float3 NDouble = 2.f * N;
	
	
	/********************AMBIENT***********************/
	// Valeur de la composante diffuse
	float4 diff = saturate(dot(N, -L)) + vAEcl; 
	
	float3 RCoeff = diff * NDouble;
	
	// R = 2 * (N.L) * N – L
	float3 R = normalize(RCoeff - L) ;
    
	// Calcul de la spécularité 
	float4 S = pow(saturate(dot(R, V)), puissance) * vAEcl; 
	
	// I = A + D * N.L + (R.V)n
	float4 couleur =  couleurTexture * vAEcl * sunPower + 
			   textureColorUnderDiffuse * diff +
			   vSMat * S;

	

	/*********************POINT 1***********************/
	L = normalize(vs.vLum1);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	float4 point1Color = (textureColorUnderDiffuse * vs.dlLight1.x * diff) * mappedLightPointPowerCoeff1.x;
	
	
	
	/*********************POINT 2***********************/
	L = normalize(vs.vLum2);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point2Color = (textureColorUnderDiffuse * vs.dlLight1.y * diff) * mappedLightPointPowerCoeff1.y;
	
	
	
	/*********************POINT 3***********************/
	L = normalize(vs.vLum3);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point3Color = (textureColorUnderDiffuse * vs.dlLight1.z * diff) * mappedLightPointPowerCoeff1.z;
	
	
	
	/*********************POINT 4***********************/
	L = normalize(vs.vLum4);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	float4 point4Color = (textureColorUnderDiffuse * vs.dlLight1.w * diff) * mappedLightPointPowerCoeff1.w;
	
	
	
	/*********************POINT 5***********************/
	L = normalize(vs.vLum5);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point5Color = (textureColorUnderDiffuse * vs.dlLight2.x * diff) * mappedLightPointPowerCoeff2.x;
	
	
	
	
	
	/*********************POINT 6***********************/
	L = normalize(vs.vLum6);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point6Color = (textureColorUnderDiffuse * vs.dlLight2.y * diff) * mappedLightPointPowerCoeff2.y;
	
	
	
	
	
	/*********************POINT 7***********************/
	L = normalize(vs.vLum7);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point7Color = (textureColorUnderDiffuse * vs.dlLight2.z * diff) * mappedLightPointPowerCoeff2.z;
	
	
	
	
	
	/*********************POINT 8***********************/
	L = normalize(vs.vLum8);
	
	diff = saturate(dot(N, -L));
	
	RCoeff = diff * NDouble;
	
	R = normalize(RCoeff - L);
	
	// Calcul de la spécularité 
	S = pow(saturate(dot(R, V)), puissance);
	
	
	float4 point8Color = (textureColorUnderDiffuse * vs.dlLight2.w * diff) * mappedLightPointPowerCoeff2.w;
	
	
	
	return couleur + point1Color + point2Color + point3Color + point4Color + point5Color + point6Color + point7Color + point8Color;
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
