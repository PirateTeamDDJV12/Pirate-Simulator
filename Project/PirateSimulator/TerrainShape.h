#ifndef TERRAIN_SHAPE_H
#define TERRAIN_SHAPE_H
#include "ShapeComponent.h"

namespace PirateSimulator
{
    class TerrainShape : public ShapeComponent
    {
    public:
        
        std::unique_ptr<physx::PxHeightFieldSample[]> _heightMap;
        physx::unique_ptr<physx::PxHeightField> _heightField;

        bool isBoat()
        {
            return false;
        };
        bool isPiece()
        {
            return false;
        };

        bool isTrigger()
        {
            return false;
        }
        
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
