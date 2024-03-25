#include "Engine/Game/GameObject.h"

#include "Engine/Manager/SceneManager.h"

#include "Engine/Util/Serialize/json.h"

#include "Engine/Resource/Prefab.h"

#include "Engine/Game/Component/Renderer/iRenderer.h"

#include "Engine/Game/Layer.h"

namespace ehw
{
	namespace strKey::Json
	{
		namespace GameObject
		{
			STRKEY_DECLARE(m_baseComponents);
			STRKEY_DECLARE(m_childs);
		}
	}


	GameObject::GameObject()
		: m_transform()
		, m_baseComponents()
		, m_ownerScene(nullptr)
		, m_layer()
		, m_name()
		, m_state(eState::Active)
		, m_bAwake(false)
		, m_bDontDestroyOnLoad(false)
	{
		AddComponent(&m_transform);
	}

	GameObject::GameObject(const std::string_view _name)
		: GameObject()
	{
		m_name = _name;
	}


	GameObject::GameObject(const GameObject& _other)
		: Entity(_other)
		, m_baseComponents()
		, m_ownerScene(_other.m_ownerScene)
		, m_layer()
		, m_name(_other.m_name)
		, m_state(_other.m_state)
		, m_bAwake(_other.m_bAwake)
		, m_bDontDestroyOnLoad(_other.m_bDontDestroyOnLoad)
	{
		//1. 컴포넌트 목록 복사
		for (size_t i = 0; i < _other.m_baseComponents.size(); ++i)
		{
			if (_other.m_baseComponents[i])
			{
				std::unique_ptr<iComponent> cloned = std::unique_ptr<iComponent>(_other.m_baseComponents[i]->Clone());
				AddComponent(cloned);
			}
		}
	}

