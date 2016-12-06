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
        /*get the beginning point of the bezier curve*/
        const DirectX::XMFLOAT3& getStartingPoint() const noexcept;

        /*get the ending point of the bezier curve*/
        const DirectX::XMFLOAT3& getEndingPoint() const noexcept;

        /*get the first control point of the bezier curve*/
        const DirectX::XMFLOAT3& getFirstControlPoint() const noexcept;

        /*get the second control point of the bezier curve*/
        const DirectX::XMFLOAT3& getSecondingPoint() const noexcept;
        
        /*get the number of points in the curve composing the trajectory*/
        size_t getPointCount() const noexcept;

        /*get the delta t between each points*/
        float getPointStep() const noexcept;

        /*return the trajectory*/
        const std::vector<DirectX::XMFLOAT3>& getTrajectory() const noexcept;


    private:
        /*compute the point at t (0.f <= t <= 1.f) (see wikipedia for the formula of cubic Bezier curve)*/
        void computePoint(float t, const DirectX::XMFLOAT3 end);
    };
}


#endif //!BEZIERCURVE_H_INCLUDED