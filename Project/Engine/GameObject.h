		#pragma once
#include "Entity.h"

#include "Components.h"
#include "ComMgr.h"
#include "Com_Transform.h"

namespace mh
{
	class Layer;
	class GameObject : public Entity
	{
		friend class GameObject;
		friend class EventMgr;
	public:
		enum class eState
		{
			Active,
			Paused,
			Dead,
		};
		inline static bool IsActive(eState _state) { return (eState::Active == _state); }
		inline static bool IsDead(eState _state) { return (eState::Dead == _state); }
		inline static bool IsPaused(eState _state) { return (eState::Paused == _state); }

	public:
		GameObject();
		GameObject(const GameObject& _other);
		CLONE(GameObject);

		virtual ~GameObject();

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		
		virtual void Init();
		virtual void Start();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:

		template <typename T>
		inline T* AddComponent();
		
		IComponent* AddComponent(IComponent* _pCom);
		inline IComponent* AddComponent(const std::string_view _strKey);

		template <typename T>
		inline T* GetComponent();

		inline IComponent* GetComponent(define::eComponentType _type) { return mComponents[(int)_type]; }

		template <typename T>
		inline eComponentType GetComponentType();

		const std::vector<IComponent*>& GetComponents() { return mComponents; }
		inline const std::span<IScript*> GetScripts();

		void SetName(const std::string_view _Name) { mName = _Name; }
		const std::string& GetName() const { return mName; }

		eState GetState() { return mState; }
		
		void Pause() { mState = eState::Paused; }
		void Destroy();
				
		bool IsDontDestroy() { return mbDontDestroy; }
		void DontDestroy(bool _enable) { mbDontDestroy = _enable; }
		
		define::eLayerType GetLayerType() const { return mLayerType; }
		void SetLayerType(define::eLayerType _type) { mLayerType = _type; }

		GameObject* AddChild(GameObject* _pObj);

		void GetGameObjectHierarchy(std::vector<GameObject*>& _gameObjects);

		bool IsMaster() const { return (nullptr == mParent); }
		GameObject* GetParent() { return mParent; }
		const std::vector<GameObject*>& GetChilds() const { return mChilds; }

		void SetParent(GameObject* _pObj) { mParent = _pObj; }
		void RemoveChild(GameObject* _pObj);

		bool IsInitialized() const { return mbInitalized; }
		bool IsStarted() const { return mbStarted; }

		//편의성을 위한 컴포넌트 받아오기 함수
		ITransform* Transform() { return static_cast<ITransform*>(mComponents[(int)eComponentType::Transform]); }
		ICollider* Collider() { return static_cast<ICollider*>(mComponents[(int)eComponentType::Transform]); }
		ILight* Light() { return static_cast<ILight*>(mComponents[(int)eComponentType::Light]); }
		Com_Camera* Camera() { return static_cast<Com_Camera*>(mComponents[(int)eComponentType::Camera]); }
		IRenderer* Renderer() { return static_cast<IRenderer*>(mComponents[(int)eComponentType::Renderer]); }
		Com_AudioSource* AudioSource() { return static_cast<Com_AudioSource*>(mComponents[(int)eComponentType::AudioSource]); }
		Com_AudioListener* AudioListener() { return static_cast<Com_AudioListener*>(mComponents[(int)eComponentType::AudioListener]); }
		Com_BehaviorTree* BehaviorTree() { return static_cast<Com_BehaviorTree*>(mComponents[(int)eComponentType::BehaviorTree]); }

	protected:
		void DestroyRecursive();

	private:
		std::string mName;
		eState mState;
		define::eLayerType mLayerType;

		std::vector<IComponent*>	mComponents;

		GameObject* mParent;
		std::vector<GameObject*> mChilds;

		bool mbInitalized;
		bool mbStarted;
		bool mbDontDestroy;
	};


	template <typename T>
	T* GameObject::AddComponent()
	{
		define::eComponentType order = GetComponentType<T>();

		if (eComponentType::UNKNOWN == order)
			return nullptr;

		T* pCom = new T;
		pCom->SetComTypeID(ComMgr::GetComTypeID<T>());
		pCom->SetKey(ComMgr::GetComName<T>());

		return static_cast<T*>(AddComponent(static_cast<IComponent*>(pCom)));
	}

