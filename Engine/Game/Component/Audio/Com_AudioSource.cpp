#include "Com_AudioSource.h"

#include "Engine/Resource/AudioClip.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Transform.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	Com_AudioSource::Com_AudioSource()
		: Component(Com_AudioSource::concrete_name)
		, mAudioClip(nullptr)
	{
	}

	//eResult Com_AudioSource::SaveJson(Json::Value* _pJson)
	//{
	//	if (nullptr == _pJson)
	//	{
	//		return eResult::Fail;
	//	}
	//	eResult result = iComponent::SaveJson(_pJson);
	//	if (eResult_fail(result))
	//	{
	//		return result;
	//	}
	//	Json::Value& jVal = *_pJson;

	//	if (mAudioClip)
	//	{
	//		jVal[JSON_KEY(mAudioClip)] = mAudioClip->get_strkey();
	//	}

	//	return eResult::Success;
	//}

	//eResult Com_AudioSource::LoadJson(const Json::Value* _pJson)
	//{
	//	if (nullptr == _pJson)
	//	{
	//		return eResult::Fail;
	//	}
	//	eResult result = iComponent::LoadJson(_pJson);
	//	if (eResult_fail(result))
	//	{
	//		return result;
	//	}
	//	const Json::Value& jVal = *_pJson;

	//	if (jVal.isMember(JSON_KEY(mAudioClip)))
	//	{
	//		std::string strKey = jVal[JSON_KEY(mAudilClip)].asString();
	//		mAudioClip = ResourceManager<AudioClip>::GetInst().Load(strKey);
	//	}

	//	return eResult::Success;
	//}

	Com_AudioSource::~Com_AudioSource()
	{

	}

	eResult Com_AudioSource::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_AudioSource::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}


	void Com_AudioSource::final_update()
	{
		const auto& tr = gameObject()->GetComponent<Transform>();
		float3 pos = tr->get_local_position();
		float3 foward = tr->get_local_direction(eDirection::Forward);

		mAudioClip->Set3DAttributes(pos, foward);
	}


	void Com_AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void Com_AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void Com_AudioSource::SetLoop(bool _loop)
	{
		mAudioClip->SetLoop(_loop);
	}
}
