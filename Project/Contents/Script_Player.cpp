#include "PCH_Contents.h"
#include "Script_Player.h"

#include <Engine/Animation3D.h>
#include <Engine/Com_Animator3D.h>
#include <Engine/GameObject.h>
#include <Engine/InputMgr.h>
#include <Engine/MeshData.h>
#include <Engine/ResourceMgr.h>

namespace mh
{
	void Script_Player::RequireComponent()
	{
		std::shared_ptr<MeshData> meshdata = ResourceMgr::Load<MeshData>("Player_Default");
		if (nullptr == meshdata || false == meshdata->Instantiate(GetOwner()))
		{
			ERROR_MESSAGE_W(L"메쉬 데이터 로드 실패");
		}

		GetOwner()->SetName("Player");
		GetOwner()->GetComponent<Com_Transform>()->SetRelativePosY(-100.f);
		GetOwner()->GetComponent<Com_Transform>()->SetRelativePosZ(750.f);
		auto* animator = GetOwner()->GetComponent<Com_Animator3D>();
		animator->Play("Evade");
		
		animator->AddEvent("Evade", 0, std::bind(&Script_Player::TestCallback, this));
	}

	void Script_Player::Init()
	{
		Com_Animator3D* animator = GetOwner()->GetComponent<Com_Animator3D>();
		if (animator)
		{
			//5animator->AddEvent()
		}
	}

	void Script_Player::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::P))
		{
			Com_Animator3D* animator = GetOwner()->GetComponent<Com_Animator3D>();
			if (animator)
			{
				animator->PlayNext();
			}
		}

		if (InputMgr::GetKeyDown(eKeyCode::E))
		{
		}
	}
	void Script_Player::TestCallback()
	{
		OutputDebugStringA("!!!!Called!!!\n");
	}
}

