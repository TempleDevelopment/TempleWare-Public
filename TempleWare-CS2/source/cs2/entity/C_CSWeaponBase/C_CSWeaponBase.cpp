#include "C_CSWeaponBase.h"
#include "..\..\..\templeware\hooks\hooks.h"
CCSWeaponBaseVData* C_CSWeaponBase::Data()
{
	// return pointer to weapon data
	return *reinterpret_cast<CCSWeaponBaseVData**>((uintptr_t)this + H::oGetWeaponData); // 0x360
}
