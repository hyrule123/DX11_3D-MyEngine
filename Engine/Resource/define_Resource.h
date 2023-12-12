#pragma once



#include "../define_Macro.h"
#include "../define_Enum.h"

namespace ehw
{
	enum class eResourceType
	{
		UNKNOWN = -1,
		Mesh,
		MeshData,

		Texture,
		Material,
		//Sound,
		/*Font,*/
		Animation,

		AudioClip,
		Prefab,
		
		GraphicsShader,
		ComputeShader,
		END,

	};

	namespace strKey
	{
		STRKEY DirName_Resource = "Res";
		//리소스 이름 겸 Res 폴더 내의 폴더명으로 사용
		STRKEY ArrResName[(int)eResourceType::END] =
		{
			"Mesh",
			"MeshData",

			"Texture",
			"Material",
			//"Sound",
			//"Font,",
			"Animation2D",

			"AudioClip",
			"Prefab",

			"Shader/Graphics",
			"Shader/Compute",
		};
		inline STRKEY GetResName(eResourceType _type) { return ArrResName[(int)_type]; }

		STRKEY DirName_CompiledShader = "Shader";
		STRKEY Ext_CompiledShader = ".cso";
		STRKEY Ext_Mesh = ".mesh";
		STRKEY Ext_MeshData = ".json";
		STRKEY Ext_Material = ".json";
		STRKEY Ext_Skeleton = ".bone";
		STRKEY Ext_Anim3D = ".a3d";
		STRKEY Ext_Tex[] =
		{
			
			".png",
			".dds",
			".tga",
			".bmp",
			".jpg",
		};
		constexpr size_t Ext_Tex_Size = sizeof(Ext_Tex) / sizeof(const char*);

		namespace Default
		{
			namespace mesh
			{
				STRKEY_DECLARE(PointMesh);
				STRKEY_DECLARE(RectMesh);
				STRKEY_DECLARE(DebugRectMesh);
				STRKEY_DECLARE(CircleMesh);
				STRKEY_DECLARE(CubeMesh);
				STRKEY_DECLARE(SphereMesh);
			}

			namespace material
			{
				STRKEY_DECLARE(RectMaterial);
				STRKEY_DECLARE(SpriteMaterial);
				STRKEY_DECLARE(UIMaterial);
				STRKEY_DECLARE(GridMaterial);
				STRKEY_DECLARE(DebugMaterial);
				STRKEY_DECLARE(ParticleMaterial);
				STRKEY_DECLARE(Basic3DMaterial);
				STRKEY_DECLARE(PostProcessMaterial);
				STRKEY_DECLARE(DefferedMaterial);
				STRKEY_DECLARE(MergeMaterial);
				STRKEY_DECLARE(LightDirMaterial);
				STRKEY_DECLARE(LightPointMaterial);
			}

			namespace texture
			{
				STRKEY DefaultSprite = "DefaultSprite.png";
				STRKEY CartoonSmoke = "particle\\CartoonSmoke.png";
				STRKEY noise_01 = "noise\\noise_01.png";
				STRKEY noise_02 = "noise\\noise_02.png";
				STRKEY noise_03 = "noise\\noise_03.jpg";
				STRKEY BasicCube = "Cube\\TILE_01.tga";
				STRKEY BasicCubeNormal = "Cube\\TILE_01_N.tga";
				STRKEY Brick = "Cube\\Brick.jpg";
				STRKEY Brick_N = "Cube\\Brick_N.jpg";
				STRKEY PaintTexture = "PaintTexture";
				STRKEY RenderTarget = "RenderTarget";
			}

			namespace shader
			{
				namespace graphics
				{
					STRKEY_DECLARE(RectShader);
					STRKEY_DECLARE(SpriteShader);
					STRKEY_DECLARE(UIShader);
					STRKEY_DECLARE(GridShader);
					STRKEY_DECLARE(DebugShader);
					STRKEY_DECLARE(NormalConvertShader);
					STRKEY_DECLARE(ParticleShader);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(Basic3DShader);
					STRKEY_DECLARE(DefferedShader);
					//STRKEY_DECLARE(LightShader);
					STRKEY_DECLARE(LightDirShader);
					STRKEY_DECLARE(LightPointShader);
					STRKEY_DECLARE(MergeShader);
				}
				namespace compute
				{
					STRKEY_DECLARE(ParticleCS);
					STRKEY_DECLARE(PostProcessShader);
					STRKEY_DECLARE(BasicShader);
					STRKEY_DECLARE(Animation3D);
					STRKEY_DECLARE(NormalConvert);
					STRKEY_DECLARE(GPUInitSetting);
				}
			}
		}

	}



}
