#ifndef RENDERERMANAGER_H_INCLUDED
#define RENDERERMANAGER_H_INCLUDED

#include "GameConfig.h"
#include "Mesh.h"

#include <vector>


namespace PirateSimulator
{
    struct RenderArea
    {
        size_t x;
        size_t z;

        std::vector<IMesh*> meshArray;

        RenderArea(size_t x, size_t z) :
            x{ x },
            z{ z }
        {}
    };

    class RendererManager
    {
    private:
        using RenderStack = std::vector<std::vector<IMesh*>*>;


    private:
        enum
        {
            MAX_VISIBLE_AREA = 8
        };

    public:
        enum
        {
            AREA_EDGE_COUNT = 4,
            AREA_TOTAL_COUNT = AREA_EDGE_COUNT * AREA_EDGE_COUNT,
            AREA_WIDTH = (GameGlobals::SceneGlobals::MAX_X - GameGlobals::SceneGlobals::MIN_X) / AREA_EDGE_COUNT + 1,
            VISIBLE_AREA = (AREA_TOTAL_COUNT < MAX_VISIBLE_AREA) ? AREA_TOTAL_COUNT : MAX_VISIBLE_AREA,

            LAST_AREA = AREA_EDGE_COUNT - 1
        };


    public:
        static RendererManager singleton;


    private:
        std::vector<IMesh*> m_obligatoryBeforeMesh;
        std::vector<RenderArea> m_staticMeshArray;
        std::vector<IMesh*> m_movingMeshArray;
        std::vector<IMesh*> m_obligatoryEndMesh;

        RenderStack m_stack;

        size_t m_currentX;
        size_t m_currentZ;
        size_t m_forwardForward;
        size_t m_rightRight;

        void (RendererManager::* m_pMeshDraw)();


    private:
        RendererManager() :
            m_pMeshDraw{ m_pMeshDraw = &RendererManager::drawAll },
            m_staticMeshArray{},
            m_obligatoryBeforeMesh{},
            m_obligatoryEndMesh{},
            m_currentX{},
            m_currentZ{},
            m_forwardForward{},
            m_rightRight{}
        {
            m_stack.reserve(MAX_VISIBLE_AREA);
            m_staticMeshArray.reserve(AREA_TOTAL_COUNT);
            for (size_t z = 0; z < AREA_EDGE_COUNT; ++z)
            {
                for (size_t x = 0; x < AREA_EDGE_COUNT; ++x)
                {
                    m_staticMeshArray.push_back(RenderArea(x, z));
                }
            }
        }


    private:
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;


    public:
        void addAnObligatoryMeshToDrawBefore(IMesh* mesh)   { m_obligatoryBeforeMesh.push_back(mesh); }
        void addAMovingSortableMesh(IMesh* mesh)            { m_movingMeshArray.push_back(mesh); }
        void addAnObligatoryMeshToDrawAtEnd(IMesh* mesh)    { m_obligatoryEndMesh.push_back(mesh); }
        void addAStaticSortableMesh(IMesh* mesh);

        size_t getObligatoryMeshBeforeCount() const noexcept 
        { 
            return m_obligatoryBeforeMesh.size(); 
        }

        size_t getObligatoryMeshEndCount() const noexcept
        {
            return m_obligatoryEndMesh.size();
        }

        size_t getMovingSecondaryMeshCount() const noexcept
        {
            return m_movingMeshArray.size();
        }

        size_t getAreaCount() const noexcept
        {
            return m_staticMeshArray.size();
        }

        size_t getStaticMeshInArea(size_t x, size_t z) const noexcept
        {
            return m_staticMeshArray[x * z].meshArray.size();
        }

        void setSortingMesh(bool mustSortMesh) noexcept
        {
            if (mustSortMesh)
            {
                m_pMeshDraw = &RendererManager::drawSorting;
            }
            else
            {
                m_pMeshDraw = &RendererManager::drawAll;
            }
        }

        std::vector<IMesh*>* findStaticMeshInArea(size_t x, size_t z) noexcept
        {
            size_t index = z * AREA_EDGE_COUNT + x;
            if (index < AREA_TOTAL_COUNT)
            {
                return &m_staticMeshArray[index].meshArray;
            }
            
            return nullptr;
        }


    private:
        void drawSorting();
        void drawAll();

        void sortAndDraw();

        void updateRenderedStack() noexcept;

        void addToStack(size_t x, size_t z) noexcept
        {
            std::vector<IMesh*>* toAddToStack = findStaticMeshInArea(x, z);
            if (toAddToStack)
            {
                m_stack.push_back(toAddToStack);
            }
        }

    public:
        void operator()() { (this->*m_pMeshDraw)(); }
    };
}

#endif // !RENDERERMANAGER_H_INCLUDED