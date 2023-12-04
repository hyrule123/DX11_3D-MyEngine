
#pragma once
#include "IComponent.h"
#include "ICollider2D.h"

namespace ehw
{
	class IScript : public IComponent
	{
	public:
		IScript();
		IScript(const IScript& _other) = default;
		virtual ~IScript();

		virtual void Init() {};
		virtual void Awake() override {};
		virtual void Update() override {};
		virtual void InternalUpdate() final {};

		virtual void OnCollisionEnter(ICollider2D* _collider) {};
		virtual void OnCollisionStay(ICollider2D* _collider) {};
		virtual void OnCollisionExit(ICollider2D* _collider) {};

		virtual void OnTriggerEnter(ICollider2D* _collider) {};
		virtual void OnTriggerStay(ICollider2D* _collider) {};
		virtual void OnTriggerExit(ICollider2D* _collider) {};

	private:
	};
}
