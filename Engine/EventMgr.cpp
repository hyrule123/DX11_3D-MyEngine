#include "PCH_Engine.h"
#include "EventMgr.h"

#include "AtExit.h"

#include "SceneMgr.h"
#include "GameObject.h"
#include "Prefab.h"


namespace ehw
{
	std::vector<EventMgr::tEvent>	EventMgr::mEvents{};
	std::vector<EventMgr::tEvent>	EventMgr::mLazyEvents{};
	bool							EventMgr::mbLevelModified{};

	bool EventMgr::Init()
	{
		AtExit::AddFunc(Release);
		return true;
	}

	void EventMgr::Release()
	{
		//이벤트 매니저에 이벤트가 남아있을 경우 여기에만 포인터가 남아 있으면 메모리 릭이 발생한다
		if (false == mEvents.empty())
		{
			size_t size = mEvents.size();
			for (size_t i = 0; i < size; ++i)
			{
				//오브젝트 생성 = 여기서만 주소를 들고 있으므로 메모리릭 발생 위험성 있음
				if (eEventType::SpawnGameObj == mEvents[i].Type)
				{
					GameObject* pObj = reinterpret_cast<GameObject*>(mEvents[i].lParam);
					SAFE_DELETE(pObj);
				}
			}
		}

		mEvents.clear();
		mLazyEvents.clear();
		mbLevelModified = false;
	}

	void EventMgr::Update()
	{
		mbLevelModified = false;
		ProcessLazyEvent();
		ProcessEvent();
	}

	void EventMgr::SpawnNewGameObj(const tEvent& _event)
	{
		GameObject* Obj = reinterpret_cast<GameObject*>(_event.lParam);
		eLayerType layerType = static_cast<eLayerType>(_event.wParam);

		//오브젝트가 nullptr일 경우
		//레이어가 설정되지 않았을 경우
		//ASSERT
		ASSERT(eLayerType::None != layerType, "레이어 타입을 지정하지 않았습니다.");
		ASSERT(Obj, "GameObject가 nullptr 입니다.");

		SceneMgr::GetActiveScene()->AddGameObjectHierarchy(layerType, Obj);
		mbLevelModified = true;
	}


	void EventMgr::DestroyGameObj(const tEvent& _event)
	{
		//lParam = GameObject Pointer
		//wParam = None
		GameObject* _pObj = reinterpret_cast<GameObject*>(_event.lParam);

		//이미 삭제 대기 상태에 들어가 있는 경우 추가로 등록하지 않음.
		//중복 삭제 방지(댕글링 포인터 위험)
		if (GameObject::eState::Dead == _pObj->GetState())
			return;

		_pObj->DestroyRecursive();

		//만약 부모 오브젝트는 삭제 대상이 아닐 경우 부모로부터 연결 해제
		GameObject* pParent = _pObj->GetParent();
		if (pParent && GameObject::eState::Dead != pParent->GetState())
		{
			pParent->RemoveChild(_pObj);
		}
	}

	void EventMgr::AddChildGameObj(const tEvent& _event)
	{
		//lParam = Parent GameObject*
		//wParam = Child GameObject*
		GameObject* pParent = reinterpret_cast<GameObject*>(_event.lParam);
		GameObject* pChild = reinterpret_cast<GameObject*>(_event.wParam);

		if (pParent && pChild)
		{
			pParent->AddChild(pChild);

			//새 오브젝트일 경우(start되지 않은 오브젝트일 경우 새 오브젝트라고 간주)
			if (false == pChild->IsStarted())
			{
				//부모의 레이어에 일괄적으로 넣어준다
				SceneMgr::GetActiveScene()->AddGameObjectHierarchy(pParent->GetLayerType(), pChild);
			}

			//이미 게임 안에 생성되었던 오브젝트라면 레이어만 옮겨준다
			else
			{
				std::vector<GameObject*> gameObjs;
				pChild->GetGameObjectHierarchy(gameObjs);
				IScene* scene = SceneMgr::GetActiveScene();
				for (size_t i = 0; i < gameObjs.size(); ++i)
				{
					SceneMgr::GetActiveScene()->ChangeGameObjectLayer(pParent->GetLayerType(), pChild);
				}
			}

		}
	}

	void EventMgr::MoveGameObjLayer(const tEvent& _event)
	{
		GameObject* pObj = reinterpret_cast<GameObject*>(_event.lParam);
		eLayerType targetLayer = static_cast<eLayerType>(_event.wParam);

		IScene* scene = SceneMgr::GetActiveScene();
		if (scene)
			scene->ChangeGameObjectLayer(targetLayer, pObj);
	}


