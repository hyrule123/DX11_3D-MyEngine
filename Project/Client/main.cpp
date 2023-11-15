#include "PCH_Client.h"

#include <Engine/GameMainWindow.h>
#include <Contents/ContentsClassInitializer.h>

#ifdef _DEBUG
inline void DebugCheck(long _block);
#endif _DEBUG

BOOL APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    DebugCheck(0);
#endif //_DEBUG

    tDesc_GameMainWindow Desc{};
    Desc.Inst = hInstance;
    Desc.TitleName = TEXT("Monster Hunter World");
    Desc.ClassName = TEXT("GameEngine Client");
    Desc.LeftPos = 100;
    Desc.TopPos = 100;
    Desc.Width = 1280;
    Desc.Height = 720;

    Desc.WindowIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    Desc.ExternalInitFuncs.push_back(mh::ContentsClassInitializer::Init);

    Desc.GPUDesc.ResolutionX = (UINT)Desc.Width;
    Desc.GPUDesc.ResolutionY = (UINT)Desc.Height;
    Desc.GPUDesc.RefreshRate = 144u;

    return GameMainWindow::Run(Desc);
}

#ifdef _DEBUG
inline void DebugCheck(long _block)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (_block)
    {
        _CrtSetBreakAlloc(_block);
    }
}
#endif //_DEBUG
