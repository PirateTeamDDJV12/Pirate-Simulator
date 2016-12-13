#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "TerrainShape.h"
#include "PhysicsManager.h"
#include "RessourceManager.h"
#include "Vertex.h"
#include "ICollisionHandler.h"
#include "GameObjectManager.h"
#include "Terrain.h"
#include <d3d9types.h>
#include "../PetitMoteur3D/PetitMoteur3D/Config/Config.hpp"
#include <d3d9.h>
#include "RendererManager.h"
#include "BlocMeshStructure.h"
#include <vector>
#include "../PetitMoteur3D/PetitMoteur3D/util.h"

using namespace physx;


namespace PirateSimulator
{
    class CollisionTerrainHandler: public ICollisionHandler
    {
        struct CUSTOMVERTEX {
            physx::PxVec3 p;
            physx::PxVec3 n;
            static const int FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
        };
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

    void TerrainShape::setGameObject(GameObject* parent)
    {
   
        //On récupère le Terrain
        Terrain* terrainData = GameObjectManager::singleton.getGameObjectByName("terrain")->getComponent<Terrain>();
        float width = terrainData->getWidth();
        float height = terrainData->getHeight();
        float scale = terrainData->getScale();
        auto config = Config::getInstance();
        int verticeCount = width*height;
        std::vector<physx::PxVec3> vertexArray;
        std::vector<unsigned int> index_bloc;

        CUSTOMVERTEX* pVoid;
        auto pDispositif = RendererManager::singleton.getDispositif(); // Prendre en note le dispositif
        vertexArray.reserve(width * height);
        std::vector<float> myFile = RessourcesManager::GetInstance().ReadHeightMapFile(config->getExportName());
        int nbPoint = Vertex::INFO_COUNT * width * height;
        for (int i = 0; i < nbPoint; i += Vertex::INFO_COUNT)
        {
            physx::PxVec3 p{ myFile[i], myFile[i + 2], myFile[i + 1]};
            vertexArray.push_back(p);
        }
        for (size_t i = nbPoint; i < myFile.size(); i += 3)
        {
            index_bloc.push_back(static_cast<unsigned int>(myFile[i]));
            index_bloc.push_back(static_cast<unsigned int>(myFile[i]+1));
            index_bloc.push_back(static_cast<unsigned int>(myFile[i]+2));
        }

    



        _heightMap = std::make_unique<physx::PxHeightFieldSample[]>(height*width);

        for (int z = 0; z < height; ++z)
        {
            for (int x = 0; x < width; ++x)
            {
                physx::PxVec3 getPoint = vertexArray.at(x*height+z);
                auto ptHeight = getPoint.y;
                _heightMap[x*height + z].height = ptHeight;
                _heightMap[x*height + z].materialIndex0 = 0;
                _heightMap[x*height + z].materialIndex1 = 0;
                _heightMap[x*height + z].clearTessFlag();
            }
        }
        int* pIdx;    // a void pointer
        for (int z= 0; z < height - 1; ++z)
        {
            for (int x = 0; x < width; ++x)
            {
                index_bloc.push_back( x + z*width);
                index_bloc.push_back(x + (z+1)*width);
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

        PxRigidStatic &m_actor = *PhysicsManager::singleton.physics().createRigidStatic(parent->m_transform.getPose());
        
        PxShape *_shape = m_actor.createShape(PxHeightFieldGeometry(_heightField.get(), PxMeshGeometryFlag::eDOUBLE_SIDED, scale, scale, scale),
             *m_material);
         m_shape = _shape;
       
        PhysicsManager::singleton.scene().addActor(m_actor);
        PxFilterData filterData;
        filterData.word0 = EACTORTERRAIN;
        filterData.word1 = EACTORVEHICLE;
        m_shape->setSimulationFilterData(filterData);
         //Register shape
        setHandler(ICollisionHandlerRef(new CollisionTerrainHandler));
        //m_actor->setMass(0.0001f);
        m_actor.userData = parent;
        PhysicsManager::singleton.registerNewComponent(this);
        m_gameObject = parent;
    }

}