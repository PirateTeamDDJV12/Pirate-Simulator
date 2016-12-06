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

    const float step = 1.f / static_cast<float>(pointCount);
    size_t computeCount = pointCount - 1;

    for (size_t point = 1; point < computeCount; ++point)
    {
        computePoint(static_cast<float>(point) * step, end);
    }

    m_bezierTrajectory.push_back(end);
}

void CubicBezierCurve::computePoint(float placementPoint, const DirectX::XMFLOAT3 end)
{
    const float placementPointInvert = 1.f - placementPoint;

    const float squareInvert = placementPointInvert * placementPointInvert;
    const float cubeInvert = squareInvert * placementPointInvert;
    
    const float squareT = placementPoint * placementPoint;
    const float cubeT = squareT * placementPoint;

    const float x =
        m_bezierTrajectory.front().x * cubeInvert +
        3.f * (m_firstControlPoint.x * placementPoint * squareInvert +
            m_secondControlPoint.x * squareT * placementPointInvert) +
        end.x * cubeT;

    const float y =
        m_bezierTrajectory.front().y * cubeInvert +
        3.f * (m_firstControlPoint.y * placementPoint * squareInvert +
            m_secondControlPoint.y * squareT * placementPointInvert) +
        end.y * cubeT;

    const float z =
        m_bezierTrajectory.front().z * cubeInvert +
        3.f * (m_firstControlPoint.z * placementPoint * squareInvert +
            m_secondControlPoint.z * squareT * placementPointInvert) +
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