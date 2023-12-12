#include "../PCH_Engine.h"
#include "FontWrapper.h"



#include "../Util/AtExit.h"


#include "GPUManager.h"


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper/x64/Debug/FW1FontWrapperL.lib")
#else _DEBUG
#pragma comment(lib, "FW1FontWrapper/x64/Release/FW1FontWrapper.lib")
#endif _DEBUG


#else _WIN64
#error "x64만 지원합니다"
#endif _WIN64





namespace ehw
{
	IFW1Factory* FontWrapper::mFW1Factory = nullptr;
	IFW1FontWrapper* FontWrapper::mFontWrapper = nullptr;

	bool FontWrapper::Init()
	{
		AtExit::AddFunc(Release);

		if (FAILED(FW1CreateFactory(FW1_VERSION, &mFW1Factory)))
			return false;

		auto pDevice = GPUManager::Device();
		if (FAILED(mFW1Factory->CreateFontWrapper(pDevice.Get(), L"Arial", &mFontWrapper)))
			return false;

		return true;
	}

	void FontWrapper::DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint _rgb)
	{
		auto context = GPUManager::Context();
		mFontWrapper->DrawString(context.Get(),
								 _string, // String
								 _size,// Font size
								 _x,// X position
								 _y,// Y position
								 _rgb,// Text color, 0xAaBbGgRr
								 0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
								);
	}

	void FontWrapper::Release()
	{
		mFW1Factory->Release();
		mFW1Factory = nullptr;

		mFontWrapper->Release();
		mFontWrapper = nullptr;
	}
}
