#include "CCSPlayerController.h"
#include "../../../templeware/interfaces/interfaces.h"

CCSPlayerController::CCSPlayerController(uintptr_t address) : address(address) {}

uintptr_t CCSPlayerController::getAddress() const {
	return address;
}

const char* CCSPlayerController::getName() const {
	if (!address) return nullptr;
	return reinterpret_cast<const char*>((uintptr_t)this + SchemaFinder::Get(HASH("CBasePlayerController->m_iszPlayerName")));
}
