#include "PCH_Engine.h"
#include "Com_Renderer_Mesh.h"

#include "GameObject.h"
#include "IAnimator.h"

namespace mh
{
	void Com_Renderer_Mesh::Render()
	{
		//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
		if (false == IsRenderReady())
			return;

		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != GetCurrentMaterial(i))
			{
				//메쉬 바인딩
				GetMesh()->BindBuffer(i);

				//재질 바인딩
				GetCurrentMaterial(i)->BindData();

				//메쉬 렌더링
				GetMesh()->Render(i);
			}
		}
	}
	void Com_Renderer_Mesh::RenderEnd()
	{
		if (false == IsRenderReady())
			return;

		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != GetCurrentMaterial(i))
			{
				//재질 바인딩
				GetCurrentMaterial(i)->UnBindData();
			}
		}
	}
}
