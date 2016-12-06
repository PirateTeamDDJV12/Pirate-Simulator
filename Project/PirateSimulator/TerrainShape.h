#ifndef TERRAIN_SHAPE_H
#define TERRAIN_SHAPE_H
#include "ShapeComponent.h"
#include "PhysicsManager.h"
#include <tchar.h>

namespace PirateSimulator
{
    class TerrainShape : public ShapeComponent
    {
        virtual TCHAR* heightMapFile() { return _T("PirateSimulator/heightmapOutput.txt"); } //Inch Allah
        std::unique_ptr<physx::PxHeightFieldSample[]> _heightMap;
        physx::unique_ptr<physx::PxHeightField> _heightField;
        static std::string typeId() noexcept
        {
            return "TerrainShape";
        }
        void onContact(const physx::PxContactPair &aContactPair) override
        {
            auto actor0 = static_cast<ShapeComponent*>(aContactPair.shapes[0]->getActor()->userData);
            auto actor1 = static_cast<ShapeComponent*>(aContactPair.shapes[1]->getActor()->userData);
            //Todo Set Behaviour
        }

        void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) override
        {
            auto actor0 = static_cast<ShapeComponent*>(contactShape->getActor()->userData);
            auto actor1 = static_cast<ShapeComponent*>(actorShape->getActor()->userData);
            
        }

        std::string getTypeId() const noexcept override
        {
            return TerrainShape::typeId();
        }

        void setGameObject(GameObject* parent) override;

    };
    
}


#endif // ifndef TERRAIN_SHAPE_H
