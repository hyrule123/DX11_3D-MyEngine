#pragma once
#include "IResource.h"
#include "AudioMgr.h"



namespace mh
{
	

	class AudioClip : public IResource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual eResult Load(const std::fs::path& _filePath) override;
		
		void Play();
		void Stop();
		void Set3DAttributes(const float3 _pos, const float3 _vel);
		void SetLoop(bool _loop) { mbLoop = _loop; }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

