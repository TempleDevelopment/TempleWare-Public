#pragma once
#include <cstdint>
#include "../C_EntityInstance/C_EntityInstance.h"
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "../../../../source/templeware/utils/schema/schema.h"
#include "../../../../source/templeware/utils/memory/vfunc/vfunc.h"
#include "../handle.h"

class C_BaseEntity : public CEntityInstance
{
public:
	schema(int, m_iMaxHealth, "C_BaseEntity->m_iMaxHealth");
	SCHEMA_ADD_OFFSET(int, m_iHealth, 0x34C);
	SCHEMA_ADD_OFFSET(int, m_iTeamNum, 0x3EB);

	bool PostDataUpdate(int iUpdateType)
	{
		return M::vfunc<bool, 7>(this, iUpdateType);
	}

	bool IsBasePlayer()
	{
		SchemaClassInfoData_t* pClassInfo;
		dump_class_info(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;

		return HASH(pClassInfo->szName) == HASH("C_CSPlayerPawn");
	}

	bool IsViewmodelAttachment()
	{
		SchemaClassInfoData_t* pClassInfo;
		dump_class_info(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;

		return HASH(pClassInfo->szName) == HASH("C_CS2HudModelArms");
	}

	bool IsViewmodel()
	{
		SchemaClassInfoData_t* pClassInfo;
		dump_class_info(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;

		return HASH(pClassInfo->szName) == HASH("C_CS2HudModelWeapon");
	}

	bool IsPlayerController()
	{
		SchemaClassInfoData_t* _class = nullptr;
		dump_class_info(&_class);
		if (!_class)
			return false;

		const uint32_t hash = HASH(_class->szName);

		return (hash == HASH("CCSPlayerController"));
	}
};
