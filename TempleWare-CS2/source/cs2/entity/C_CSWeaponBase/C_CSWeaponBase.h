#pragma once
#include <cstdint>
#include "..\C_EntityInstance\C_EntityInstance.h"
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "..\..\..\..\source\templeware\utils\schema\schema.h"
#include "..\..\..\..\source\templeware\utils\memory\vfunc\vfunc.h"
#include "..\handle.h"

class CCSPlayer_WeaponServices
{
public:
	schema(bool, m_bAllowSwitchToNoWeapon, "CPlayer_WeaponServices->m_bAllowSwitchToNoWeapon");
	schema(CBaseHandle, m_hMyWeapons, "CPlayer_WeaponServices->m_hMyWeapons");
	schema(CBaseHandle, m_hActiveWeapon, "CPlayer_WeaponServices->m_hActiveWeapon");
	schema(CBaseHandle, m_hLastWeapon, "CPlayer_WeaponServices->m_hLastWeapon");
	schema(int, m_iAmmo, "CPlayer_WeaponServices->m_iAmmo");
	schema(float, m_flNextAttack, "CCSPlayer_WeaponServices->m_flNextAttack");
	schema(bool, m_bIsLookingAtWeapon, "CCSPlayer_WeaponServices->m_bIsLookingAtWeapon");
	schema(bool, m_bIsHoldingLookAtWeapon, "CCSPlayer_WeaponServices->m_bIsHoldingLookAtWeapon");
};

class CCSWeaponBaseVData
{
public:
	SCHEMA_ADD_OFFSET(const char*, m_szName, 0xD20);
	SCHEMA_ADD_OFFSET(int32_t, GetWeaponType, 0x348);
};

class C_CSWeaponBase 
{
public:
// @todo: add few schemas here
	CCSWeaponBaseVData* Data();
};
