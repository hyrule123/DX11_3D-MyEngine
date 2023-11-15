#pragma once
#include "define_GPU.h"
#include "SimpleMath.h"

#include "DefaultShader/SH_Func_Light.hlsli"

namespace ehw
{
	class ConstBuffer;
	class StructBuffer;
	class Texture;
	
	class Com_Camera;

	class Com_Light3D;
	class GameObject;
	

	class MultiRenderTarget;
	class RenderMgr
	{
		friend class Application;
		friend class GPUMgr;
	public:
		static void Render();

		static ConstBuffer* GetConstBuffer(define::eCBType _Type) { return mConstBuffers[(int)_Type].get(); }
		//static void SetDataToConstBuffer(eCBType _Type, void* _pData, uint _dataCount = 1u);

		static inline Com_Camera* GetMainCam() { return mMainCamera; }
		static ComPtr<ID3D11RasterizerState>	GetRasterizerState(define::eRSType _Type) { return mRasterizerStates[(int)_Type]; }
		static ComPtr<ID3D11BlendState>		GetBlendState(define::eBSType _Type) { return mBlendStates[(int)_Type]; }
		static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(define::eDSType _Type) { return mDepthStencilStates[(int)_Type]; }
		
		static void SetMainCamera(Com_Camera* _pCam) { mMainCamera = _pCam; }
		static inline Com_Camera* GetCamera(uint _Idx);

		static void RegisterCamera(Com_Camera* _pCam) { mCameras.push_back(_pCam); }
		
		static void AddDebugMesh(const define::tDebugMesh& _DebugMesh) { mDebugMeshes.push_back(_DebugMesh); }

		static std::vector<define::tDebugMesh>& GetDebugMeshes() { return mDebugMeshes; }

		static void SetInspectorGameObject(GameObject* _pObj) { mInspectorGameObject = _pObj; }
		static GameObject* GetInspectorGameObject() { return mInspectorGameObject; }

		inline static MultiRenderTarget* GetMultiRenderTarget(define::eMRTType _Type);

		//Renderer
		static void PushLightAttribute(const tLightAttribute& lightAttribute) { mLightAttributes.push_back(lightAttribute); }
		static void AddLight(Com_Light3D* _pComLight) { if (_pComLight) mLights.push_back(_pComLight); }
		static void RemoveLight(Com_Light3D* _pComLight);

		static const std::vector<Com_Light3D*>& GetLights() { return mLights; }


		static void BindLights();
		static void BindNoiseTexture();
		static void CopyRenderTarget();

		static void ClearMultiRenderTargets();

	private:
		static void UpdateGlobalCBuffer();

		//해상도 변경 시 GPUMgr에서 호출
		static bool SetResolution(UINT _ResolutionX, UINT _ResolutionY);

		static bool CreateMultiRenderTargets(UINT _ResolutionX, UINT _ResolutionY);
		static void SetTexturesToDefferedMaterials();

		static void LoadDefaultMesh();
		static void LoadDefaultMaterial();
		static void LoadDefaultShader();
		static void LoadDefaultTexture();

		static void CreateSamplerStates();
		static void CreateRasterizerStates();
		static void CreateDepthStencilStates();
		static void CreateBlendStates();
		
		static void CreateBuffer();

		static void Init();
		static void Release();

	private:
		static Com_Camera* mMainCamera;
		static GameObject* mInspectorGameObject;

		static std::unique_ptr<ConstBuffer>		mConstBuffers[(uint)define::eCBType::END];
		static ComPtr<ID3D11SamplerState>		mSamplerStates[(uint)define::eSamplerType::END];
		static ComPtr<ID3D11RasterizerState>	mRasterizerStates[(uint)define::eRSType::END];
		static ComPtr<ID3D11DepthStencilState>	mDepthStencilStates[(uint)define::eDSType::END];
		static ComPtr<ID3D11BlendState>			mBlendStates[(uint)define::eBSType::END];
		
		static std::vector<Com_Camera*>					mCameras;
		static std::vector<define::tDebugMesh>			mDebugMeshes;

		static std::unique_ptr<MultiRenderTarget> mMultiRenderTargets[(uint)define::eMRTType::END];

		static std::vector<Com_Light3D*>			mLights;
		static std::vector<tLightAttribute>			mLightAttributes;
		static std::unique_ptr<StructBuffer>		mLightsBuffer;

		static std::shared_ptr<Texture>				mPostProcessTexture;
		static std::shared_ptr<Texture>				mNoiseTex;
	};


	inline Com_Camera* RenderMgr::GetCamera(uint _Idx)
	{
		Com_Camera* pCam = nullptr;
		if (mCameras.size() > (size_t)_Idx)
			pCam = mCameras[_Idx];

		return pCam;
	}

	inline MultiRenderTarget* RenderMgr::GetMultiRenderTarget(define::eMRTType _Type)
	{
		return mMultiRenderTargets[(int)_Type].get();
	}
}


