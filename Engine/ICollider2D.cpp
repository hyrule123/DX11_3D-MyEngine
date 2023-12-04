#include "PCH_Engine.h"
#include "ICollider2D.h"

#include "GameObject.h"

#include "RenderMgr.h"
#include "IScript.h"

#include "json-cpp/json.h"

namespace ehw
{
	uint ICollider2D::gColliderNumber = 0;
	ICollider2D::ICollider2D()
		: ICollider(eColliderType::None)
		, mTransform(nullptr)
		, mSize(float2::One)
		, mCenter(float2::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
	{
		mID = gColliderNumber++;
	}

	ICollider2D::ICollider2D(const ICollider2D& _collider)
		: ICollider(_collider.m_ComCategory)
		, mTransform(nullptr)
		, mSize(_collider.mSize)
		, mCenter(_collider.mCenter)
		, mbTrigger(false)
		, mID(0)
		, mRadius(_collider.mRadius)
	{
		mID = gColliderNumber++;
	}

	ICollider2D::~ICollider2D()
	{
	}

	eResult ICollider2D::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = ICollider::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;

		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(m_ComCategory));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mSize));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mCenter));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mPosition));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mRadius));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mbTrigger));

		return eResult::Success;
	}

	eResult ICollider2D::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = ICollider::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		const Json::Value& jVal = (*_pJVal);

		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(m_ComCategory));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mSize));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mCenter));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mPosition));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mRadius));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mbTrigger));

		mID = gColliderNumber++;

		if (GetOwner() && nullptr == mTransform)
		{
			Com_Transform* ownerTransform = GetOwner()->GetComponent<Com_Transform>();
			if (ownerTransform)
			{
				mTransform = ownerTransform;
			}
		}

		return eResult::Success;
	}

	void ICollider2D::Awake()
	{
		mTransform = GetOwner()->GetComponent<Com_Transform>();
	}

	void ICollider2D::Update()
	{
	}

	void ICollider2D::InternalUpdate()
	{
		float3 scale = mTransform->GetRelativeScale();
		scale *= float3(mSize.x, mSize.y, 1.0f);

		float3 rotation = mTransform->GetRelativeRotXYZ();

		float3 position = mTransform->GetRelativePos();
		float3 colliderPos = position + float3(mCenter.x, mCenter.y, 0.0f);
		mPosition = colliderPos;

		MATRIX scaleMatrix = MATRIX::CreateScale(scale);
		MATRIX rotationMatrix;
		rotationMatrix = MATRIX::CreateRotationX(rotation.x);
		rotationMatrix *= MATRIX::CreateRotationY(rotation.y);
		rotationMatrix *= MATRIX::CreateRotationZ(rotation.z);

		MATRIX positionMatrix;
		positionMatrix.Translation(float3(colliderPos.x, colliderPos.y, colliderPos.z));

		MATRIX worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		tDebugMesh meshAttribute = {};
		meshAttribute.position = float3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotatation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = m_ComCategory;

		RenderMgr::AddDebugMesh(meshAttribute);
	}


	void ICollider2D::OnCollisionEnter(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionEnter(_collider);
		}
	}

	void ICollider2D::OnCollisionStay(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionStay(_collider);
		}
	}

	void ICollider2D::OnCollisionExit(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionExit(_collider);
		}
	}

	void ICollider2D::OnTriggerEnter(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerEnter(_collider);
		}
	}

	void ICollider2D::OnTriggerStay(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerStay(_collider);
		}
	}

	void ICollider2D::OnTriggerExit(ICollider2D* _collider)
	{
		const std::span<IScript*> scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerExit(_collider);
		}
	}

}