	void EventMgr::ProcessEvent()
	{
		for (size_t i = 0; i < mEvents.size(); ++i)
		{
			switch (mEvents[i].Type)
			{
			case eEventType::SpawnGameObj:
				SpawnNewGameObj(mEvents[i]);
				break;
			case eEventType::DeleteGameObj:
				DestroyGameObj(mEvents[i]);
				break;
			case eEventType::AddChild:
				AddChildGameObj(mEvents[i]);
				break;
			case eEventType::MoveGameObjLayer:
				MoveGameObjLayer(mEvents[i]);
				break;

			default:
				break;
			}
		}
		mEvents.clear();
	}

	void EventMgr::ProcessLazyEvent()
	{
		//bDestroy 상태인 게임오브젝트를 cLevel에서 제거시킨다.(댕글링 포인터 제거)
		SceneMgr::GetActiveScene()->Destroy();

		//size_t size = mLazyEvents.size();
		//for (size_t i = 0; i < size; ++i)
		//{
		//	switch (mLazyEvents[i].Type)
		//	{

		//	default:
		//		break;
		//	}
		//}
		mLazyEvents.clear();
	}


	GameObject* EventMgr::SpawnGameObject(eLayerType _layer, GameObject* _gameObj)
	{
		ASSERT_DEBUG(eLayerType::None != _layer, "레이어를 지정하지 않았습니다.");
		ASSERT_DEBUG(nullptr != _gameObj, "게임오브젝트가 nullptr 입니다.");

		//Scene 시작 안됐을 경우 바로 넣어준다
		IScene* scene = SceneMgr::GetActiveScene();
		ASSERT(scene, "scene이 없습니다.");

		if (false == scene->IsInitialized())
		{
			scene->AddGameObject(_layer, _gameObj);
		}

		//Scene 시작됐을 경우에는 지연 삽입
		else
		{
			tEvent evn{};
			evn.Type = eEventType::SpawnGameObj;
			evn.lParam = reinterpret_cast<DWORD_PTR>(_gameObj);
			evn.wParam = static_cast<DWORD_PTR>(_layer);
			mEvents.push_back(evn);
		}

		return _gameObj;
	}


	void EventMgr::ChangeGameObjectLayer(eLayerType _layer, GameObject* _gameObj)
	{
		ASSERT(eLayerType::None != _layer && _gameObj, "목표 레이어가 없거나 게임오브젝트가 설정되어 있지 않습니다.");
		tEvent evn{};
		//lParam = GameObject Pointer
		//wParam = Target Layer
		evn.Type = eEventType::MoveGameObjLayer;
		evn.lParam = reinterpret_cast<DWORD_PTR>(_gameObj);
		evn.wParam = static_cast<DWORD_PTR>(_layer);
		mEvents.push_back(evn);
	}


	GameObject* EventMgr::SpawnGameObject(eLayerType _layer)
	{
		std::unique_ptr<GameObject> _obj = std::make_unique<GameObject>();
		SpawnGameObject(_layer, _obj.get());
		return _obj.release();
	}


	void EventMgr::DestroyGameObj(GameObject* _pObject)
	{
		tEvent evn = {};
		evn.Type = eEventType::DeleteGameObj;
		evn.lParam = reinterpret_cast<DWORD_PTR>(_pObject);
		evn.wParam = NULL;

		AddEvent(evn);
	}
	void EventMgr::AddChildGameObj(GameObject* _pParent, GameObject* _pChild)
	{
		tEvent evn = {};
		evn.Type = eEventType::AddChild;
		evn.lParam = reinterpret_cast<DWORD_PTR>(_pParent);
		evn.wParam = reinterpret_cast<DWORD_PTR>(_pChild);

		EventMgr::AddEvent(evn);
	}


	//GameObject* EventMgr::SpawnPrefab2D(std::shared_ptr<Prefab> _Prefab, const float2& _vWorldPosXY)
	//{
	//	return nullptr;
	//}
	//GameObject* EventMgr::SpawnPrefab(std::shared_ptr<Prefab> _Prefab, const float3& _vWorldPos)
	//{
	//	return nullptr;
	//}
	//GameObject* EventMgr::SpawnPrefab(std::shared_ptr<Prefab> _Prefab)
	//{
	//	return nullptr;
	//}

}

