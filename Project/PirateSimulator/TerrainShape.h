#ifndef TERRAIN_SHAPE_H
#define TERRAIN_SHAPE_H
#include "ShapeComponent.h"
#include "PhysicsManager.h"
#include <tchar.h>
#include <d3d9types.h>
#include "..\PetitMoteur3D\PetitMoteur3D\Config\Config.hpp"

namespace PirateSimulator
{
    class TerrainShape : public ShapeComponent
    {
    public:
        struct CUSTOMVERTEX {
            physx::PxVec3 p;
            physx::PxVec3 n;
            DWORD COLOR;
            static const int FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
        };

        std::unique_ptr<physx::PxHeightFieldSample[]> _heightMap;
        physx::unique_ptr<physx::PxHeightField> _heightField;

        

        
        static std::string typeId() noexcept
        {
            return "ShapeComponent";
        }
        

        std::string getTypeId() const noexcept override
        {
            return TerrainShape::typeId();
        }

        void setGameObject(GameObject* parent) override;
        
    };
    
}


#endif // ifndef TERRAIN_SHAPE_H
