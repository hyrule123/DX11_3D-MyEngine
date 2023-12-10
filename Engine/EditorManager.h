#pragma once
#include "guiChild.h"
#include "define_GPU.h"
#include "define_Struct.h"
#include "define_Macro.h"
#include <unordered_map>
#include "json-cpp/json-forwards.h"
#include "ImGuizmo.h"

namespace editor
{
	class EditorObject;
	class DebugObject;
	class EditorManager
	{
		friend class GameEngineWindow;
	public:
		static void Run();

		static inline void SetEnable(bool _bEnable);
		static inline bool GetEnable() { return mbEnable; }
		static inline void ToggleEnable() { SetEnable(!mbEnable); }

		static EditorBase* FindGuiWindow(const std::string_view _strKey);

		template <typename T>
		static T* AddGuiWindow();

		static const std::unordered_map<std::string, EditorBase*, ehw::tHashFunc_StringView, std::equal_to<>>&
			GetGUIs() { return mGuiWindows; }

	private:
		static void Init();
		static void Release();

		static void Update();
		static void InternalUpdate();
		static void Render();
		
		static void DebugRender(ehw::tDebugMesh& mesh);

		//Window 이름으로 저장된 Json 값이 있을 경우 로드함
		static Json::Value* CheckJsonSaved(const std::string& _strKey);

		static void InitGuiWindows();

		//=================== IMGUI ===========================
		static void ImGuiInitialize();
		static void ImGuiRelease();

		static void ImGuiNewFrame();
		static void ImGuiRender();
		
		static void AddGuiWindow(EditorBase* _pBase);

		static inline std::string CreateUniqueImGuiKey(const std::string_view _str, int i);

	private:
		static std::unordered_map<std::string, EditorBase*, ehw::tHashFunc_StringView, std::equal_to<>> mGuiWindows;

		static std::vector<EditorObject*> mEditorObjects;
		static std::vector<DebugObject*> mDebugObjects;

		//현재 GUI를 표시할지 여부
		static bool mbEnable;

		//GUI가 최초 1회 초기화 되었는지 여부
		static bool mbInitialized;

		static std::unique_ptr<Json::Value> mJsonUIData;

	private:
		static ImGuizmo::OPERATION mCurrentGizmoOperation;
		static void RenderGuizmo();
	};

	inline void EditorManager::SetEnable(bool _bEnable)
	{
		mbEnable = _bEnable;

		if (mbEnable && (false == mbInitialized))
		{
			EditorManager::Init();
			mbInitialized = true;
		}
	}


	template<typename T>
	inline T* EditorManager::AddGuiWindow()
	{
		static_assert(std::is_base_of_v<EditorBase, T>);

		//editor Child는 무조건 child로 들어가기위한 용도이므로 assert
		static_assert(false == std::is_base_of_v<guiChild, T>);

		T* retPtr = new T;

		Json::Value* pJval = CheckJsonSaved(retPtr->GetName());
		if (pJval)
		{
			retPtr->LoadJson(pJval);
		}

		AddGuiWindow(static_cast<EditorBase*>(retPtr));
		return retPtr;
	}

	inline std::string EditorManager::CreateUniqueImGuiKey(const std::string_view _str, int i)
	{
		std::string uniqStr;
		uniqStr.reserve(_str.size() + 5);
		uniqStr += _str;
		uniqStr += "##";
		uniqStr += std::to_string(i);
		return uniqStr;
	}
}

