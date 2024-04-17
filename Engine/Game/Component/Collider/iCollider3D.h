#pragma once
#include "Engine/Game/Component/Collider/iCollider.h"
#include "Engine/define_Enum.h"

#include "Engine/Game/Collision/PhysX.h"

namespace ehw
{
	class Com_Transform;
	class Collision3D;
	class iCollider3D : public iCollider
	{
	public:
		iCollider3D(eCollider3DType _col3dType);
		CLONE_DISABLE(iCollider3D);
		
		virtual ~iCollider3D();

		eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult(); };
		eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult(); };

		void Init() override;
		void Awake() override;

		MATRIX GetColliderMatrix() override { return MATRIX{}; };

		//Start 호출 되기 전에만 동작함.
		void SetSyncScaleToTransfom(bool enable) { m_isSyncScaleToTransform = enable; }

	protected:
		inline bool IsSyncScaleToTransform() const { return m_isSyncScaleToTransform; }
		inline Collision3D* GetCollision3D() { return m_col3dMgr; }

	private:
		eCollider3DType				m_colliderType;

		bool						m_isSyncScaleToTransform;

		Collision3D*				m_col3dMgr;
	};
}


