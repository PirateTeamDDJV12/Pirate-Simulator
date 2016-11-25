#ifndef DEBUGD3D11CUSTOM_H_INCLUDED
#define DEBUGD3D11CUSTOM_H_INCLUDED


#include "RendererManager.h"
#include "..\PetitMoteur3D\PetitMoteur3D\dispositifD3D11.h"

namespace PirateSimulator
{
    class DebugD3D11Custom
    {
    private:
        ID3D11Debug* pDebug;
        ID3D11Device* pD3DDevice;


    public:
        DebugD3D11Custom() :
            pDebug{ nullptr },
            pD3DDevice{ nullptr }
        {
            pD3DDevice = RendererManager::singleton.getDispositif()->GetD3DDevice();

            HRESULT hr = pD3DDevice->QueryInterface(IID_PPV_ARGS(&pDebug));
        }

        void reportLiveObject()
        {
#ifdef _DEBUG
            if (pDebug != nullptr)
            {
                pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                pDebug->Release();
                pDebug = nullptr;
            }
#endif // _DEBUG
        }

        ~DebugD3D11Custom()
        {
            if (pD3DDevice)
            {
                pD3DDevice->Release();
            }
            if (pDebug)
            {
                pDebug->Release();
            }
        }
    };
}

#endif //!DEBUGD3D11CUSTOM_H_INCLUDED