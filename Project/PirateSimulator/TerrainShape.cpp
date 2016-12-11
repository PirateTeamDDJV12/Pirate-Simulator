#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "TerrainShape.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"
#include "../PetitMoteur3D/PetitMoteur3D/Config/Config.hpp"
#include <d3d9types.h>
#include "RessourceManager.h"
#include "Vertex.h"


using namespace physx;





namespace PirateSimulator
{
    struct CUSTOMVERTEX {
        physx::PxVec3 p;
        physx::PxVec3 n;
        static const int FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
        CUSTOMVERTEX() {};
    };
    void TerrainShape::setGameObject(GameObject* parent)
    {
        //_heightmapData
        //CUSTOMVERTEX* pVoid = new CUSTOMVERTEX();    // a void pointer
        std::vector<CUSTOMVERTEX> pVoid;




        std::vector<float> myFile = PirateSimulator::RessourcesManager::GetInstance().ReadHeightMapFile("PirateSimulator/heightmapOutput.txt");
        const int vertexLineCount = 1 + PirateSimulator::Vertex::INFO_COUNT;
        int nbPoint = vertexLineCount * Config::getInstance()->getHeight() * Config::getInstance()->getWidth();
        int pointCount = 0;
        for (int i = 0; i < nbPoint; i += vertexLineCount)
        {
            float x = myFile[i + 1];
            float y = myFile[i + 3];
            float z = myFile[i + 2];
            float xNorm = myFile[i + 4];
            float yNorm = myFile[i + 5];
            float zNorm = myFile[i + 6];
            pVoid.push_back(CUSTOMVERTEX());
            pVoid[pointCount].p.x = x;
            pVoid[pointCount].p.y = y;
            pVoid[pointCount].p.z = z;
            pVoid[pointCount].n.x = xNorm;
            pVoid[pointCount].n.y = yNorm;
            pVoid[pointCount].n.z = zNorm;
            ++pointCount;
        }

        _heightMap = std::make_unique<physx::PxHeightFieldSample[]>(Config::getInstance()->getHeight()*Config::getInstance()->getScale()
            *Config::getInstance()->getWidth()*Config::getInstance()->getScale());

        for (int y = 0; y < Config::getInstance()->getHeight()*Config::getInstance()->getScale(); ++y)
        {
            for (int x = 0; x < Config::getInstance()->getWidth()*Config::getInstance()->getScale(); ++x)
            {
                _heightMap[x*Config::getInstance()->getHeight()*Config::getInstance()->getScale() + y].height =
                    PxI16(65535 * pVoid[x + y*Config::getInstance()->getWidth()*Config::getInstance()->getScale()].p.y / Config::getInstance()->getScale());
                _heightMap[x*Config::getInstance()->getHeight()*Config::getInstance()->getScale() + y].materialIndex0 = 0;
                _heightMap[x*Config::getInstance()->getHeight()*Config::getInstance()->getScale() + y].materialIndex1 = 0;
                _heightMap[x*Config::getInstance()->getHeight()*Config::getInstance()->getScale() + y].clearTessFlag();
            }
        }



        //HeightMapData
        PxHeightFieldDesc heightMapDesc;
        heightMapDesc.format = PxHeightFieldFormat::eS16_TM;
        heightMapDesc.nbColumns = Config::getInstance()->getWidth()*Config::getInstance()->getScale();
        heightMapDesc.nbRows = Config::getInstance()->getHeight()*Config::getInstance()->getScale();
        heightMapDesc.samples.data = _heightMap.get();
        heightMapDesc.samples.stride = sizeof(PxHeightFieldSample);
        PxHeightField * heightField = PhysicsManager::singleton.physics().createHeightField(heightMapDesc);
        //_heightField = physx::unique_ptr<physx::PxHeightField>(
          //  PhysicsManager::singleton.physics().createHeightField(heightMapDesc));
        //Création Shape

        //m_material = PhysicsManager::singleton.physics().createMaterial(0.5f, 0.5f, 0.1f);
        m_actor = PhysicsManager::singleton.physics().createRigidDynamic(physx::PxTransform::createIdentity());
        PxRigidStatic &staticActor = *PhysicsManager::singleton.physics().createRigidStatic(physx::PxTransform::createIdentity());
        PxReal scale = 1;
         PxShape *_shape = staticActor.createShape(PxHeightFieldGeometry(heightField, PxMeshGeometryFlag::eDOUBLE_SIDED, scale, scale, scale),
             *m_material);

        PhysicsManager::singleton.scene().addActor(*m_actor);
        PxFilterData filterData;
        filterData.word0 = EACTORTERRAIN;
        filterData.word1 = EACTORVEHICLE;
         //Register shape
        PhysicsManager::singleton.registerNewComponent(this);
        m_gameObject = parent;
    }

}