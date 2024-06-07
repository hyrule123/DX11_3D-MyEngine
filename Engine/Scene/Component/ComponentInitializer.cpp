
//=========================================================
//This Code is Automatically generated by CodeGenerator
//=========================================================


#include "Engine/PCH_Engine.h"
#include "Engine/Scene/Component/ComponentInitializer.h"

#include "Engine/Manager/ComponentManager.h"
#include "Engine/Scene/Component/iComponent.h"
#include "Engine/Scene/Component/strKey_Component.h"

#include "Engine/Scene/Component\Audio\Com_AudioListener.h"
#include "Engine/Scene/Component\Animator\Com_Animator2D.h"
#include "Engine/Scene/Component\Rigidbody\Com_Rigidbody_Static.h"
#include "Engine/Scene/Component\Animator\Com_Animator3D.h"
#include "Engine/Scene/Component\Collider\Com_Collider3D_Capsule.h"
#include "Engine/Scene/Component\Audio\Com_AudioSource.h"
#include "Engine/Scene/Component\Camera\Com_Camera.h"
#include "Engine/Scene/Component\Collider\Com_Collider3D_Cube.h"
#include "Engine/Scene/Component\Collider\Com_Collider2D_AABB.h"
#include "Engine/Scene/Component\Collider\Com_Collider3D_Shapes.h"
#include "Engine/Scene/Component\Collider\Com_Collider3D_Sphere.h"
#include "Engine/Scene/Component\Light\Com_Light3D.h"
#include "Engine/Scene/Component\Renderer\Com_Renderer_3DAnimMesh.h"
#include "Engine/Scene/Component\Renderer\Com_Renderer_Mesh.h"
#include "Engine/Scene/Component\Renderer\Com_Renderer_ParticleSystem.h"
#include "Engine/Scene/Component\Renderer\Com_Renderer_Sprite.h"
#include "Engine/Scene/Component\Renderer\Com_Renderer_UIBase.h"
#include "Engine/Scene/Component\Rigidbody\Com_Rigidbody_Dynamic.h"
#include "Engine/Scene/Component\StateMachine\Com_BehaviorTree.h"
#include "Engine/Scene/Component\Transform\Com_Transform.h"

#define CONSTRUCTOR_T(T) ComponentManager::GetInst().AddComponentConstructor<T>(strKey::component::##T)

namespace ehw
{
	void ComponentInitializer::Init()
	{
		CONSTRUCTOR_T(Com_AudioListener);
		CONSTRUCTOR_T(Com_Animator2D);
		CONSTRUCTOR_T(Com_Rigidbody_Static);
		CONSTRUCTOR_T(Com_Animator3D);
		CONSTRUCTOR_T(Com_Collider3D_Capsule);
		CONSTRUCTOR_T(Com_AudioSource);
		CONSTRUCTOR_T(Com_Camera);
		CONSTRUCTOR_T(Com_Collider3D_Cube);
		CONSTRUCTOR_T(Com_Collider2D_AABB);
		CONSTRUCTOR_T(Com_Collider3D_Shapes);
		CONSTRUCTOR_T(Com_Collider3D_Sphere);
		CONSTRUCTOR_T(Com_Light3D);
		CONSTRUCTOR_T(Com_Renderer_3DAnimMesh);
		CONSTRUCTOR_T(Com_Renderer_Mesh);
		CONSTRUCTOR_T(Com_Renderer_ParticleSystem);
		CONSTRUCTOR_T(Com_Renderer_Sprite);
		CONSTRUCTOR_T(Com_Renderer_UIBase);
		CONSTRUCTOR_T(Com_Rigidbody_Dynamic);
		CONSTRUCTOR_T(Com_BehaviorTree);
		CONSTRUCTOR_T(Com_Transform);
	}
}
