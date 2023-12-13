
#include "PathManager.h"



#include "../Util/AtExit.h"

namespace ehw
{
	std::filesystem::path PathManager::mAbsoluteResPath{};
	std::filesystem::path PathManager::mRelativeResPath{};
	std::filesystem::path PathManager::mRelativePathContent[(int)eResourceType::END]{};
	std::filesystem::path PathManager::mRelativePath_ShaderCSO{};

	void PathManager::Init()
	{
		AtExit::AddFunc(PathManager::Release);

		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행
		mAbsoluteResPath = std::filesystem::current_path().parent_path().parent_path().parent_path();
		mAbsoluteResPath /= strKey::DirName_Resource;
		if (false == std::fs::exists(mAbsoluteResPath))
		{
			std::fs::create_directories(mAbsoluteResPath);
		}

		mRelativeResPath = "../../../";
		mRelativeResPath /= ehw::strKey::DirName_Resource;
		if (false == std::fs::exists(mRelativeResPath))
		{
			std::fs::create_directories(mRelativeResPath);
		}

		mRelativePath_ShaderCSO = ".";
		mRelativePath_ShaderCSO /= ehw::strKey::DirName_CompiledShader;
		if (false == std::fs::exists(mRelativePath_ShaderCSO))
		{
			std::fs::create_directories(mRelativePath_ShaderCSO);
		}

		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePathContent[i] = mRelativeResPath;
			mRelativePathContent[i] /= ehw::strKey::ArrResName[i];

			if (false == std::fs::exists(mRelativePathContent[i]))
			{
				std::fs::create_directories(mRelativePathContent[i]);
			}
		}

	}


	void PathManager::Release()
	{
		mAbsoluteResPath.clear();
		mRelativeResPath.clear();
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePathContent[i].clear();
		}

		mRelativePath_ShaderCSO.clear();
	}

}

