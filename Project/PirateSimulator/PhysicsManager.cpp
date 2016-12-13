#include "../PetitMoteur3D/PetitMoteur3D/stdafx.h"
#include "PhysicsManager.h"
#include "ShapeComponent.h"

#include  "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysicsAPI.h"
#include "GameObject.h"
#include "TimeManager.h"
#include <iostream>
#include "../PetitMoteur3D/PetitMoteur3D/PhysX/Include/PxPhysics.h"
#include "ICollisionHandler.h"
using namespace physx;


PirateSimulator::PhysicsManager PirateSimulator::PhysicsManager::singleton;

namespace PirateSimulator {

    ///////////////////////CALLBACKS//////////////////////////
    class SimulationEventCallback : public PxSimulationEventCallback
    {
    public:
        virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override
        {}

        virtual void onWake(PxActor** actors, PxU32 count) override
        {}

        virtual void onSleep(PxActor** actors, PxU32 count) override
        {}

        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
        {
            ICollisionHandler *actor0 = nullptr;
            GameObject *go1 = static_cast<GameObject*>(pairHeader.actors[0]->userData);
            GameObject *go2 = static_cast<GameObject*>(pairHeader.actors[1]->userData);

            if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_0))
                actor0 = static_cast<ICollisionHandler *>(go1->getComponent<ShapeComponent>()->getHandler());

            ICollisionHandler *actor1 = nullptr;
            if (!(pairHeader.flags & PxContactPairHeaderFlag::eDELETED_ACTOR_1))
                actor1 = static_cast<ICollisionHandler *>(go2->getComponent<ShapeComponent>()->getHandler());


            for (int i = 0; i < (int)nbPairs; ++i)
            {
                if (actor0)
                    actor0->onContact(pairs[i]);

                if (actor1)
                    actor1->onContact(pairs[i]);
            }
        }

        virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override
        {
            for(int i = 0; i < (int)count; ++i)
            {
                GameObject *go1 = static_cast<GameObject*>(pairs[i].triggerActor->userData);
                GameObject *go2 = static_cast<GameObject*>(pairs[i].otherActor->userData);

                bool triggerEnter = false;
                if(pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
                    triggerEnter = true;
                else if(pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
                    triggerEnter = false;
                else
                    continue;

                ICollisionHandler *trigger = nullptr;
                if(!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER))
                    trigger = static_cast<ICollisionHandler *>(go1->getComponent<ShapeComponent>()->getHandler());

                ICollisionHandler* other = nullptr;
                if(!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
                    other = static_cast<ICollisionHandler *>(go2->getComponent<ShapeComponent>()->getHandler());

                if(trigger)
                    trigger->onTrigger(triggerEnter, pairs[i].triggerShape, other ? pairs[i].otherShape : nullptr);

                if(other)
                    other->onTrigger(triggerEnter, pairs[i].otherShape, trigger ? pairs[i].triggerShape : nullptr);
            }
        }

    };
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    static SimulationEventCallback gDefaultSimulationCallback;

    ///////////////////////////////////////END/////////////////////////////

    static PxFilterFlags SimulationFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
    {
        // let triggers through
        if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;
        }
        // generate contacts for all that were not filtered above
        pairFlags = PxPairFlag::eCONTACT_DEFAULT;

        // trigger the contact callback for pairs (A,B) where 
        // the filtermask of A contains the ID of B and vice versa.
        if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
        {
            pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
            return PxFilterFlag::eDEFAULT;
        }

        return PxFilterFlag::eSUPPRESS;
    }

    void customizeSceneDesc(PxSceneDesc *aSceneDesc)
    {
        aSceneDesc->gravity = PxVec3(0.0f, -9.81f, 0.0f);
        //aSceneDesc->gravity = PxVec3(0.0f, 0.0f, 0.0f);
        aSceneDesc->filterShader = &SimulationFilterShader;
    }

    void PhysicsManager::update()
    {
        auto &timeManager = TimeManager::GetInstance();
        auto remainingTimeToSimulate = timeManager.getElapsedTimeFrame();


        auto durationStep = 100;
        while (remainingTimeToSimulate > 0)
        {
            if (remainingTimeToSimulate > durationStep)
            {
                _scene->simulate(durationStep);
                remainingTimeToSimulate -= durationStep;
            }
            else
            {
                _scene->simulate(remainingTimeToSimulate);
                remainingTimeToSimulate = 0;
            }
            _scene->fetchResults(true);
        }

    }
    void PhysicsManager::initialize()
    {
        _foundation = physx::unique_ptr<PxFoundation>(
            PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));

        bool recordMemoryAllocations = true;
        _profileZoneManager = physx::unique_ptr<PxProfileZoneManager>(
            &PxProfileZoneManager::createProfileZoneManager(_foundation.get())
            );

        if (!_profileZoneManager)
            std::cout<<("PxProfileZoneManager::createProfileZoneManager failed"); //GOOD

