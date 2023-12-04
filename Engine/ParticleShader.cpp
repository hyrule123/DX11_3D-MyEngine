#include "PCH_Engine.h"

#include "ParticleShader.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"
#include "TimeMgr.h"

namespace ehw
{
	ParticleShader::ParticleShader()
		: ComputeShader(typeid(ParticleShader), uint3(128, 1, 1))
		, mBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::BindData()
	{
		mBuffer->BindDataUAV(0);
		mSharedBuffer->BindDataUAV(1);

		uint3 dataCounts = { mBuffer->GetStride(), 1u, 1u };
		CalculateGroupCount(dataCounts);

		return true;
	}

	void ParticleShader::UnBindData()
	{
		mBuffer->UnBindData();
		mSharedBuffer->UnBindData();
	}
}
