#pragma once
#include "Entity.h"

#include "define_Component.h"
#include "SimpleMath.h"

namespace ehw
{
	class GameObject;
	class IScene;
	class IComponent : public Entity
	{
		friend class ComMgr;
	public:
		IComponent(eComponentCategory _type);

		IComponent(const IComponent& _other);
		CLONE_DISABLE(IComponent);

		virtual ~IComponent();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		virtual void Init() {};
		virtual void Awake() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void OnDestroy() {};
		virtual void InternalUpdate() = 0;

		GameObject* GetOwner() const { return m_Owner; }
		void SetOwner(GameObject* _owner) { m_Owner = _owner; }

		IScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(IScene* _scene) { m_OwnerScene = _scene; }

		eComponentCategory GetComCategory() const { return m_ComCategory; };

		void SetComTypeID(UINT32 _comTypeID) { m_ComTypeID = _comTypeID; }
		UINT32 GetComTypeID() const { return m_ComTypeID; };

		void CallStart() { if (false == m_bStart) { Start(); m_bStart = true; } }

		bool IsEnabled() const { return eState::Enabled == m_State; }
		void SetEnable(bool _bEnable);

		bool IsDestroyed() const { return eState::Destroy == m_State; }
		void Destroy();
		void ForceDestroy() { m_State = eState::Destroy; }

	private:
		const eComponentCategory m_ComCategory;
		UINT32 m_ComTypeID;
		GameObject* m_Owner;
		IScene* m_OwnerScene;


		bool m_bStart;

		enum class eState
		{
			Disabled,
			Enabled,
			Destroy
		} m_State;

	};
}
