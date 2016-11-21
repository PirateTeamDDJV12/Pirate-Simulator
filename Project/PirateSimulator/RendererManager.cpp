#include "RendererManager.h"
#include "GameObject.h"
#include "CameraManager.h"

#include <algorithm>

#include <DirectXMath.h>
#include "..\PetitMoteur3D\PetitMoteur3D\util.h"
#include "DebugD3D11Custom.h"

using namespace PirateSimulator;
using namespace DirectX;



RendererManager RendererManager::singleton;


RendererManager::~RendererManager()
{
    DebugD3D11Custom debug;

    debug.reportLiveObject();
}

void RendererManager::drawSorting()
{
    auto iter = m_obligatoryBeforeMesh.begin();

    for(; iter != m_obligatoryBeforeMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for(iter = m_stack.begin(); iter != m_stack.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for(iter = m_obligatoryEndMesh.begin(); iter != m_obligatoryEndMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }
}

void RendererManager::drawAll()
{
    auto iter = m_obligatoryBeforeMesh.begin();

    for(; iter != m_obligatoryBeforeMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for(auto jiter = m_staticMeshArray.begin(); jiter != m_staticMeshArray.end(); ++jiter)
    {
        for(iter = jiter->meshArray.begin(); iter != jiter->meshArray.end(); ++iter)
        {
            (*iter)->Draw();
        }
    }

    for(iter = m_movingMeshArray.begin(); iter != m_movingMeshArray.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for(iter = m_obligatoryEndMesh.begin(); iter != m_obligatoryEndMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }
}

void RendererManager::addAStaticSortableMesh(PirateSimulator::IMesh* mesh)
{
    size_t x = mesh->getGameObject()->m_transform.m_position.vector4_f32[0] / AREA_WIDTH;
    size_t z = mesh->getGameObject()->m_transform.m_position.vector4_f32[2] / AREA_WIDTH;

    auto meshArray = findStaticMeshInArea(x, z);
    if(meshArray)
    {
        meshArray->push_back(mesh);
    }
}

void RendererManager::lightAddToStack(size_t x, size_t z) noexcept
{
    std::vector<IMesh*>* toAddToStack = findStaticMeshInArea(x, z);
    if(toAddToStack && !toAddToStack->empty())
    {
        std::for_each(
            toAddToStack->begin(),
            toAddToStack->end(),
            [&](IMesh* mesh) {
            m_stack.push_back(mesh);
        }
        );
    }
}

void RendererManager::deepAddToStack(size_t x, size_t z) noexcept
{
    std::vector<IMesh*>* toAddToStack = findStaticMeshInArea(x, z);
    if(toAddToStack && !toAddToStack->empty())
    {
        float xCam = CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[0];
        float zCam = CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[2];

        std::for_each(
            toAddToStack->begin(),
            toAddToStack->end(),
            [&](IMesh* mesh) {
            float xM = mesh->getGameObject()->m_transform.m_position.vector4_f32[0] - xCam;
            float zM = mesh->getGameObject()->m_transform.m_position.vector4_f32[2] - zCam;

            if((xM*xM + zM*zM) < LONG_CAMERA_SQUARE_RANGE)
            {
                m_stack.push_back(mesh);
            }
        }
        );
    }
}

void RendererManager::setDispositif(PM3D::CDispositifD3D11* creation_dispositif_specific)
{
    m_pDispositif = creation_dispositif_specific;
}

void RendererManager::updateRenderedStack()
{
    float xCameraPosition = CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[0];
    float zCameraPosition = CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[2];

    size_t xCameraArea = (xCameraPosition < 0.f ? 0 : xCameraPosition / AREA_WIDTH);
    size_t zCameraArea = (zCameraPosition < 0.f ? 0 : zCameraPosition / AREA_WIDTH);


    if(xCameraArea != m_currentX ||
       zCameraArea != m_currentZ ||
       zCameraPosition != m_forwardForward ||
       xCameraPosition != m_rightRight)
    {
        m_stack.clear();
        m_stack.reserve(MAX_VISIBLE_AREA + m_movingMeshArray.size());

        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea);

        if(zCameraPosition > 0.f)
        {
            if(xCameraPosition > 0.f) // NE
            {
                if(xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else if(xCameraArea == 0)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
            }
            else // NW
            {
                if(xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else if(xCameraArea == 0)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
            }
        }
        else
        {
            if(xCameraPosition > 0.f) // SE
            {
                if(xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                }
                else if(xCameraArea == 0)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                }
                else
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                    }
                }
            }
            else // SW
            {
                if(xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else if(xCameraArea == 0)
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea + 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
                else
                {
                    if(zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                    else if(zCameraArea == 0)
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea + 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea + 1);
                    }
                    else
                    {
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea);
                        (this->*m_pStackAddingMethod)(xCameraArea - 1, zCameraArea - 1);
                        (this->*m_pStackAddingMethod)(xCameraArea, zCameraArea - 1);
                    }
                }
            }
        }

        m_currentX = xCameraArea;
        m_currentZ = zCameraArea;
        m_forwardForward = zCameraPosition;
        m_rightRight = xCameraPosition;

        // Add the moving objects in the stack
        for(auto iter = m_movingMeshArray.begin(); iter != m_movingMeshArray.end(); ++iter)
        {
            float xM = (*iter)->getGameObject()->m_transform.m_position.vector4_f32[0] - xCameraPosition;
            float zM = (*iter)->getGameObject()->m_transform.m_position.vector4_f32[2] - zCameraPosition;

            if((xM*xM + zM*zM) < LONG_CAMERA_SQUARE_RANGE)
            {
                m_stack.push_back((*iter));
            }
        }
    }
}
