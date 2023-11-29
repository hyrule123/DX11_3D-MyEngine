#include "Resource.hlsli"
#include "Func_Bitwise.hlsli"

//초기화해야 하는 값이 있으면 여기서 초기화.
//또는 디버그 용도로 사용
//현재 하는 작업은 Endianness 확인 하나임.

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (all(uint3(0u, 0u, 0u) != DTid))
		return;
	
	if(gInitSettingRW[0].bCPULittleEndian)
	{
		if (gInitSettingRW[0].GPUEndianessTestVar & 0x000000ff)
		{
			gInitSettingRW[0].bGPULittleEndian = 0xffffffff;
		}
		else
		{
			if (gInitSettingRW[0].GPUEndianessTestVar & 0xff000000)
			{
				gInitSettingRW[0].bGPULittleEndian = 0xffffffff;
			}
		}
	}
	
	
	if (gInitSettingRW[0].bGPULittleEndian)
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * i;
			gInitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			gInitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}

		for (uint j = 0u; j < 2u; ++j)
		{
			uint shift = 16u * j;
			gInitSettingRW[0].u16BitPartInU32Pack[j] = 0x0000FFFFu << shift;
			gInitSettingRW[0].u16BitShiftInU32Pack[j] = shift;
		}
	}
	else
	{
		for (uint i = 0u; i < 4u; ++i)
		{
			uint shift = 8u * (3u - i);
			gInitSettingRW[0].u8BitPartInU32Pack[i] = 0x000000FFu << shift;
			gInitSettingRW[0].u8BitShiftInU32Pack[i] = shift;
		}

		for (uint j = 0u; j < 2u; ++j)
		{
			uint shift = 16u * (1u - j);
			gInitSettingRW[0].u16BitPartInU32Pack[j] = 0x0000FFFFu << shift;
			gInitSettingRW[0].u16BitShiftInU32Pack[j] = shift;
		}
	}
	
	
	gInitSettingRW[0].debugVar.x = MatchEndianness(gInitSettingRW[0].debugVar.x);
}