	GameObject::~GameObject()
	{
		//Transform은 GameObject에 붙어있으므로
		for (size_t i = (size_t)eComponentCategory::Transform + 1; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				delete m_baseComponents[i];
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i])
			{
				delete m_scripts[i];
			}
		}
	}

	eResult GameObject::Serialize_Json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer& ser = *_ser;

		ser[JSON_KEY(m_name)] << m_name;
		ser[JSON_KEY(m_layer)] << m_layer;
		ser[JSON_KEY(m_bDontDestroyOnLoad)] << m_bDontDestroyOnLoad;

		ASSERT(false, "미구현");

		return eResult::Success;
	}

	eResult GameObject::DeSerialize_Json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		ASSERT(false, "미구현");

		return eResult::Success;
	}
	
	void GameObject::Awake()
	{
		if (false == IsActive() || m_bAwake)
		{
			return;
		}

		m_bAwake = true;
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->Awake();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			m_scripts[i]->Awake();
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->OnEnable();
			}
		}

		//Awake의 경우 재귀적으로 호출
		const std::vector<Com_Transform*>& childs = GetComponent<Com_Transform>()->GetChilds();
		for (size_t i = 0; i < childs.size(); ++i)
		{
			childs[i]->GetOwner()->Awake();
		}
	}

	void GameObject::Update()
	{
		if (false == IsActive())
		{
			return;
		}
			
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{

			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->CallStart();
				m_baseComponents[i]->Update();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->CallStart();
				m_scripts[i]->Update();
			}
		}
	}

	void GameObject::CollisionUpdate()
	{
		if (m_baseComponents[(int)eComponentCategory::Collider])
		{
			//std::static_pointer_cast<iCollider>(m_baseComponents[(int)eComponentCategory::Collider])
				//->CollisionUpdate();
		}
	}

	void GameObject::FinalUpdate()
	{
		if (false == IsActive())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->FinalUpdate();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->FinalUpdate();
			}
		}
	}

	void GameObject::RemoveDestroyed()
	{
		if (IsDestroyed())
		{
			m_state = eState::Destroy;
			return;
		}
		

		//개별 Component Destroy 여부 확인
		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->NeedRemove())
			{
				m_baseComponents[i] = nullptr;
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->NeedRemove())
			{
				m_scripts[i] = nullptr;
			}
		}
	}


	//이 함수는 다른 카메라가 호출함
	//
	void GameObject::Render()
	{
		if (false == IsActive())
		{
			return;
		}

		if (m_baseComponents[(int)eComponentCategory::Renderer] && 
			m_baseComponents[(int)eComponentCategory::Renderer]->IsEnabled())
		{
			static_cast<iRenderer*>(m_baseComponents[(int)eComponentCategory::Renderer])->Render();
		}
	}

	void GameObject::FrameEnd()
	{
		if (false == IsActive())
		{
			return;
		}

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
			{
				m_baseComponents[i]->FrameEnd();
			}
		}

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i] && m_scripts[i]->IsEnabled())
			{
				m_scripts[i]->FrameEnd();
			}
		}
	}


	iComponent* GameObject::AddComponent(iComponent* _pCom)
	{
		iComponent* ret = nullptr;
		if (nullptr == _pCom)
		{
			return ret;
		}

		ret = _pCom;
		eComponentCategory ComType = _pCom->GetComponentCategory();

		ASSERT(false == _pCom->GetStrKey().empty(),
			"컴포넌트에 String Key가 없습니다.\nAddComponent<T> 또는 ComponentManager::GetNewComponent()를 통해서 생성하세요.");

		
		if (eComponentCategory::Scripts == ComType)
		{
			m_scripts.push_back(static_cast<iScript*>(_pCom));
		}
		else
		{
			//해당 컴포넌트 카테고리가 비어있을 경우 컴포넌트를 집어넣는다.
			if (nullptr == m_baseComponents[(int)ComType])
			{
				m_baseComponents[(int)ComType] = _pCom;
			}

			//동일한 ID의 컴포넌트가 컴포넌트 카테고리 안에 들어가 있을경우 해당 컴포넌트를 반환한다.
			else if (_pCom->GetComponentTypeID() == m_baseComponents[(int)ComType]->GetComponentTypeID())
			{
				_pCom = nullptr;
				return m_baseComponents[(int)ComType];
			}

			//컴포넌트가 들어가 있는데, 동일한 컴포넌트 ID가 아닐 경우 에러
			else
			{
				ASSERT(nullptr == m_baseComponents[(int)ComType], "이미 중복된 타입의 컴포넌트가 들어가 있습니다.");
			}
		}

		ret->SetOwner(this);
		ret->SetOwnerScene(m_ownerScene);
		ret->Init();

		//Active 상태이고, Awake 이미 호출되었을 경우 Awake 함수 호출
		if (IsActive() && m_bAwake)
		{
			ret->Awake();
		}

		return ret;
	}



	void GameObject::SetActive(bool _bActive)
	{
		if (IsDestroyed() || IsActive() == _bActive)
			return;

		//씬이 작동 중일 경우 람다함수를 통해 지연 실행
		if (m_ownerScene->IsAwaken())
		{
			m_ownerScene->AddFrameEndJob(&GameObject::SetActiveRecursive, this, _bActive);
		}

		//씬이 작동중이지 않을 경우 바로 호출
		else
		{
			SetActiveRecursive(_bActive);
		}

	}

	void GameObject::Destroy()
	{
		if (IsDestroyed())
		{
			return;
		}

		m_state = eState::DestroyReserved;

		for (size_t i = 0; i < m_baseComponents.size(); ++i)
		{
			if (m_baseComponents[i])
			{
				m_baseComponents[i]->Destroy();
			}
		}
		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (m_scripts[i])
			{
				m_scripts[i]->Destroy();
			}
		}

		Com_Transform* tr = Transform();
		Com_Transform* parent = Transform()->GetParent();

		//부모 Transform에서 자식의 주소를 제거
		if (parent)
		{
			parent->RemoveChildPtr(tr);
		}
		tr->DestroyChildsRecursive();
	}

	void GameObject::SetActiveRecursive(bool _bActive)
	{
		//제거 대기 상태가 아님 && 바꾸고자 하는 상태가 현재 상태와 다른 경우에만
		if (false == IsDestroyed() && IsActive() != _bActive)
		{
			if (_bActive)
			{
				m_state = eState::Active;

				//Scene이 작동중인 상태인데 아직 Awake 함수가 호출되지 않았을 경우 Awake 함수 호출
				if (m_ownerScene->IsAwaken() && false == m_bAwake)
				{
					m_bAwake = true;

					for (size_t i = 0; i < m_baseComponents.size(); ++i)
					{
						m_baseComponents[i]->Awake();
					}
				}

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if (m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
						m_baseComponents[i]->OnEnable();
				}
			}
			else
			{
				m_state = eState::InActive;

				for (size_t i = 0; i < m_baseComponents.size(); ++i)
				{
					if(m_baseComponents[i] && m_baseComponents[i]->IsEnabled())
						m_baseComponents[i]->OnDisable();
				}
			}
		}
	}

	iScript* GameObject::GetScript(const std::string_view _strKey)
	{
		iScript* retScript = nullptr;

		for (size_t i = 0; i < m_scripts.size(); ++i)
		{
			if (_strKey == m_scripts[i]->GetStrKey())
			{
				retScript = m_scripts[i];
				break;
			}
		}

		return retScript;
	}
}
