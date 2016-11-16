#include "stdafx.h"

#include "Shader.h"
#include "comdef.h"

#include "../PetitMoteur3D/PetitMoteur3D/util.h"
#include "../PetitMoteur3D/PetitMoteur3D/Resource.h"

using PirateSimulator::Shader;

Shader::Shader(const std::string &path, PM3D::CDispositifD3D11 *dispositif) : m_vertexShader{}, m_pixelShader{}, m_VSBlob{}, m_PSBlob{}, m_D3DDevice{}, m_name{path}
{
    std::string vsPathString = path + ".vhl";
    std::string psPathString = path + ".phl";

    HRESULT hr;

    m_D3DDevice = dispositif->GetD3DDevice();

    std::wstring stemp = UtilitairesDX::s2ws(vsPathString);
    LPCWSTR vsPath = stemp.c_str();
    std::wstring stemp2 = UtilitairesDX::s2ws(psPathString);
    LPCWSTR psPath = stemp2.c_str();

    // Vertex Shader Compilation
    hr = D3DCompileFromFile(vsPath,
        nullptr,
        nullptr,
        "MiniPhongVS",
        "vs_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &m_VSBlob, nullptr);

    // Error checking
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCWSTR errMsg = err.ErrorMessage();
        MessageBox(nullptr, errMsg,
            TEXT("Vertex Shader Compilation"), MB_OK);
    }

    // Vertex Shader creation
    hr = m_D3DDevice->CreateVertexShader(m_VSBlob->GetBufferPointer(),
        m_VSBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader);
    // Error checking
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCWSTR errMsg = err.ErrorMessage();
        MessageBox(nullptr, errMsg,
            TEXT("Vertex Shader creation"), MB_OK);
    }

    // Pixel Shader compilation
    D3DCompileFromFile(psPath,
        nullptr, nullptr,
        "MiniPhongPS",
        "ps_4_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &m_PSBlob,
        nullptr);

    // Error checking
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCWSTR errMsg = err.ErrorMessage();
        MessageBox(nullptr, errMsg,
            TEXT("Pixel Shader compilation"), MB_OK);
    }

    // Pixel Shader creation
    m_D3DDevice->CreatePixelShader(m_PSBlob->GetBufferPointer(),
        m_PSBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader);

    // Error checking
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCWSTR errMsg = err.ErrorMessage();
        MessageBox(nullptr, errMsg,
            TEXT("Pixel Shader creation"), MB_OK);
    }
}

Shader::~Shader()
{
    m_vertexShader->Release();
    m_pixelShader->Release();
    m_VSBlob->Release();
    m_PSBlob->Release();
}

ID3D11VertexShader *Shader::getVertexShader() const
{
    return m_vertexShader;
}

ID3D11PixelShader *Shader::getPixelShader() const
{
    return m_pixelShader;
}

ID3DBlob   *Shader::getVSBlob() const
{
    return m_VSBlob;
}

ID3DBlob   *Shader::getPSBlob() const
{
    return m_PSBlob;
}

const std::string   &Shader::getName() const
{
    return m_name;
}
