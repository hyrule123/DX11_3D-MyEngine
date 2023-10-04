#include "PCH_Engine.h"
#include "guiMgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "GameMainWindow.h"
#include "AtExit.h"

#include "Mesh.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "Com_Transform.h"
#include "Com_Renderer_Mesh.h"
#include "GridScript.h"
#include "Object.h"
#include "Application.h"
#include "GPUMgr.h"
#include "InputMgr.h"
#include "PathMgr.h"

#include "guiInspector.h"
#include "guiGame.h"
#include "guiBase.h"
#include "guiResources.h"
#include "guiMainMenu.h"
#include "guiConsole.h"
#include "guiList.h"
#include "guiTree_GameObject.h"
#include "guiFBXConverter.h"
#include "guiGraphicsShaderEditor.h"
#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiMaterialEditor.h"
#include "guiNormalConverter.h"
#include "guiUVCalculator.h"

#include "json-cpp/json.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace gui
{
	constexpr const char* imguiSaveINI = "imgui.ini";
	constexpr const char* imguiSaveJSON = "imgui.json";


	std::unordered_map<std::string, guiBase*, mh::define::tHashFunc_StringView, std::equal_to<>> guiMgr::mGuiWindows{};
	//std::vector<guiBase*> guiMgr::mGuiWindows{};
	std::vector<EditorObject*> guiMgr::mEditorObjects{};
	std::vector<DebugObject*> guiMgr::mDebugObjects{};

	bool guiMgr::mbEnable{};
	bool guiMgr::mbInitialized{};

	std::unique_ptr<Json::Value> guiMgr::mJsonUIData{};

	ImGuizmo::OPERATION guiMgr::mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	using namespace mh::define;
	using namespace mh::math;
	
	void guiMgr::Init()
	{
		AtExit::AddFunc(guiMgr::Release);

		::GameMainWindow::RegisterImGuiWndProc(ImGui_ImplWin32_WndProcHandler);

		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize((UINT)eColliderType::END);

		std::shared_ptr<mh::Mesh> rectMesh = mh::ResourceMgr::Find<mh::Mesh>(mh::strKey::Default::mesh::DebugRectMesh);
		std::shared_ptr<mh::Material> material = mh::ResourceMgr::Find<mh::Material>(mh::strKey::Default::material::DebugMaterial);

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mh::Com_Renderer_Mesh* renderer
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(rectMesh);

		std::shared_ptr<mh::Mesh> circleMesh = mh::ResourceMgr::Find<mh::Mesh>("CircleMesh");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		renderer
			= mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(circleMesh);


		//그리드 이쪽으로 옮겨줘야 한다.
		// Grid Object
		//EditorObject* gridObject = new EditorObject();
		//mh::Com_Renderer_Mesh* gridMr = gridObject->AddComponent<mh::Com_Renderer_Mesh>();
		//gridMr->SetMesh(mh::ResourceMgr::Find<mh::Mesh>(L"RectMesh"));
		//gridMr->SetMaterial(mh::ResourceMgr::Find<Material>(L"GridMaterial"));
		//mh::GridScript* gridScript = gridObject->AddComponent<mh::GridScript>();
		//gridScript->SetCamera(gMainCamera);

		//mEditorObjects.push_back(gridObject);

		ImGuiInitialize();

		InitGuiWindows();


		for (const auto& iter : mGuiWindows)
		{
			iter.second->InitRecursive();
		}
	}

	void guiMgr::Run()
	{
		if (
			mh::InputMgr::GetKey(mh::eKeyCode::LCTRL)
			&&
			mh::InputMgr::GetKey(mh::eKeyCode::LSHIFT)
			&&
			mh::InputMgr::GetKeyDown(mh::eKeyCode::E)
			)
		{
			gui::guiMgr::ToggleEnable();
		}


		if (mh::InputMgr::GetKey(mh::eKeyCode::Z))
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}
		if (mh::InputMgr::GetKey(mh::eKeyCode::X))
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}

		if (false == mbEnable)
			return;
		Update();
		FixedUpdate();
		Render();

		mbInitialized = true;
	}


	void guiMgr::Update()
	{
		ImGuiNewFrame();

		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}

	void guiMgr::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}

		for (const auto& guiPair : mGuiWindows)
		{
			guiPair.second->FixedUpdate();
		}
	}

	void guiMgr::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		auto& DebugMeshes = mh::RenderMgr::GetDebugMeshes();
		for ( mh::tDebugMesh& mesh : DebugMeshes)
		{
			DebugRender(mesh);
		}
		DebugMeshes.clear();

		ImGuiRender();
	}

	void guiMgr::Release()
	{
		if (false == mbInitialized)
			return;

		//IMGUI 내부 세팅 저장
		const std::fs::path& saveDir = mh::PathMgr::GetResPathRelative();
		std::fs::path savePath = saveDir / imguiSaveINI;
		ImGui::SaveIniSettingsToDisk(savePath.string().c_str());

		//IMGUI 프로젝트 세팅 저장
		savePath.remove_filename();
		savePath /= imguiSaveJSON;
		for (const auto& guiPair : mGuiWindows)
		{
			if (guiPair.second)
			{
				if (guiPair.second->IsSaveEnable())
				{
					//한 파일에 몰아서 저장
					Json::Value& saveVal = (*mJsonUIData.get())[guiPair.first];
					guiPair.second->SaveJson(&saveVal);
				}
				delete guiPair.second;
			}
		}
		mGuiWindows.clear();


		//json 저장
		std::ofstream ofs(savePath);
		if (ofs.is_open())
		{
			ofs << (*mJsonUIData.get());
			ofs.close();
		}
		mJsonUIData.reset();

		
		for (auto& obj : mEditorObjects)
		{
			SAFE_DELETE(obj);
		}

		SAFE_DELETE(mDebugObjects[(UINT)eColliderType::Rect]);
		SAFE_DELETE(mDebugObjects[(UINT)eColliderType::Circle]);

		mEditorObjects.clear();
		mDebugObjects.clear();

		ImGuiRelease();
	}

	void guiMgr::DebugRender(mh::tDebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];
		
		mh::Com_Transform* tr = debugObj->GetComponent<mh::Com_Transform>();
		tr->SetRelativePos(mesh.position);
		tr->SetRelativeRotXYZ(mesh.rotatation);
		

		if (mesh.type == eColliderType::Rect)
			tr->SetRelativeScale(mesh.scale);
		else
			tr->SetRelativeScale(Vector3(mesh.radius));


		mh::IRenderer* renderer = debugObj->GetComponent<mh::IRenderer>();
		mh::Com_Camera* mainCam = mh::RenderMgr::GetMainCam();

		tr->FixedUpdate();

		mh::Com_Camera::SetGpuViewMatrix(
			mainCam->GetViewMatrix());
		mh::Com_Camera::SetGpuProjectionMatrix(mainCam->GetProjectionMatrix());

		debugObj->Render();
	}

	Json::Value* guiMgr::CheckJsonSaved(const std::string& _strKey)
	{
		Json::Value* retJson = nullptr;

		if (mJsonUIData->isMember(_strKey))
		{
			retJson = &((*mJsonUIData)[_strKey]);
		}

		return retJson;
	}

	void guiMgr::InitGuiWindows()
	{
		AddGuiWindow<guiMainMenu>();

		AddGuiWindow<guiInspector>();

		AddGuiWindow<guiTree_GameObject>();

		AddGuiWindow<guiResources>();

		AddGuiWindow<guiGraphicsShaderEditor>();

		AddGuiWindow<guiFBXConverter>();

		AddGuiWindow<guiMaterialEditor>();

		AddGuiWindow<guiNormalConverter>();

		AddGuiWindow<guiUVCalculator>();
	}

	void guiMgr::ImGuiInitialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		//내부 세팅 로드
		const std::fs::path& saveDir = mh::PathMgr::GetResPathRelative();
		std::fs::path savePath = saveDir / imguiSaveINI;
		if (std::fs::exists(savePath))
		{
			ImGui::LoadIniSettingsFromDisk(savePath.string().c_str());
		}
		

		//프로젝트 세팅 로드
		mJsonUIData = std::make_unique<Json::Value>();
		savePath.remove_filename();
		savePath /= imguiSaveJSON;
		std::ifstream ifs(savePath);
		if (ifs.is_open())
		{
			ifs >> *mJsonUIData;
			ifs.close();
		}


		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(mh::Application::GetHwnd());
		ImGui_ImplDX11_Init(mh::GPUMgr::Device().Get()
			, mh::GPUMgr::Context().Get());



		//설정 파일들 로드
		//TODO: 여기
		//std::filesystem::path origDir = mh::PathMgr::GetInst()->GetPathRel_Content();

		//origDir /= DIRECTORY_NAME::SAVED_SETTING;
		//std::filesystem::path fullPath = origDir / "imgui.ini";
		//io.IniFilename = NULL;

		//ImGui::LoadIniSettingsFromDisk(fullPath.string().c_str());

		//fullPath.clear();
		//fullPath = origDir;
		//fullPath /= "ImGuiSave.json";

		//std::ifstream loadfile(fullPath);
		//if (true == loadfile.is_open())
		//{
		//	loadfile >> m_SavedUIData;
		//	loadfile.close();
		//}


		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != nullptr);

		// Our state

	}

	void guiMgr::ImGuiNewFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		RenderGuizmo();

		ImGuiIO io = ImGui::GetIO();
	}

	void guiMgr::ImGuiRender()
	{
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void guiMgr::AddGuiWindow(guiBase* _pBase)
	{
		//최상위 윈도우는 이름 자체가 고유값이여야 함
		const std::string_view guiName = _pBase->GetKey();
		guiBase* findPtr = FindGuiWindow(guiName);
		if (findPtr)
		{
			_pBase->MakeUniqueKeyByName();
		}

		mGuiWindows.insert(std::make_pair(_pBase->GetKey(), _pBase));

		if (mbInitialized)
		{
			_pBase->InitRecursive();
		}
	}

	void guiMgr::RenderGuizmo()
	{
		mh::GameObject* targetgameobject = mh::RenderMgr::GetInspectorGameObject();

		if (!targetgameobject)
		{
			return;
		}

		mh::Com_Camera* mainCam = mh::RenderMgr::GetMainCam();

		if (!mainCam)
		{
			return;
		}

		//ImGuizmo::SetOrthographic(false);
		//ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

		mh::Com_Transform* tr = targetgameobject->GetComponent<mh::Com_Transform>();
		if (nullptr == tr)
		{
			return;
		}

		constexpr float kOffsetX = 60.f;
		constexpr float kOffsetY = 45.f;
		const float		x = ImGui::GetWindowPos().x - 60;
		const float		y = ImGui::GetWindowPos().y - 45;
		const float		width = ImGui::GetWindowViewport()->Size.x;
		const float		height = ImGui::GetWindowViewport()->Size.y;
		ImGuizmo::SetRect(x, y, width, height);

		MATRIX view = mainCam->GetViewMatrix();
		MATRIX projection = mainCam->GetProjectionMatrix();
		MATRIX worldMatrix = tr->GetWorldMat();

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(&worldMatrix.m[0][0], matrixTranslation, matrixRotation, matrixScale);

		MATRIX matTranslation = MATRIX::CreateTranslation(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
		MATRIX matRotation = MATRIX::CreateFromYawPitchRoll(matrixRotation[1], matrixRotation[0], matrixRotation[2]);
		MATRIX matScale = MATRIX::CreateScale(matrixScale[0], matrixScale[1], matrixScale[2]);

		worldMatrix = matScale * matRotation * matTranslation;

		ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &worldMatrix.m[0][0]);

		if (ImGuizmo::IsUsing())
		{
			Vector3 position{};
			Quaternion rotation{};
			Vector3 scale{};
			worldMatrix.Decompose(scale, rotation, position);


			if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
			{
				tr->SetRelativePos(position);
			}
			else if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
			{
				// 회전 작동 오류있음
				// 현재사용 X
				Matrix mat = Matrix::CreateFromQuaternion(rotation);
				float x, y, z;

				if (mat._13 > 0.998f)
				{
					y = atan2f(mat._21, mat._22);
					x = XM_PI / 2.0f;
					z = 0;
				}
				else if (mat._13 < -0.998f)
				{
					y = atan2f(mat._21, mat._22);
					x = -XM_PI / 2.0f;
					z = 0;
				}
				else
				{
					y = atan2f(-mat._23, mat._33);
					x = asinf(mat._13);
					z = atan2f(-mat._12, mat._11);
				}

				Vector3 axisRotation(x, y, z);
				tr->SetRelativeRotXYZ(axisRotation);

			}
			else if (mCurrentGizmoOperation == ImGuizmo::SCALE)
			{
				tr->SetRelativeScale(scale);
			}
			else
			{
				return;
			}
		}
	}

	void guiMgr::ImGuiRelease()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}


	guiBase* guiMgr::FindGuiWindow(const std::string_view _strKey)
	{
		guiBase* pui = nullptr;

		const auto& iter = mGuiWindows.find(_strKey);
		if (iter != mGuiWindows.end())
		{
			pui = iter->second;
		}
		
		return pui;
	}
}
