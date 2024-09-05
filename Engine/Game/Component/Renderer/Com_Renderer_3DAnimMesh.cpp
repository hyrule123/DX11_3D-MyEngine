#include "Engine/Game/Component/Renderer/Com_Renderer_3DAnimMesh.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Animator/Com_Animator3D.h"
#include "Engine/Game/Component/Transform.h"

#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"


namespace ehw
{
	Com_Renderer_3DAnimMesh::Com_Renderer_3DAnimMesh()
		: Com_Renderer_Mesh(Com_Renderer_3DAnimMesh::concrete_name)
	{
	}

	Com_Renderer_3DAnimMesh::~Com_Renderer_3DAnimMesh()
	{
	}

	void Com_Renderer_3DAnimMesh::Render()
	{
		//애니메이터가 없거나
		//3D 애니메이터가 아니거나
		//재생 중이 아니거나
		//->일반 Mesh 타입으로 렌더링
		auto animator = gameObject()->GetComponent<Com_Animator3D>();
		if (
			nullptr == animator ||
			false == animator->IsPlaying()
			)
		{
			Com_Renderer_Mesh::Render();
			return;
		}

		if (false == IsRenderReady())
			return;

		auto tr = gameObject()->GetComponent<Transform>();
		tr->bind_data();

		animator->bind_data();
		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				//메쉬 바인딩 - Render 내부에서 실행되도록 변경
				//GetMesh()->BindBuffer(i);

				//재질에 애니메이션 정보 넣어주고 바인딩
				mtrl->SetAnim3D(true);
				mtrl->SetBoneCount(GetMesh()->GetSkeleton()->GetBoneCount());
				mtrl->bind_data();

				// 사용할 메쉬 업데이트 및 렌더링
				GetMesh()->Render(i);

				//mtrl->UnbindData();
				mtrl->SetBoneCount(0);
				mtrl->SetAnim3D(false);
			}
		}

		animator->UnBindData();
	}
}

