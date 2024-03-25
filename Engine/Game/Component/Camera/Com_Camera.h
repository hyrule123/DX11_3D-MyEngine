#pragma once
#include "Engine/Game/Component/iComponent.h"

#include "Engine/define_Enum.h"

#include <bitset>

namespace  ehw
{
	
	class Com_Camera 
		: public Component<Com_Camera, eComponentCategory::Camera>
	{
		class CullingAgent;
	public:
		Com_Camera();
		virtual ~Com_Camera();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		__forceinline static const MATRIX& GetGpuViewMatrix() { return gView; }
		__forceinline static const MATRIX& GetGpuViewInvMatrix() { return gViewInverse; }
		__forceinline static const MATRIX& GetGpuProjectionMatrix() { return gProjection; }
		__forceinline static void SetGpuViewMatrix(MATRIX _view) { gView = _view; }
		__forceinline static void SetGpuProjectionMatrix(MATRIX _projection) { gProjection = _projection; }

		virtual void FinalUpdate() override;

		//이 함수는 RenderMgr가 호출
		void RenderCamera();

		void CreateViewMatrix();
		
		void RegisterCameraInRenderer();

		void TurnLayerMask(uint32 _layer, bool _enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

		void SetProjectionType(eProjectionType _type) { mProjType = _type; CreateProjectionMatrix(); }
		eProjectionType GetProjectionType() const { return mProjType; }

		inline void SetCullEnable(bool _bCullingEnable);
		bool IsCullEnabled() const { return mbCullEnable; }

		void CreateProjectionMatrix();
		void CreateProjectionMatrix(uint ResolutionX, uint ResolutionY);
		
		void SetScale(float _scale);
		float GetScale() const { return m_scale; }
		const MATRIX& GetViewMatrix() const { return mView; }
		const MATRIX& GetProjectionMatrix() const { return mProjection; }

	private:
		void SortGameObjects();
		void RenderDeffered();
		void RenderForwardOpaque();
		void RenderCutout();
		void RenderTransparent();
		void RenderPostProcess();
		void PushGameObjectToRenderingModes(const std::shared_ptr<GameObject>& _gameObj);

	private:
		static MATRIX gView;
		static MATRIX gViewInverse;
		static MATRIX gProjection;

		MATRIX mView;
		MATRIX mViewInverse;
		MATRIX mProjection;

		eProjectionType mProjType;
		std::unique_ptr<CullingAgent> mCullingAgent;
		bool mbCullEnable;

		float mAspectRatio;

		float mNear;
		float mFar;
		float m_scale;

		std::bitset<g_maxLayer> mLayerMasks;
		std::vector<std::shared_ptr<GameObject>> m_defferedOpaqueGameObjects;
		std::vector<std::shared_ptr<GameObject>> m_forwardOpaqueGameObjects;
		std::vector<std::shared_ptr<GameObject>> m_cutoutGameObjects;		//Alpha Test
		std::vector<std::shared_ptr<GameObject>> m_transparentGameObject;	//Alpha Blend
		std::vector<std::shared_ptr<GameObject>> m_postProcessGameObjects;



		class CullingAgent : public Entity
		{
		public:
			CullingAgent() {};
			virtual ~CullingAgent() {};

			virtual void FinalUpdate() = 0;

		};

		class CullingAgent_Orthographic : public CullingAgent
		{
		public:
			CullingAgent_Orthographic();
			virtual ~CullingAgent_Orthographic();

			virtual void FinalUpdate() { ASSERT(false, "미구현"); }
		};

		class CullingAgent_Perspective : public CullingAgent
		{
		public:
			CullingAgent_Perspective();
			virtual ~CullingAgent_Perspective();

			virtual void FinalUpdate() { ASSERT(false, "미구현"); }
		};
	};

}
