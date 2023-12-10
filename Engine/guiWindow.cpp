#include "PCH_Engine.h"
#include "guiWIndow.h"


namespace editor
{
	guiWindow::guiWindow(const std::string_view _strName)
		: EditorBase(_strName)
		, mWindowFlag()
	{
		SetSaveEnable(true);
	}
	guiWindow::~guiWindow()
	{
	}
	bool guiWindow::BeginUI()
	{
		bool retVal = ImGui::Begin(GetStrKey().c_str(), GetEnablePtr(), mWindowFlag);
		if(false == retVal)
		{
			ImGui::End();
		}

		return retVal;
	}
	void guiWindow::EndUI()
	{
		ImGui::End();
	}
}
