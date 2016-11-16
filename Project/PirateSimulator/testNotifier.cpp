#include "stdafx.h"


#include "NotifierManager.h"

#include "GameObject.h"
#include "GameObjectManager.h"
#include "..\PetitMoteur3D\PetitMoteur3D\ObjetMesh.h"
#include "..\PetitMoteur3D\PetitMoteur3D\MoteurWindows.h"
#include "..\PetitMoteur3D\PetitMoteur3D\Singleton.h"


using namespace PirateSimulator;
using namespace PM3D;


void testNotifier()
{
    auto notifier = NotifierManager::singleton.addANotifier<GameObject>("GameObjectNotifier")->as<Notifier<GameObject>>();

    notifier->registerNotified(GameObjectManager::singleton);

    auto transform = Transform();

    transform.m_position.vector4_f32[0] = 2.f;
    transform.m_position.vector4_f32[1] = 0.f;
    transform.m_position.vector4_f32[2] = 2.f;

    auto testMesh = new CObjetMesh(".\\modeles\\jin\\jin.OMB", ShaderCObjectMesh::ShadersParams(), CMoteurWindows::GetInstance().getDispositif());

    auto TestGO = new GameObject(transform, "TestNotification");
    TestGO->addComponent<IMesh>(testMesh);

    RendererManager::singleton.addAnObligatoryMeshToDrawAtEnd(testMesh);

    notifier->sendNotification(*TestGO);
}