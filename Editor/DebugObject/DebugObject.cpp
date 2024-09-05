#include "Editor/DebugObject/DebugObject.h"

#include <Engine/Game/Component/Component.h>

namespace ehw::editor
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::final_update()
	{
		const auto& vecCom = GetComponents();
		for (size_t i = 0; i < vecCom.size(); ++i)
		{
			if (nullptr == vecCom[i])
				continue;

			vecCom[i]->final_update();
		}
	}
}
