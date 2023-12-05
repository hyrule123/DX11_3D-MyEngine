#pragma once
#include "Entity.h"
#include "Layer.h"

namespace ehw
{
	class GameObject;
	class iScene 
		: public Entity
	{
	public:
		iScene();
		virtual ~iScene();

		//실제 로직은 여기서
		void SceneAwake();
		void SceneUpdate();
		void SceneInternalUpdate();
		void SceneRender();
		void SceneDestroy();


		//아래 함수를 재정의해서 씬을 커스터마이즈 하면 됨
		virtual void OnEnter() = 0;//리소스 로드

		virtual void Update() {};
		virtual void InternalUpdate() {};
		virtual void Render() {};
		virtual void Destroy() {};
		virtual void OnExit() {};
		
		//Add 'New' Game Object -> 이미 레이어에 들어갔었던 게임오브젝트는 이 함수를 사용하면 안됨
		inline void	AddGameObject(const eLayerType _type, GameObject* _gameObj);
		inline void	AddGameObjectHierarchy(const eLayerType _type, GameObject* _gameObj);

		inline void	ChangeGameObjectLayer(const eLayerType _targetLayer, GameObject* _gameObj);
		inline void	MoveGameObjectLayerHierarchy(const eLayerType _targetLayer, GameObject* _gameObj);

		Layer&							GetLayer(eLayerType _type) { return m_Layers[(uint)_type]; }
		std::vector<GameObject*>		GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const eLayerType _type);


		bool	IsAwaken() const { return m_bAwake; }


	private:
		std::array<Layer, (int)eLayerType::END> m_Layers;
		bool m_bAwake;
	};

	inline void iScene::AddGameObject(const eLayerType _type, GameObject* _gameObj)
	{
		ASSERT_DEBUG(eLayerType::None != _type, "레이어를 지정하지 않았습니다.");
		ASSERT_DEBUG(_gameObj, "GameObject가 nullptr 입니다.");
		GetLayer(_type).AddGameObject(_gameObj);
	}

	inline void iScene::AddGameObjectHierarchy(const eLayerType _type, GameObject* _gameObj)
	{
		ASSERT_DEBUG(eLayerType::None != _type, "레이어를 지정하지 않았습니다.");
		ASSERT_DEBUG(_gameObj, "GameObject가 nullptr 입니다.");

		std::vector<GameObject*> gameObjs{};
		_gameObj->GetGameObjectHierarchy(gameObjs);
		for (size_t i = 0; i < gameObjs.size(); ++i)
		{
			AddGameObject(_type, gameObjs[i]);
		}
	}

	inline void iScene::ChangeGameObjectLayer(const eLayerType _targetLayer, GameObject* _gameObj)
	{
		ASSERT(eLayerType::None != _targetLayer && _gameObj, "게임오브젝트가 없거나 목표 레이어가 설정되어 있지 않습니다.");

		eLayerType prevLayer = _gameObj->GetLayerType();
		if (eLayerType::None != prevLayer)
		{
			GetLayer(prevLayer).RemoveGameObject(_gameObj);
		}

		//새 레이어에 넣어준다.
		GetLayer(_targetLayer).AddGameObject(_gameObj);
	}

	inline void iScene::MoveGameObjectLayerHierarchy(const eLayerType _targetLayer, GameObject* _gameObj)
	{
		ASSERT(eLayerType::None != _targetLayer, "목표 레이어를 지정하지 않았습니다.");
		ASSERT(_gameObj, "게임오브젝트가 nullptr 입니다.");

		std::vector<GameObject*> gameObjs{};
		_gameObj->GetGameObjectHierarchy(gameObjs);
		for (size_t i = 0; i < gameObjs.size(); ++i)
		{
			ChangeGameObjectLayer(_targetLayer, gameObjs[i]);
		}
	}

}
