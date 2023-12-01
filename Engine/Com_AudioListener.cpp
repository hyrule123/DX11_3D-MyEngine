#include "PCH_Engine.h"

#include "Com_AudioListener.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "AudioMgr.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	

	Com_AudioListener::Com_AudioListener()
		: IComponent(eComponentType::AudioListener)
	{

	}

	Com_AudioListener::~Com_AudioListener()
	{

	}

	void Com_AudioListener::Awake()
	{

	}

	void Com_AudioListener::Update()
	{

	}

	void Com_AudioListener::FixedUpdate()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		float3 pos = tr->GetRelativePos();
		float3 foward = tr->Forward();
		float3 up = tr->Up();

		float3 vel = { 0.0f, 0.0f, 0.0f };
		AudioMgr::Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

}
