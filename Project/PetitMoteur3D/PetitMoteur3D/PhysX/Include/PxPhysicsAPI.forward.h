#pragma once

#include <memory>

//=============================================================================
// FORWARD DECLARATIONS
//=============================================================================
namespace physx 
{ 
	class PxTransform; 
	class PxConvexMesh;
	class PxPhysics;
	class PxActor;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxScene;
	struct PxContactPair;
	class PxShape;

	// custom deleter for unique_ptr
	template<class _Ty>
	struct physx_deleter
	{
		void operator()(_Ty *_Ptr) const _NOEXCEPT
		{
			_Ptr->release();
		}
	};

	template<class _Ty>
	using unique_ptr = std::unique_ptr<_Ty, physx_deleter<_Ty>>;
}

