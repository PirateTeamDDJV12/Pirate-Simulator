#ifndef BEZIERCURVE_H_INCLUDED
#define BEZIERCURVE_H_INCLUDED

#include <vector>
#include <directxmath.h>


namespace PirateSimulator
{
    class CubicBezierCurve
    {
    private:
        std::vector<DirectX::XMFLOAT3> m_bezierTrajectory;
        DirectX::XMFLOAT3 m_firstControlPoint;
        DirectX::XMFLOAT3 m_secondControlPoint;


    public:
        CubicBezierCurve(
            const DirectX::XMFLOAT3& start,
            const DirectX::XMFLOAT3& firstControlPoint,
            const DirectX::XMFLOAT3& secondControlPoint,
            const DirectX::XMFLOAT3& end,
            size_t pointCount
        );


    public:
        const DirectX::XMFLOAT3& getStartingPoint() const noexcept;
        const DirectX::XMFLOAT3& getEndingPoint() const noexcept;
        const DirectX::XMFLOAT3& getFirstControlPoint() const noexcept;
        const DirectX::XMFLOAT3& getSecondingPoint() const noexcept;
        
        size_t getPointCount() const noexcept;
        float getPointStep() const noexcept;
        const std::vector<DirectX::XMFLOAT3>& getTrajectory() const noexcept;


    private:
        void computePoint(float placementPoint, const DirectX::XMFLOAT3 end);
    };
}


#endif //!BEZIERCURVE_H_INCLUDED