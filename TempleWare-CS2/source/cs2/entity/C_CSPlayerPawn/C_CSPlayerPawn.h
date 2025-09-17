#pragma once
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "../../../templeware/utils/schema/schema.h"
#include "../C_CSWeaponBase/C_CSWeaponBase.h"
#include "../C_BaseEntity/C_BaseEntity.h"

#include <cstdint>

class C_CSPlayerPawn : public C_BaseEntity {
public:
	SCHEMA_ADD_OFFSET(Vector_t, m_vOldOrigin, 0x15B0);
	SCHEMA_ADD_OFFSET(Vector_t, m_vecViewOffset, 0xD98);
	SCHEMA_ADD_OFFSET(CCSPlayer_WeaponServices*, m_pWeaponServices, 0x1400);
	C_CSPlayerPawn(uintptr_t address);

	C_CSWeaponBase* GetActiveWeapon() const;
	CCSPlayer_WeaponServices* GetWeaponServices() const;
	Vector_t getPosition() const;
	Vector_t getEyePosition() const;
	Vector_t getViewOffset() const;

	int getIDEntIndex() const;
	int getMoveType() const;
	uintptr_t getAddress() const;
	int32_t getHealth() const;
	uint32_t getFlags() const;
	uint8_t getTeam() const;
	bool getInScoped() const;
private:
	uintptr_t address;
};
