#pragma once
#include <cstdint>
#include "../cutlbuffer/cutlbuffer.h"
#include "keyvalues.h"
#include "../../../templeware/utils/memory/vfunc/vfunc.h"
#include "../../../templeware/utils/memory/patternscan/patternscan.h"
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../cutl/utlhash/utlhash.h"
#include "..\..\..\templeware\utils\math\utlvector\utlvector.h"
#include "..\..\..\templeware\interfaces\interfaces.h"



void CKeyValues3::LoadFromBuffer(const char* szString)
{
	CUtlBuffer buffer(0, (std::strlen(szString) + 10), 1);

	buffer.PutString(szString);
	this->LoadKV3(&buffer);
}

bool CKeyValues3::LoadKV3(CUtlBuffer* buffer)
{
	KV3ID_t kv3ID = KV3ID_t("generic", 0x41B818518343427E, 0xB5F447C23C0CDF8C);
	return I::LoadKeyValues(this, nullptr, buffer, &kv3ID, nullptr);
}

CKeyValues3* CKeyValues3::create_material_from_resource()
{
	using fnSetTypeKV3 = CKeyValues3 * (__fastcall*)(CKeyValues3*, unsigned int, unsigned int);
	static const fnSetTypeKV3 oSetTypeKV3 = reinterpret_cast<fnSetTypeKV3>(M::patternScan("tier0", ("40 53 48 83 EC ? 4C 8B 11 41 B9")));

    CKeyValues3* pKeyValue = new CKeyValues3[0x10];

    return oSetTypeKV3(pKeyValue, 1U, 6U);
}
