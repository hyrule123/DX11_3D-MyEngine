#include "Editor/DebugObject/SceneViewer.h"

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/Resource/Texture.h>


namespace ehw::editor
{
	SceneViewer::SceneViewer()
		: EditorWindow(name::GameViewer)
	{
	}

	SceneViewer::~SceneViewer()
	{
	}

	void SceneViewer::update_UI()
	{
		std::shared_ptr<Texture> renderTarget
			= RenderManager::get_inst().GetRenderTargetTex();

		std::shared_ptr<Texture> gameTex
			= std::make_shared<Texture>();
		gameTex->create(RenderManager::get_inst().GetResolutionX(), RenderManager::get_inst().GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		
		//61 번 셰이더 리소스 뷰 null초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		auto pContext = RenderManager::get_inst().Context();
		pContext->PSSetShaderResources(GPU::Register::t::guiGameTexture, 1u, &gameSRV);
		pContext->CopyResource(gameTex->get_texture().Get()
			, renderTarget->get_texture().Get());

		gameTex->bind_data_SRV(GPU::Register::t::guiGameTexture, eShaderStageFlag::Pixel);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image((void*)gameTex->GetSRV().Get(), panelSize
				, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintColor);
	}

}
