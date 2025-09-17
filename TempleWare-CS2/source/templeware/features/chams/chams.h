#pragma once
#include <cstdint>
#include "../../../cs2/entity/C_BaseEntity/C_BaseEntity.h"
#include "../../../cs2/entity/C_Material/C_Material.h"

#include "../../utils/math/utlstronghandle/utlstronghandle.h"

// forward declarations
class CMeshData;

enum ChamsType {
	FLAT,
	ILLUMINATE,
	GLOW,
	MAXCOUNT
};

enum ChamsEntity : std::int32_t {
	INVALID = 0,
	ENEMY,
	TEAM,
	VIEWMODEL,
	HANDS
};

enum MaterialType {
	e_visible,
	e_invisible,
	e_max_material
};

namespace chams
{
	class Materials {
	public:
		bool init();
	};

	static ChamsEntity GetTargetType(C_BaseEntity* entity) noexcept;
	CStrongHandle<CMaterial2> create(const char* name, const char szVmatBuffer[]);
	void __fastcall hook(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk);
}
