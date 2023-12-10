#pragma once
#include "EditorChild.h"
#include <functional>
#include "define_Struct.h"

namespace editor
{
	class TreeWidget : public EditorChild
	{
	public:
		struct tNode : public EditorEntity
		{
			tNode();
			virtual ~tNode();

			void UpdateUI();

			void SetData(ehw::tDataPtr _data) { mData = _data; }

			void SetStem(bool bEnable) { mbStem = bEnable; }

			void AddNode(tNode* node);
			const std::vector<tNode*>& GetChilds() { return m_Childs; }

			TreeWidget* mTreeWidget;
			ehw::tDataPtr mData;

			tNode* m_Parent;
			std::vector<tNode*> m_Childs;

			bool mbStem;
			bool mbSelected;
		};

		TreeWidget();
		virtual ~TreeWidget();

		virtual void UpdateUI() override;

		tNode* AddNode(tNode* parent, const std::string& name, ehw::tDataPtr data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(tNode* node);

		void SetEvent(EditorBase* widget, std::function<void(ehw::tDataPtr)> func)
		{
			mEventGUI = widget;
			mEvent = func;
		}


	private:
		std::unique_ptr<tNode> mRoot;

		bool mbDummyRootUse;
		tNode* mSelectedNode;

		EditorBase* mEventGUI;
		std::function<void(ehw::tDataPtr data)> mEvent;
	};
}
