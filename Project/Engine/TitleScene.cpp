#include "EnginePCH.h"

#include "TitleScene.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Renderer.h"
#include "GameResources.h"
#include "Texture.h"
#include "PlayerScript.h"
#include "Camera.h"
#include "CameraScript.h"
#include "SpriteRenderer.h"
#include "GridScript.h"
#include "Object.h"
#include "Input.h"
#include "Collider2D.h"
#include "Player.h"
#include "Monster.h"
#include "CollisionManager.h"
#include "Animator.h"
#include "Light.h"
#include "PaintShader.h"
#include "ParticleSystem.h"

namespace mh
{
	TitleScene::TitleScene()
		: Scene(eSceneType::Tilte)
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera);
		cameraObj->SetName("MainCamera");
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -20.0f));
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Perspective);
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		renderer::gMainCamera = cameraComp;


		GameObject* player = object::Instantiate<GameObject>(eLayerType::Player);
		player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
		player->GetComponent<Transform>()->SetScale(Vector3(5.0f, 5.0f, 5.0f));
		//player->GetComponent<Transform>()->SetRotation(Vector3(15.0f, 45.0f, 0.0f));
		player->SetName("Player");
		MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMaterial(GameResources::Find<Material>("BasicMaterial"));
		mr->SetMesh(GameResources::Find<Mesh>("CubeMesh"));
		player->AddComponent<PlayerScript>();
		//mr->SetMesh(GameResources::Find<Mesh>(L"SphereMesh"));

		////paint shader
		//std::shared_ptr<PaintShader> paintShader = GameResources::Find<PaintShader>(L"PaintShader");
		////L"SmileTexture"
		//std::shared_ptr<Texture> paintTex = GameResources::Find<Texture>(L"PaintTexture");
		//paintShader->SetTarget(paintTex);
		//paintShader->OnExcute();

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName("directionalLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 100.0f, 0.0f));
			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(45.0f, 0.0f, 0.0f));

			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetSpecular(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			lightComp->SetAmbient(Vector4(0.15f, 0.15f, 0.15f, 1.0f));
		}

		/*{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName(L"PointLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(20.0f);
			lightComp->SetDiffuse(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player);
			directionalLight->SetName(L"PointLight");

			directionalLight->GetComponent<Transform>()->SetPosition(Vector3(6.0f, 0.0f, 0.0f));
			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetRadius(20.0f);
			lightComp->SetDiffuse(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		}*/


		//// Main Camera Game Object
		//GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera);
		//cameraObj->SetName(L"MainCamera");
		//Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		////cameraComp->RegisterCameraInRenderer();
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		//cameraObj->AddComponent<CameraScript>();
		//gMainCamera = cameraComp;
		//

		//


		//Transform* zeldaTr;
		////SMILE RECT
		//{
		//	Player* obj = object::Instantiate<Player>(eLayerType::Player);
		//	
		//	renderer::gInspectorGameObject = obj;
		//	obj->SetName(L"Zelda");
		//	zeldaTr = obj->GetComponent<Transform>();
		//	zeldaTr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		//	zeldaTr->SetRotation(Vector3(0.0f, 0.0f, 45.0f));
		//	zeldaTr->SetScale(Vector3(200.0f, 200.0f, 1.0f));
		//	Collider2D* collider = obj->AddComponent<Collider2D>();
		//	collider->SetType(eColliderType::Rect);
		//	collider->SetSize(Vector2(1.5f, 0.5f));

		//	Animator* animator = obj->AddComponent<Animator>();
		//	std::shared_ptr<Texture> texture = GameResources::Load<Texture>(L"Zelda", L"Zelda.png");
		//	animator->Create(L"Idle", texture, Vector2(0.0f, 0.0f), Vector2(120.0f, 130.0f), Vector2::Zero, 3, 0.1f);
		//	animator->Create(L"MoveDown", texture, Vector2(0.0f, 520.0f), Vector2(120.0f, 130.0f), Vector2::Zero, 8, 0.1f);
		//	animator->Create(L"MoveLeft", texture, Vector2(0.0f, 650.0f), Vector2(120.0f, 130.0f), Vector2::Zero, 10, 0.1f);

		//	animator->Play(L"MoveLeft", true);

		//	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
		//	std::shared_ptr<Material> mateiral = GameResources::Find<Material>(L"SpriteMaterial");
		//	mr->SetMaterial(mateiral);
		//	std::shared_ptr<Mesh> mesh = GameResources::Find<Mesh>(L"RectMesh");
		//	mr->SetMesh(mesh);
		//	obj->AddComponent<PlayerScript>();
		//	object::DontDestroyOnLoad(obj);
		//}

		////SMILE RECT
		//{
		//	Player* obj = object::Instantiate<Player>(eLayerType::Player);
		//	obj->SetName(L"SMILE");
		//	Transform* tr = obj->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(2.0f, 0.0f, 5.0f));
		//	//tr->SetParent(zeldaTr);
		//	//tr->SetScale(Vector3(2.0f, 1.0f, 1.0f));
		//	//tr->SetRotation(Vector3(0.0f, 0.0f, XM_PIDIV2 / 2.0f));
		//	//tr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		//	Collider2D* collider = obj->AddComponent<Collider2D>();
		//	collider->SetSize(Vector2(2.0f, 2.0f));
		//	collider->SetType(eColliderType::Rect);
		//	//collider->SetCenter(Vector2(0.2f, 0.2f));
		//	//collider->SetSize(Vector2(1.5f, 1.5f));

		//	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
		//	std::shared_ptr<Material> mateiral = GameResources::Find<Material>(L"RectMaterial");
		//	mr->SetMaterial(mateiral);
		//	std::shared_ptr<Mesh> mesh = GameResources::Find<Mesh>(L"RectMesh");
		//	mr->SetMesh(mesh);
		//	object::DontDestroyOnLoad(obj);
		//}

		////tParticle
		//{
		//	Player* obj = object::Instantiate<Player>(eLayerType::tParticle);
		//	obj->SetName(L"PARTICLE");
		//	Transform* tr = obj->GetComponent<Transform>();
		//	tr->SetPosition(Vector3(0.0f, 0.0f, 100.0f));
		//	obj->AddComponent<ParticleSystem>();
		//}

		////post process object
		//{
		//	GameObject* obj = object::Instantiate<GameObject>(eLayerType::PostProcess);
		//	obj->SetName(L"PostProcessGameObject");
		//	zeldaTr = obj->GetComponent<Transform>();
		//	zeldaTr->SetPosition(Vector3(0.0f, 0.0f, 19.0f));
		//	zeldaTr->SetScale(Vector3(200.0f, 200.0f, 1.0f));

		//	Collider2D* collider = obj->AddComponent<Collider2D>();
		//	collider->SetType(eColliderType::Rect);
		//	//collider->SetSize(Vector2(1.0f, 0.5f));

		//	MeshRenderer* mr = obj->AddComponent<MeshRenderer>();
		//	std::shared_ptr<Material> mateiral = GameResources::Find<Material>(L"PostProcessMaterial");
		//	mr->SetMaterial(mateiral);


		//	std::shared_ptr<Mesh> mesh = GameResources::Find<Mesh>(L"RectMesh");
		//	mr->SetMesh(mesh);
		//}

		Scene::Initialize();
	}
	void TitleScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Play);
		}

		Scene::Update();
	}
	void TitleScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}
	void TitleScene::Render()
	{
		Scene::Render();
	}
	void TitleScene::OnEnter()
	{
	}
	void TitleScene::OnExit()
	{
	}
}
