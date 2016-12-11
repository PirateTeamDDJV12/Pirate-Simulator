#pragma once
//=============================================================================
// EXTERNAL DECLARATIONS
//=============================================================================
#include "SimulationComponent.h"

namespace engine {

    //=============================================================================
    // CLASS SimulationComponentBase
    //=============================================================================
    template<class TDerived, class PXACTORTYPE>
    void SimulationComponentBase<TDerived, PXACTORTYPE>::onAttached(const GameObjectRef &iGameObject)
    {
        _go = iGameObject;
        _pxActor = TDerived::createPxActor();
        _pxActor->userData = static_cast<IGameObject*>(_go.lock().get());
        Game<ISimulationManager>()->scene().addActor(*_pxActor);
    }

    template<class TDerived, class PXACTORTYPE>
    void SimulationComponentBase<TDerived, PXACTORTYPE>::onDetached(const GameObjectRef &iGameObject)
    {
        _pxActor.reset();
    }

    template<class TDerived, class PXACTORTYPE>
    void SimulationComponentBase<TDerived, PXACTORTYPE>::onContact(const physx::PxContactPair &aContactPair)
    {
        if (_collisionHandler)
        {
            _collisionHandler->onContact(aContactPair);
        }
    }

    template<class TDerived, class PXACTORTYPE>
    void SimulationComponentBase<TDerived, PXACTORTYPE>::onTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape)
    {
        if (_collisionHandler)
        {
            _collisionHandler->onTrigger(triggerEnter, actorShape, contactShape);
        }
    }

    template<class TDerived, class PXACTORTYPE>
    void SimulationComponentBase<TDerived, PXACTORTYPE>::setHandler(ICollisionHandlerRef iHandler)
    {
        _collisionHandler = iHandler;
    }


} // namespace engine
