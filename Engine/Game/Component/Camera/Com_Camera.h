#pragma once
#include "Engine/Game/Component/Component.h"

#include "Engine/define_Enum.h"

#include <bitset>
//그냥 하나여도 drawindexedinstanced로 호출하는걸로 통일
//https://www.gamedev.net/forums/topic/654210-drawindexed-vs-drawindexedinstanced/
namespace  ehw
{
	class Renderer;
	class SceneRenderer;
	class Com_Camera 
		: public Component<Com_Camera, eComponentCategory::Camera>
	{
		CLASS_NAME(Com_Camera);
		
		class CullingAgent;
	public:
		Com_Camera();
		virtual ~Com_Camera();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void final_update() override;
		void OnEnable() override;
		void OnDisable() override;

		void render_gameobjects(const tRenderQueue& _renderQueue);
		void render_lights();

		void CreateViewMatrix();

		void TurnLayerMask(uint32 _layer, bool _enable = true);
		void EnableLayerMasks() { m_layerMasks.set(); }
		void DisableLayerMasks() { m_layerMasks.reset(); }

		void SetProjectionType(eProjectionType _type) { m_projectionType = _type; CreateProjectionMatrix(); }
		eProjectionType GetProjectionType() const { return m_projectionType; }

		inline void SetCullEnable(bool _bCullingEnable);
		bool IsCullEnabled() const { return m_isEnableCulling; }

		void CreateProjectionMatrix();
		void CreateProjectionMatrix(uint ResolutionX, uint ResolutionY);
		
		void SetScale(float _scale);
		float GetScale() const { return m_scale; }
		const MATRIX& GetViewMatrix() const { return m_camera_matrices.view; }
		const MATRIX& GetProjectionMatrix() const { return m_camera_matrices.projection; }

		void bind_data_to_GPU();

	private:
		tCamera m_camera_matrices;

		eProjectionType m_projectionType;
		std::unique_ptr<CullingAgent> m_cullingAgent;
		bool m_isEnableCulling;

		float m_aspectRation;

		float m_nearDistance;
		float m_farDistance;
		float m_scale;

		std::bitset<g_maxLayer> m_layerMasks;
		//std::vector<Renderer*> m_defferedOpaque;
		//std::vector<Renderer*> m_forwardOpaque;
		//std::vector<Renderer*> m_alphaTest;		//forward_cutout
		//std::vector<Renderer*> m_alphaBlend;	//forward_transparent
		//std::vector<Renderer*> m_postProcess;

	public:
		class CullingAgent : public Entity
		{
		public:
			CullingAgent(const std::string_view key);
			virtual ~CullingAgent() {};

			virtual void final_update() = 0;

		};

		class CullingAgent_Orthographic : public CullingAgent
		{
			CLASS_NAME(CullingAgent_Orthographic);
		public:
			CullingAgent_Orthographic();
			virtual ~CullingAgent_Orthographic();

			virtual void final_update() { ASSERT(false, "미구현"); }
		};

		class CullingAgent_Perspective : public CullingAgent
		{
			CLASS_NAME(CullingAgent_Perspective);
		public:
			CullingAgent_Perspective();
			virtual ~CullingAgent_Perspective();

			virtual void final_update() { ASSERT(false, "미구현"); }
		};
	};

}
