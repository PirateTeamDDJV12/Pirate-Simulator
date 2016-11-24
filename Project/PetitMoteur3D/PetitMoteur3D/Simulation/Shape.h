#pragma once
#include "..\..\..\PirateSimulator\Component.h"
#include "../../PetitMoteur3D/stdafx.h"
namespace PirateSimulator
{
	class  Shape : public Component
	{
	private:
		physx::PxActorShape m_shape;


	};
}