#if PX_SUPPORT_GPU_PHYSX
        PxCudaContextManagerDesc cudaContextManagerDesc;
        _cudaContextManager = physx::unique_ptr<PxCudaContextManager>(
            PxCreateCudaContextManager(*_foundation, cudaContextManagerDesc, _profileZoneManager.get()));
        if (_cudaContextManager)
        {
            if (!_cudaContextManager->contextIsValid())
            {
                _cudaContextManager.reset();
            }
        }
#endif

        _physics = physx::unique_ptr<PxPhysics>(
            PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation,
                PxTolerancesScale(), recordMemoryAllocations, _profileZoneManager.get()));
        if (!_physics)
            std::cout<<("PxCreatePhysics failed");
        if (!PxInitExtensions(*_physics))
            std::cout << ("PxInitExtensions failed");


        if (_physics->getPvdConnectionManager() != nullptr)
        {
            PxVisualDebuggerConnectionFlags connectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
            _visualDebuggerConnection = physx::unique_ptr<debugger::comm::PvdConnection>(
                PxVisualDebuggerExt::createConnection(_physics->getPvdConnectionManager(),
                    "localhost", 5425, 100, connectionFlags));
            
            if (_visualDebuggerConnection == nullptr)
                std::cout << ("    NOT CONNECTED!\n");//PB mais c'est pareil dans balls
            else
                std::cout<<("    CONNECTED!\n");
        }


        // create the scene
        PxSceneDesc sceneDesc(_physics->getTolerancesScale());
        customizeSceneDesc(&sceneDesc);

        if (!sceneDesc.cpuDispatcher)
        {
            _cpuDispatcher = physx::unique_ptr<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(1));
            if (!_cpuDispatcher)
                std::cout << "PxDefaultCpuDispatcherCreate failed!"; //GOOD
            sceneDesc.cpuDispatcher = _cpuDispatcher.get();
            if (!sceneDesc.filterShader)
                sceneDesc.filterShader = PxDefaultSimulationFilterShader;

#if PX_SUPPORT_GPU_PHYSX
            if (!sceneDesc.gpuDispatcher && _cudaContextManager)
            {
                sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
            }
#endif

            _scene = physx::unique_ptr<physx::PxScene>(_physics->createScene(sceneDesc));
            if (!_scene)
                std::cout << "createScene failed!"; //PB

            _scene->setSimulationEventCallback(&gDefaultSimulationCallback);
        }
        
    }
    


    physx::PxPhysics& PhysicsManager::physics()
    {
        return  *_physics;
    }

    physx::PxScene& PhysicsManager::scene()
    {
        return *_scene;
    }

    void PhysicsManager::reset()
    {
        for (auto shape : m_components)
        {
            shape->cleanUp();
            //shape->~ShapeComponent();
            //m_components.pop_back();
        }
        m_components.clear();
        /*specificRelease(_foundation);

        specificRelease(_physics);
        specificRelease(_scene);
        specificRelease(_cpuDispatcher);
        specificRelease(_cudaContextManager);
        specificRelease(_visualDebuggerConnection);
        _profileZoneManager->release();*/
    }


    ShapeComponent* PhysicsManager::getVehiculeShape()
    {
        ShapeComponent* vehicle = nullptr;

        for (auto shape : m_components)
        {
            if (shape->getPiece() == nullptr)
                vehicle = shape;
        }
        return vehicle;
    }
}

