cbuffer param
{
	float darkening; // darkening effect to darken the final picture -> must be between 0.f and 1.f
	
	float3 uselessFill;
}

struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree;  // la texture
SamplerState SampleState; // l'état de sampling

//---------------------------------------------------------
// Vertex Shader «Nul»
//---------------------------------------------------------
VS_Sortie NulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0 )
{
VS_Sortie sortie = (VS_Sortie)0;
	sortie.Pos = Pos;
	sortie.CoordTex = CoordTex;
	return sortie;
}

//-----------------------------------------------------
// Pixel Shader «Nul»
//-----------------------------------------------------
float4 GrayOverlayPostEffect( VS_Sortie vs) : SV_Target
{
	float4 couleur = textureEntree.Sample(SampleState, vs.CoordTex);
	
	float average = (couleur.x + couleur.y + couleur.z) * darkening / 3.f;
	
	couleur.x = average;
	couleur.y = average;
	couleur.z = average;
	
	return couleur;
}

technique11 Nul
{
	pass p0
	{
		VertexShader = compile vs_4_0 NulVS();
		PixelShader = compile ps_4_0 GrayOverlayPostEffect();
		SetGeometryShader(NULL);
	}
}