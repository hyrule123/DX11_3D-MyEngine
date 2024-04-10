#pragma once
#include "Engine/GPU/CommonGPU.h"
#include "Engine/CommonStruct.h"

namespace ehw
{
	class Texture;
	class GPUManager 
	{
		friend class GameEngine;

	public:
		__forceinline static ComPtr<ID3D11Device>			Device() { return mDevice; }
		__forceinline static ComPtr<ID3D11DeviceContext>	Context() { return mContext; }

	public:
		static void ClearRenderTarget();// 화면 지워주기
		static inline void Present(bool _bVSync = false);

		static inline std::shared_ptr<Texture> GetRenderTargetTex();
		static inline std::shared_ptr<Texture> GetDepthStencilBufferTex();

		static bool SetResoulution(UINT _ResolutionX, UINT _ResolutionY);
		static UINT GetResolutionX() { return mResolutionX; }
		static UINT GetResolutionY() { return mResolutionY; }
		static uint2 GetResolution() { return uint2{ mResolutionX, mResolutionY }; }

		//struct tGPUManagerDesc
		//{
		//	UINT ResolutionX;
		//	UINT ResolutionY;
		//	UINT RefreshRate;
		//};
	private:
		static bool Init(const tGPUManagerDesc& _Desc);
		static void Release();
		static void Reset();
		static bool CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext);

		
		
		//스왑체인 + 렌더타겟 생성하여 반환
		static std::shared_ptr<Texture> CreateSwapChain(UINT _ResolutionX, UINT _ResolutionY, UINT _RefreshRate);
		static std::shared_ptr<Texture> CreateDepthStencil(UINT _ResolutionX, UINT _ResolutionY);

		//Application의 창 크기를 따라감
		static void CreateMainViewPort();

		//mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());

	private:
		static UINT mResolutionX;
		static UINT mResolutionY;
		static UINT mRefreshRate;

		static ComPtr<ID3D11Device> mDevice;
		static ComPtr<ID3D11DeviceContext> mContext;
	
		static ComPtr<IDXGISwapChain> mSwapChain;
		
		static std::shared_ptr<ehw::Texture> mRenderTargetTexture;
		static std::shared_ptr<ehw::Texture> mDepthStencilBufferTexture;

		
		static D3D11_VIEWPORT mViewPort;
	};

	inline void GPUManager::Present(bool _bVSync)
	{
		int VSync = _bVSync ? 1 : 0;
		mSwapChain->Present(VSync, 0u);
	}

	inline std::shared_ptr<ehw::Texture> GPUManager::GetRenderTargetTex()
	{
		return mRenderTargetTexture;
	}

	inline std::shared_ptr<ehw::Texture> GPUManager::GetDepthStencilBufferTex()
	{
		return mDepthStencilBufferTexture;
	}
}


