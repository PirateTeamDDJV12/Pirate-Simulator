#include "BezierCurve.h"

using namespace PirateSimulator;


CubicBezierCurve::CubicBezierCurve(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& firstControlPoint,
    const DirectX::XMFLOAT3& secondControlPoint,
    const DirectX::XMFLOAT3& end,
    size_t pointCount
) :
    m_firstControlPoint{ firstControlPoint },
    m_secondControlPoint{ secondControlPoint }
{
    m_bezierTrajectory.reserve(pointCount);

    m_bezierTrajectory.push_back(start);

    size_t computeCount = pointCount - 1;
    const float step = 1.f / static_cast<float>(computeCount);

    for (size_t point = 1; point < computeCount; ++point)
    {
        computePoint(static_cast<float>(point) * step, end);
    }

    m_bezierTrajectory.push_back(end);
}

void CubicBezierCurve::computePoint(float t, const DirectX::XMFLOAT3& end)
{
    const float placementPointInvert = 1.f - t;

    const float squareInvert = placementPointInvert * placementPointInvert;
    const float cubeInvert = squareInvert * placementPointInvert;
    
    const float squareT = t * t;
    const float cubeT = squareT * t;

    const float secondCoeff = 3.f * t * squareInvert;
    const float thirdCoeff = 3.f * squareT * placementPointInvert;

    const float x =
        m_bezierTrajectory.front().x * cubeInvert +
        m_firstControlPoint.x * secondCoeff +
        m_secondControlPoint.x * thirdCoeff +
        end.x * cubeT;

    const float y =
        m_bezierTrajectory.front().y * cubeInvert +
        m_firstControlPoint.y * secondCoeff +
        m_secondControlPoint.y * thirdCoeff +
        end.y * cubeT;

    const float z =
        m_bezierTrajectory.front().z * cubeInvert +
        m_firstControlPoint.z * secondCoeff +
        m_secondControlPoint.z * thirdCoeff +
        end.z * cubeT;

    m_bezierTrajectory.emplace_back(x, y, z);
}

const DirectX::XMFLOAT3& CubicBezierCurve::getStartingPoint() const noexcept
{
    return m_bezierTrajectory.front();
}

const DirectX::XMFLOAT3& CubicBezierCurve::getEndingPoint() const noexcept
{
    return m_bezierTrajectory.back();
}

const DirectX::XMFLOAT3& CubicBezierCurve::getFirstControlPoint() const noexcept
{
    return m_firstControlPoint;
}

const DirectX::XMFLOAT3& CubicBezierCurve::getSecondingPoint() const noexcept
{
    return m_secondControlPoint;
}

size_t CubicBezierCurve::getPointCount() const noexcept
{
    return m_bezierTrajectory.size();
}

float CubicBezierCurve::getPointStep() const noexcept
{
    return 1 / static_cast<float>(m_bezierTrajectory.size());
}

const std::vector<DirectX::XMFLOAT3>& CubicBezierCurve::getTrajectory() const noexcept
{
    return m_bezierTrajectory;
}