	inline IComponent* GameObject::AddComponent(const std::string_view _strKey)
	{
		IComponent* pCom = ComMgr::GetNewCom(_strKey);

		if (nullptr == pCom)
		{
			return nullptr;
		}

		return AddComponent(pCom);
	}



	inline void GameObject::DestroyRecursive()
	{
		mState = eState::Dead;
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			mChilds[i]->DestroyRecursive();
		}
	}

	inline GameObject* GameObject::AddChild(GameObject* _pChild)
	{
		//nullptr이나 자기 자신을 인자로 호출했을 경우 오류 발생			
		MH_ASSERT(_pChild && this != _pChild);

		//부모 오브젝트가 있을 경우 기존의 부모 오브젝트에서 자신을 제거한 후 여기에 추가해야함
		GameObject* parent = _pChild->GetParent();
		if (parent)
		{
			parent->RemoveChild(_pChild);
		}
		_pChild->SetParent(this);
		mChilds.push_back(_pChild);

		if (mbInitalized && false == _pChild->IsInitialized())
		{
			_pChild->Init();
		}

		return _pChild;
	}

	inline void GameObject::GetGameObjectHierarchy(std::vector<GameObject*>& _gameObjects)
	{
		_gameObjects.push_back(this);
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			mChilds[i]->GetGameObjectHierarchy(_gameObjects);
		}
	}

	inline void GameObject::RemoveChild(GameObject* _pObj)
	{
		for (auto iter = mChilds.begin(); iter != mChilds.end(); ++iter)
		{
			if ((*iter) == _pObj)
			{
				(*iter)->SetParent(nullptr);
				mChilds.erase(iter);
				break;
			}
		}
	}


	template <typename T>
	T* GameObject::GetComponent()
	{
		T* pCom{};

		if constexpr (std::is_base_of_v<IScript, T>)
		{
			const std::string_view name = ComMgr::GetComName<T>();
			for (size_t i = (size_t)eComponentType::Scripts; i < mComponents.size(); ++i)
			{
				if (name == mComponents[i]->GetKey())
				{
					pCom = static_cast<T*>(mComponents[i]);
					break;
				}
			}
		}
		else
		{
			eComponentType ComType = GetComponentType<T>();
			if (mComponents[(int)ComType])
			{
				//일단 ID값으로 비교 후 일치 시 static_cast해도 안전
				if (ComMgr::GetComTypeID<T>() == mComponents[(int)ComType]->GetComTypeID())
				{
					pCom = static_cast<T*>(mComponents[(int)ComType]);
				}
			}
		}

		return pCom;
	}


	template<typename T>
	inline eComponentType GameObject::GetComponentType()
	{
		if constexpr (std::is_base_of_v<ITransform, T>)
		{
			return eComponentType::Transform;
		}
		else if constexpr (std::is_base_of_v<ICollider, T>)
		{
			return eComponentType::Collider;
		}
		else if constexpr (std::is_base_of_v<IAnimator, T>)
		{
			return eComponentType::Animator;
		}
		else if constexpr (std::is_base_of_v<ILight, T>)
		{
			return eComponentType::Light;
		}
		else if constexpr (std::is_base_of_v<Com_Camera, T>)
		{
			return eComponentType::Camera;
		}
		else if constexpr (std::is_base_of_v<Com_AudioSource, T>)
		{
			return eComponentType::AudioSource;
		}
		else if constexpr (std::is_base_of_v<Com_AudioListener, T>)
		{
			return eComponentType::AudioListener;
		}
		else if constexpr (std::is_base_of_v<IRenderer, T>)
		{
			return eComponentType::Renderer;
		}
		else if constexpr (std::is_base_of_v<Com_BehaviorTree, T>)
		{
			return eComponentType::BehaviorTree;
		}
		else if constexpr (std::is_base_of_v<IScript, T>)
		{
			return eComponentType::Scripts;
		}

		return eComponentType::UNKNOWN;
	}


	inline const std::span<IScript*> GameObject::GetScripts()
	{
		std::span<IScript*> scriptSpan{};

		int ScriptSize = (int)mComponents.size() - (int)eComponentType::Scripts;
		if (0 < ScriptSize)
		{
			scriptSpan =
				std::span<IScript*>((IScript**)mComponents.data() + (size_t)eComponentType::Scripts, (size_t)ScriptSize);
		}

		return scriptSpan;
	}
}

