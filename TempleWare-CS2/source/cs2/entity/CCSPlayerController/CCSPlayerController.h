#pragma once
#include <cstdint>
#pragma once
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../C_CSWeaponBase/C_CSWeaponBase.h"
#include <cstdint>
class CCSPlayerController {
public:
	CCSPlayerController(uintptr_t address);
	const char* getName() const;
	uintptr_t getAddress() const;

	SCHEMA_ADD_OFFSET(bool, IsLocalPlayer, 0x778);
	SCHEMA_ADD_OFFSET(CBaseHandle, m_hPawn, 0x8FC);

private:
	uintptr_t address;
};
