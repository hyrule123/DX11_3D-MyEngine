#pragma once
#include "EditorChild.h"

#include "guiTreeWidget.h"

namespace ehw
{
	class GameObject;
}

namespace editor
{
	class guiTree_GameObject : public EditorWindow
	{
	public:
		guiTree_GameObject();
		virtual ~guiTree_GameObject();

		virtual void Update() override;

		void GameobjectSelectCallback(ehw::tDataPtr _data);
		void InitializeScene();
		void AddGameObject(TreeWidget::tNode* parent, ehw::GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}
