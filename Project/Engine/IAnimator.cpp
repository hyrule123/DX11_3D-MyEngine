#include "PCH_Engine.h"
#include "IAnimator.h"

namespace mh
{
	IAnimator::IAnimator(define::eDimensionType _type)
		: IComponent(define::eComponentType::Animator)
		, mDimensionType(_type)
	{
	}


	void IAnimator::AddNotify(uint _frameIdx, const std::function<void()>& _func)
	{

	}
}

