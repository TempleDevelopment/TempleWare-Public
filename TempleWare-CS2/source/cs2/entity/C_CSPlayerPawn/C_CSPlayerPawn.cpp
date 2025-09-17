#include "C_CSPlayerPawn.h"

// #include "../../../templeware/offsets/offsets.h"
#include "../../../templeware/interfaces/interfaces.h"


C_CSPlayerPawn::C_CSPlayerPawn(uintptr_t address) : address(address) {}

Vector_t C_CSPlayerPawn::getPosition() const {
	return *(Vector_t*)(address + SchemaFinder::Get(HASH("C_BasePlayerPawn->m_vOldOrigin")));
}

Vector_t C_CSPlayerPawn::getEyePosition() const {
	return Vector_t();
}

C_CSWeaponBase* C_CSPlayerPawn::GetActiveWeapon() const {
	if (!this)
		return nullptr;

	CCSPlayer_WeaponServices* weapon_services = this->GetWeaponServices();
	if (weapon_services == nullptr)
		return nullptr;

	C_CSWeaponBase* active_weapon = I::GameEntity->Instance->Get<C_CSWeaponBase>(weapon_services->m_hActiveWeapon());
	if (!active_weapon)
		return nullptr;

	return active_weapon;
}

CCSPlayer_WeaponServices* C_CSPlayerPawn::GetWeaponServices() const {
	if (!address) return nullptr;
	return reinterpret_cast<CCSPlayer_WeaponServices*>((uintptr_t)this + SchemaFinder::Get(HASH("C_CSPlayerPawn->m_pWeaponServices")));
}

uintptr_t C_CSPlayerPawn::getAddress() const {
	return address;
}

int C_CSPlayerPawn::getMoveType() const {
	return *reinterpret_cast<int*>((uintptr_t)this + SchemaFinder::Get(HASH("C_BaseEntity->m_nActualMoveType")));
}

uint32_t C_CSPlayerPawn::getFlags() const {
	return *reinterpret_cast<uint32_t*>((uintptr_t)this + SchemaFinder::Get(HASH("C_BaseEntity->m_fFlags")));
}

bool C_CSPlayerPawn::getInScoped() const {
	return *reinterpret_cast<bool*>((uintptr_t)this + SchemaFinder::Get(HASH("C_CSPlayerPawn->m_bIsScoped")));
}

int C_CSPlayerPawn::getIDEntIndex() const {
	return *reinterpret_cast<int*>((uintptr_t)this + SchemaFinder::Get(HASH("C_CSPlayerPawnBase->m_iIDEntIndex")));
}

int32_t C_CSPlayerPawn::getHealth() const {
	return *reinterpret_cast<uint32_t*>((uintptr_t)this + SchemaFinder::Get(HASH("C_BaseEntity->m_iHealth")));
}

uint8_t C_CSPlayerPawn::getTeam() const {
	return *reinterpret_cast<uint8_t*>((uintptr_t)this + SchemaFinder::Get(HASH("C_BaseEntity->m_iTeamNum")));
}

Vector_t C_CSPlayerPawn::getViewOffset() const {
	return *reinterpret_cast<Vector_t*>((uintptr_t)this + SchemaFinder::Get(HASH("C_BaseModelEntity->m_vecViewOffset")));
}
