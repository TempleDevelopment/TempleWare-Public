#pragma once
#include "IEngineClient/IEngineClient.h"
#include "CGameEntitySystem/CGameEntitySystem.h"
#include "..\..\cs2\entity\C_CSPlayerPawn\C_CSPlayerPawn.h"
#include "..\..\cs2\datatypes\cutlbuffer\cutlbuffer.h"
#include "..\..\cs2\datatypes\keyvalues\keyvalues.h"
#include "..\..\cs2\entity\C_Material\C_Material.h"
#include "../interfaces/Source2/inputsystem.h"


namespace I
{
	inline void(__fastcall* EnsureCapacityBuffer)(CUtlBuffer*, int) = nullptr;
	inline CUtlBuffer* (__fastcall* ConstructUtlBuffer)(CUtlBuffer*, int, int, int) = nullptr;
	inline void(__fastcall* PutUtlString)(CUtlBuffer*, const char*);
	inline std::int64_t(__fastcall* CreateMaterial)(void*, void*, const char*, void*, unsigned int, unsigned int);
	inline bool(__fastcall* LoadKeyValues)(CKeyValues3*, void*, CUtlBuffer*, const KV3ID_t*, const char*);

	// Logging functions
	inline void(__fastcall* ConMsg)(const char*, ...);
	inline void(__fastcall* ConColorMsg)(const Color&, const char*, ...);

	inline IEngineClient* EngineClient = nullptr;
	inline IGameResourceService* GameEntity = nullptr;
	inline IInputSystem* InputSys = nullptr;
	class Interfaces {
	public:
		bool init();
	};
}
