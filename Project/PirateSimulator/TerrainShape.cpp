#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "TerrainShape.h"
#include "PhysicsManager.h"
#include "RessourceManager.h"
#include "Vertex.h"
#include "ICollisionHandler.h"
#include "GameObjectManager.h"
#include "Terrain.h"
#include <d3d9types.h>

using namespace physx;


namespace PirateSimulator
{
    class CollisionTerrainHandler: public ICollisionHandler
    {
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
    };
    struct CUSTOMVERTEX {
        physx::PxVec3 p;
        physx::PxVec3 n;
        static const int FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
        CUSTOMVERTEX() {};
    };

    /*void TerrainShape::setGameObject(GameObject* parent)
    {
        auto config = Config::getInstance();
        float width = config->getWidth();
        float height = config->getHeight();
        float scale = config->getScale();
        static const TCHAR *kHeightPixMapFile = heightMapFile();
        physx::PxVec3 scale = heightMapScale();
        std::vector<CUSTOMVERTEX> pVoid;
        _heightMap = std::make_unique<physx::PxHeightFieldSample[]>(width*height);
        
    }*/





    void TerrainShape::setGameObject(GameObject* parent)
    {
        //On récupère le Terrain
        Terrain* terrainData = GameObjectManager::singleton.getGameObjectByName("terrain")->getComponent<Terrain>();
        float width = terrainData->getWidth();
        float height = terrainData->getHeight();
        float scale = terrainData->getScale();
        std::vector<CUSTOMVERTEX> pVoid;
        

        _heightMap = std::make_unique<physx::PxHeightFieldSample[]>(Config::getInstance()->getHeight()
            *Config::getInstance()->getWidth());

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                _heightMap[x*height + y].height = terrainData->getHeight(DirectX::XMVECTOR{static_cast<float>(x),static_cast<float>(y) });
                _heightMap[x*height + y].materialIndex0 = 0;
                _heightMap[x*height + y].materialIndex1 = 0;
                _heightMap[x*height + y].clearTessFlag();
            }
        }



        //HeightMapData
        
        PxHeightFieldDesc heightMapDesc;
        heightMapDesc.format = PxHeightFieldFormat::eS16_TM;
        heightMapDesc.nbColumns = width;
        heightMapDesc.nbRows = height;
        heightMapDesc.samples.data = _heightMap.get();
        heightMapDesc.samples.stride = sizeof(PxHeightFieldSample);
        
         _heightField = physx::unique_ptr<physx::PxHeightField>(
            PhysicsManager::singleton.physics().createHeightField(heightMapDesc));
        //Création Shape

        //m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
        //m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
        PxRigidStatic &m_actor = *PhysicsManager::singleton.physics().createRigidStatic(parent->m_transform.getPose());
        
        PxShape *_shape = m_actor.createShape(PxHeightFieldGeometry(_heightField.get(), PxMeshGeometryFlag::eDOUBLE_SIDED, scale / 65535.f , scale/width, scale/height),
             *m_material);
         m_shape = _shape;
       
        PhysicsManager::singleton.scene().addActor(m_actor);
        PxFilterData filterData;
        filterData.word0 = EACTORTERRAIN;
        filterData.word1 = 0;
        m_shape->setSimulationFilterData(filterData);
         //Register shape
        setHandler(ICollisionHandlerRef(new CollisionTerrainHandler));
        //m_actor->setMass(0.0001f);
        m_actor.userData = parent;
        PhysicsManager::singleton.registerNewComponent(this);
        m_gameObject = parent;
    }

}