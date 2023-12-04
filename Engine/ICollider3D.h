#pragma once
#include "ICollider.h"

namespace ehw
{
	class ICollider3D : public ICollider
	{
	public:
		ICollider3D();
		ICollider3D(const ICollider3D& _collider);
		CLONE(ICollider3D);
		
		virtual ~ICollider3D();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void InternalUpdate() override { ASSERT(false, "미구현"); }; // 구현 안함

		void SetType(eColliderType _type) { m_ComCategory = _type; }

	private:
		eColliderType m_ComCategory;
	};
}


