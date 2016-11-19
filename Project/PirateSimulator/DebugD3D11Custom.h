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
            if (pDebug != nullptr)
            {
                pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
                pDebug = nullptr;
            }
        }
    };
}

#endif //!DEBUGD3D11CUSTOM_H_INCLUDED