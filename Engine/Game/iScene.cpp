#include "Engine/Game/iScene.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Collision/CollisionSystem.h"

namespace ehw
{
	iScene::iScene()
		: m_gameObjects{}
		, m_delayedAddQueue{}
		, m_FrameEndJobs{}
		, m_bAwake{false}
		, m_layerNames{}
		, m_collisionSystem{}
	{
		m_layerNames[0] = "Default";
	}

	iScene::~iScene()
	{
	}


	void iScene::SceneInit()
	{
		tDesc desc{};
		Init(desc);

		if (desc.EnableCollision2D || desc.EnableCollision3D)
		{
			m_collisionSystem = std::make_unique<CollisionSystem>(this);

			if (desc.EnableCollision2D)
			{
				m_collisionSystem->CreateCollision2D();
			}
			if (desc.EnableCollision3D)
			{
				m_collisionSystem->CreateCollision3D();
			}
		}
	}

	void iScene::SceneAwake()
	{
		if (m_bAwake)
		{
			return;
		}

		m_bAwake = true;
		
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Awake();
		}
	}
	void iScene::SceneUpdate()
	{
		Update();

		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Update();
		}

		if (m_collisionSystem)
		{
			m_collisionSystem->Update();
		}
	}


	void iScene::SceneFinalUpdate()
	{
		FinalUpdate();
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FinalUpdate();
		}
	}
	void iScene::SceneRender()
	{
		Render();
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Render();
		}
	}

	void iScene::SceneFrameEnd()
	{
		FrameEnd();

		//각 GameObject에 대해 frameend 호출
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FrameEnd();
		}

		if(m_collisionSystem)
		{
			m_collisionSystem->FrameEnd();
		}

		//FrameEnd에 예약해놓은 함수 호출
		for (size_t i = 0; i < m_FrameEndJobs.size(); ++i)
		{
			m_FrameEndJobs[i]();
		}
		m_FrameEndJobs.clear();

		//마지막으로 Scene 작동 중 추가된 GameObject를 추가
		AddGameObjectsInternal(m_delayedAddQueue);
	}


	void iScene::Destroy()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Destroy();
		}
	}

	GameObject* iScene::AddGameObject(std::unique_ptr<GameObject>& _newObject, const uint _layer)
	{
		GameObject* ret = nullptr;
		//true가 반환되면 에러 있다는 뜻
		if (SetGameObjectInfo(_newObject, _layer))
		{
			return ret;
		}

		ret = _newObject.get();

		if (false == m_bAwake)
		{
			AddGameObjectInternal(_newObject);
		}

		//Scene이 재생 중일 경우에는 한 프레임이 끝난 후 추가한다.
		else
		{
			std::unique_ptr<GameObject> temp{};
			temp.swap(_newObject);
			m_delayedAddQueue.push_back(std::move(temp));
		}

		return ret;
	}

	size_t iScene::AddGameObjects(GameObjects& _gameObjects, const uint _layer)
	{
		size_t inserted = 0;

		//복사
		GameObjects gameObjects{};
		gameObjects.swap(_gameObjects);

		std::function<bool(std::unique_ptr<GameObject>&)> predFunc =
			std::bind(&iScene::SetGameObjectInfo, this, std::placeholders::_1, _layer);
		
		std::erase_if(gameObjects, predFunc);

		inserted = gameObjects.size();

		if (m_bAwake)
		{
			m_delayedAddQueue.insert(m_delayedAddQueue.end()
				, std::make_move_iterator(gameObjects.begin())
				, std::make_move_iterator(gameObjects.end())
			);
		}
		else
		{
			AddGameObjectsInternal(gameObjects);
		}

		return inserted;
	}

	std::vector<std::unique_ptr<GameObject>> iScene::GetDontDestroyGameObjects()
	{
		std::vector<std::unique_ptr<GameObject>> dontGameObjs{};

		//false인 것들이 뒤에 모임
		auto iter = std::partition(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::unique_ptr<GameObject>& _gameObj)->bool
			{
				return !_gameObj->IsDontDestroyOnLoad();
			}
		);

		//resize 해야 할 사이즈 미리 받아놓고
		size_t iterPos = iter - m_gameObjects.begin();

		dontGameObjs.insert(dontGameObjs.end(),
			std::make_move_iterator(iter),
			std::make_move_iterator(m_gameObjects.end())
		);

		//resize 해주고
		m_gameObjects.resize(iterPos);

		return dontGameObjs;
	}

	void iScene::RemoveDestroyed()
	{
		//Destroy 상태의 Object 제거
		std::erase_if(m_gameObjects,
			[](const std::unique_ptr<GameObject>& _obj)->bool
			{
				GameObject::eState state = _obj->GetState();
				if (GameObject::eState::Destroy == state)
				{
					return true;
				}
				else
				{
					_obj->RemoveDestroyed();
				}

				return false;
			}
		);
	}

	bool iScene::SetGameObjectInfo(std::unique_ptr<GameObject>& _obj, const uint _layer)
	{
		if (nullptr == _obj)
		{
			return true;
		}
		else if (false == IsLayerValid(_layer))
		{
			return true;
		}

		//레이어는 일단 변경시켜줌
		_obj->SetLayer(_layer);

		//OwnerScene이 이미 this가 되었다는건 이미 m_gameObjects에 들어왔다는 뜻
		//release를 통해서 자동 메모리 관리를 해제하고 true를 반환한다.
		if (this == _obj->GetScene())
		{
			_obj.release();
			return true;
		}
		else
		{
			_obj->SetScene(this);
		}

		return false;
	}

	void iScene::AddGameObjectInternal(std::unique_ptr<GameObject>& _obj)
	{
		std::unique_ptr<GameObject> temp{};
		temp.swap(_obj);
		m_gameObjects.push_back(std::move(temp));
		if (m_bAwake)
		{
			m_gameObjects.back()->Awake();
		}
	}

	void iScene::AddGameObjectsInternal(GameObjects& _from)
	{
		auto iter = m_gameObjects.insert(m_gameObjects.end()
			, std::make_move_iterator(_from.begin())
			, std::make_move_iterator(_from.end())
		);
		_from.clear();

		for (iter; iter != m_gameObjects.end(); ++iter)
		{
			if (m_bAwake)
			{
				(*iter)->Awake();
			}
		}
	}
}
