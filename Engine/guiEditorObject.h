#pragma once

#include "GameObject.h"

namespace gui
{
	class EditorObject : public ehw::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Awake();
		void Update();
		void FixedUpdate();
		void Render();

	private:
		
	};
}
