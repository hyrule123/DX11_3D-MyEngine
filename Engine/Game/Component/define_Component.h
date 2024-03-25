#pragma once
#include "strKey_Component.h"



#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	STRKEY ComponentPrefix = "Com_";
	enum class eComponentCategory
	{
		UNKNOWN,
		Transform,
		Collider,
		Animator,
		Light,
		Camera,
		Renderer,

		AudioSource,
		AudioListener,

		BehaviorTree,

		BaseComponentEnd,

		Scripts = BaseComponentEnd,

		END
	};
	inline bool IsComponentCategoryValid(eComponentCategory _cat)
	{
		return ((eComponentCategory::UNKNOWN < _cat) && (_cat < eComponentCategory::END));
	}

	namespace strKey
	{
		STRKEY eComponentCategory_String[(int)eComponentCategory::END] =
		{
			"Transform",
			"Collider",
			"Animator",
			"Light",
			"Camera",
			"Renderer",

			"AudioSource",
			"AudioListener",

			"BehaviorTree",
			"Scripts"
		};
		
		inline STRKEY GetComponentCategoryName(eComponentCategory _category)
		{
			return eComponentCategory_String[(int)_category];
		}
	}
}
