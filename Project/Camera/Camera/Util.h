#ifndef UTIL_H
#define UTIL_H

#include <directxmath.h>

// Loads an XMFLOAT3 into an XMVECTOR.
inline DirectX::XMVECTOR MathFV(DirectX::XMFLOAT3& val)
{
    return XMLoadFloat3(&val);
}

// Stores an XMVECTOR in an XMFLOAT3
inline DirectX::XMFLOAT3 MathVF(DirectX::XMVECTOR& vec)
{
    DirectX::XMFLOAT3 val;
    DirectX::XMStoreFloat3(&val, vec);
    return val;
}

// Stores an XMVECTOR in an XMFLOAT4X4A
inline DirectX::XMFLOAT4X4 MathMF(DirectX::XMMATRIX& mat)
{
    DirectX::XMFLOAT4X4 val;
    DirectX::XMStoreFloat4x4(&val, mat);
    return val;
}

// Loads an XMFLOAT4X4 into an XMMATRIX
inline DirectX::XMMATRIX MathFM(DirectX::XMFLOAT4X4& val)
{
    return DirectX::XMLoadFloat4x4(&val);
}

#endif //!UTIL_H