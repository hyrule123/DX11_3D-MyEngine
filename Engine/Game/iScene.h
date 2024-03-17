#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/type_traits_Ex.h"

#include "Engine/define_Enum.h"

#include <queue>
#include <future>
#include <bitset>

namespace ehw
{

	class GameObject;
	class iScene 
		: public Entity
	{
	public:
		using GameObjects = std::vector<std::shared_ptr<GameObject>>;

		iScene();
		virtual ~iScene();

		//로직 호출
		void SceneAwake();
		void SceneUpdate();
		void SceneCollisionUpdate();
		void SceneFinalUpdate();
		void SceneRender();
		void SceneRemoveDestroyed();
		void SceneFrameEnd();


		//로직 재정의
		virtual void OnEnter() = 0;//리소스 로드

		virtual void Update() {}
		virtual void FinalUpdate() {}
		virtual void Render() {}
		virtual void RemoveDestroyed() {}
		virtual void FrameEnd() {}
		virtual void OnExit() {}

		bool	IsAwaken() const { return m_bAwake; }


		

		//Add 'New' Game Object -> 이미 레이어에 들어갔었던 게임오브젝트는 이 함수를 사용하면 안됨
		std::shared_ptr<GameObject> NewGameObject(const uint _layer = 0u, const std::string_view _name = "");
		std::shared_ptr<GameObject> AddGameObject(const std::shared_ptr<GameObject>& _gameObject, const uint _layer, const std::string_view _name);

		template <type_traits_Ex::is_enum_class_v T>
		inline std::shared_ptr<GameObject> NewGameObject(const T _layer, const std::string_view _name = "");

		//inline void AddGameObject(const std::shared_ptr<GameObject>& _gameObject, const uint32 _layer);

		std::vector<std::shared_ptr<GameObject>>		GetDontDestroyGameObjects();
		const GameObjects& GetGameObjects() { return m_gameObjects; }

		template <class F, class... Args>
		inline void AddFrameEndJob(F&& _func, Args&&... _args);

		//리턴값이 필요한 경우(future 변수 사용)
		//future를 쓰지 말고 레퍼런스를 보내내서 변수 변화를 보는게 더 나을지도...
		//template <class F, class... Args>
		//inline std::future<typename std::invoke_result<F, Args...>::type> 
		//	AddFrameEndJobReturn(F&& _func, Args&&... _args);

		
	private:
		GameObjects m_gameObjects;
		std::array<std::string, g_maxLayer>				m_layerNames;

		std::queue<std::function<void()>> m_FrameEndJobs;

		bool m_bAwake;
	};


	template<type_traits_Ex::is_enum_class_v T>
	inline std::shared_ptr<GameObject> iScene::NewGameObject(const T _layer, const std::string_view _name)
	{
		return NewGameObject(static_cast<std::underlying_type_t<T>>(_layer, _name));
	}


	template<class F, class ...Args>
	inline void iScene::AddFrameEndJob(F&& _func, Args && ..._args)
	{
		m_FrameEndJobs.push(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
	}


	//template<class F, class ...Args>
	//inline std::future<typename std::invoke_result<F, Args...>::type> iScene::AddFrameEndJobReturn(F&& _func, Args && ..._args)
	//{
	//	using return_type = std::invoke_result<F, Args...>::type;

	//	auto pFunc = std::make_shared<std::packaged_task<return_type()>>(
	//		std::bind(std::forward<F>(_func), std::forward<Args>(_args)...)
	//	);

	//	std::future<return_type> pFunc_result_future = pFunc->get_future();
	//	m_FrameEndJobs.push([pFunc]() { (*pFunc)(); });

	//	return pFunc_result_future;
	//}

}
