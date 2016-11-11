#include "stdafx.h"

#include "RendererManager.h"
#include "GameObject.h"
#include "CameraManager.h"

#include <algorithm>

#include <DirectXMath.h>

using namespace PirateSimulator;
using namespace DirectX;



RendererManager RendererManager::singleton;


void RendererManager::drawSorting()
{
    auto iter = m_obligatoryBeforeMesh.begin();

    for (; iter != m_obligatoryBeforeMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for (auto jiter = m_stack.begin(); jiter != m_stack.end(); ++jiter)
    {
        for (iter = (*jiter)->begin(); iter != (*jiter)->end(); ++iter)
        {
            (*iter)->Draw();
        }
    }

    for (iter = m_obligatoryBeforeMesh.begin(); iter != m_obligatoryBeforeMesh.end(); ++iter)
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

    for (iter = m_obligatoryBeforeMesh.begin(); iter != m_obligatoryBeforeMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }

    for (iter = m_obligatoryBeforeMesh.begin(); iter != m_obligatoryBeforeMesh.end(); ++iter)
    {
        (*iter)->Draw();
    }
}

void RendererManager::addAStaticSortableMesh(PirateSimulator::IMesh* mesh)
{
    size_t x = mesh->getGameObject()->m_transform.m_position.vector4_f32[0] / AREA_WIDTH;
    size_t z = mesh->getGameObject()->m_transform.m_position.vector4_f32[2] / AREA_WIDTH;

    auto meshArray = findStaticMeshInArea(x, z);
    if (meshArray)
    {
        meshArray->push_back(mesh);
    }
}

void RendererManager::updateRenderedStack()
{
    size_t xCameraArea = (CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[0] < 0.f ? 
                            0 : 
                            CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[0] / AREA_WIDTH);

    size_t zCameraArea = (CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[2] < 0.f ? 
                            0 : 
                            CameraManager::singleton.getMainCameraGO()->m_transform.m_position.vector4_f32[2] / AREA_WIDTH);

    float scalarProductCForwardWForward = CameraManager::singleton.getMainCameraGO()->m_transform.m_forward.vector4_f32[2];
    float scalarProductCRightWRight = CameraManager::singleton.getMainCameraGO()->m_transform.m_forward.vector4_f32[0];

    if (xCameraArea != m_currentX || 
        zCameraArea != m_currentZ || 
        scalarProductCForwardWForward != m_forwardForward || 
        scalarProductCRightWRight != m_rightRight)
    {
        m_stack.clear();
        m_stack.reserve(MAX_VISIBLE_AREA);

        m_stack.push_back(findStaticMeshInArea(xCameraArea, zCameraArea));

        if (scalarProductCForwardWForward > 0.f) 
        {
            if (scalarProductCRightWRight > 0.f) // NE
            {
                if (xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else if (xCameraArea == 0)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
            }
            else // NW
            {
                if (xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else if (xCameraArea == 0)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
            }
        }
        else
        {
            if (scalarProductCRightWRight > 0.f) // SE
            {
                if (xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                }
                else if (xCameraArea == 0)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                }
                else
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea + 1);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea,     zCameraArea + 1);
                    }
                }
            }
            else // SW
            {
                if (xCameraArea != 0 && xCameraArea != LAST_AREA)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else if (xCameraArea == 0)
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea + 1, zCameraArea);
                    }
                    else
                    {
                        addToStack(xCameraArea + 1, zCameraArea);
                        addToStack(xCameraArea + 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
                else
                {
                    if (zCameraArea != 0 && zCameraArea != LAST_AREA)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                    else if (zCameraArea == 0)
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea + 1);
                        addToStack(xCameraArea,     zCameraArea + 1);
                    }
                    else
                    {
                        addToStack(xCameraArea - 1, zCameraArea);
                        addToStack(xCameraArea - 1, zCameraArea - 1);
                        addToStack(xCameraArea,     zCameraArea - 1);
                    }
                }
            }
        }

        xCameraArea = m_currentX;
        zCameraArea = m_currentZ;
        scalarProductCForwardWForward = m_forwardForward;
        scalarProductCRightWRight = m_rightRight;
    }
}