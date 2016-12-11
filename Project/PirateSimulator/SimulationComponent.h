#ifndef SIMULATION_COMPONENT_H
#define  SIMULATION_COMPONENT_H



#include <memory>
#include <string>
#include "PhysicsManager.h"
#include "..\PetitMoteur3D\PetitMoteur3D\PhysX\Include\PxActor.h"
#include "GameObject.h"
#include "ICollisionHandler.h"
#include "IColliderInterface.h"

#pragma warning( disable : 4250 )

namespace PirateSimulator {

    //=============================================================================
    // CLASS SimulationComponentBase
    //=============================================================================
    template<class TDerived, class PXACTORTYPE>
    class SimulationComponentBase :
        public IColliderInterface/*,
        public IPoseInterface,
        virtual public IComponentInterface*/
    {
    public:
        virtual void onAttached(const GameObjectRef &iGameObject);
        virtual void onDetached(const GameObjectRef &iGameObject);
        virtual void onContact(const physx::PxContactPair &aContactPair) ;
        virtual void onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape) ;
        virtual void setHandler(ICollisionHandlerRef iHandler) ;

    protected:
        GameObjectRef _go;
        physx::unique_ptr<PXACTORTYPE> _pxActor;
        ICollisionHandlerRef _collisionHandler;
    };

    //=============================================================================
    // CLASS DynamicSimulationComponent
    //=============================================================================
    class DynamicSimulationComponent :
        public SimulationComponentBase<DynamicSimulationComponent, physx::PxRigidDynamic>
    {
    private:
        using base = SimulationComponentBase;

    public:
        static std::string TypeId();

       /* static IComponentInterface::IdType* Interfaces()
        {
            static IComponentInterface::IdType sInterfaces[] = {
                IPoseInterface::TypeId(),
                IColliderInterface::TypeId(),
                ISimulationInterface::TypeId(),
                IRigidSimulationInterface::TypeId(),
                IDynamicSimulationInterface::TypeId(),
                0
            };

            return sInterfaces;
        }*/

    public:
        static physx::unique_ptr<physx::PxRigidDynamic> createPxActor();

        virtual physx::PxTransform pose() ;
        virtual void setPose(const physx::PxTransform &iPose);

    public:
        virtual physx::PxRigidDynamic& pxActor();
    };

    //=============================================================================
    // CLASS StaticSimulationComponent
    //=============================================================================
    class StaticSimulationComponent :
        public SimulationComponentBase<StaticSimulationComponent, physx::PxRigidStatic>
    {
    private:
        using Base = SimulationComponentBase;

    public:
        static std::string TypeId();

      /*  static IComponentInterface::IdType* Interfaces()
        {
            static IComponentInterface::IdType sInterfaces[] = {
                IPoseInterface::TypeId(),
                IColliderInterface::TypeId(),
                ISimulationInterface::TypeId(),
                IRigidSimulationInterface::TypeId(),
                IStaticSimulationInterface::TypeId(),
                0
            };

            return sInterfaces;
        }*/

    public:
        static physx::unique_ptr<physx::PxRigidStatic> createPxActor();

    public: // IStaticSimulationInterface
        virtual physx::PxRigidStatic& pxActor() ;

    public: // IPoseInterface
        virtual physx::PxTransform pose() ;
        virtual void setPose(const physx::PxTransform &iPose) ;
    };

} // namespace engine




#endif
