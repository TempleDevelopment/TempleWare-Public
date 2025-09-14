#pragma once
#include "includeHooks.h"
#include "../../cs2/entity/C_AggregateSceneObject/C_AggregateSceneObject.h"
#include "../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../cs2/datatypes/cutlbuffer/cutlbuffer.h"
#include "../../cs2/datatypes/keyvalues/keyvalues.h"
#include "../../cs2/entity/C_Material/C_Material.h"
#include "../../cs2/entity/C_Env_Sky/c_env_sky.h"

// Forward declaration
class CMeshData;
class CEntityIdentity;

namespace H {
	// void* __fastcall hkUpdateSceneObject(C_AggregateSceneObject* object, void* unk);
	void __fastcall hkFrameStageNotify(void* a1, int stage);
	void* __fastcall hkLevelInit(void* pClientModeShared, const char* szNewMap);
	void __fastcall hkChamsObject(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
	void __fastcall hkRenderFlashbangOverlay(void* a1, void* a2, void* a3, void* a4, void* a5);
	void* __fastcall hkDrawSmokeArray(void* rcx, void* pSomePointer1, void* pSomePointer2, void* pSomePointer3, void* pSomePointer4, void* pSomePointer5);
	void __fastcall hkDrawScopeOverlay(void* a1, void* a2);
	void* __fastcall hkDrawLegs(void* a1, void* a2, void* a3, void* a4, void* a5);
	bool __fastcall hkMouseInputEnabled(void* rcx);
	void* __fastcall hkIsRelativeMouseMode(void* pThisptr, bool bActive);
	void* __fastcall hkDrawViewModel(float* a1, float* offsets, float* fov);
	inline float g_flActiveFov;
	float hkGetRenderFov(void* rcx);
	void __fastcall hkLightingModulate(void* pLightBinnerGPU, CSceneLightObject* pSceneLightObject, void* a3);
	void* __fastcall hkUpdateSkybox(c_env_sky* sky);


	inline CInlineHookObj<decltype(&hkChamsObject)> DrawArray = { };
	inline CInlineHookObj<decltype(&hkFrameStageNotify)> FrameStageNotify = { };
	// inline CInlineHookObj<decltype(&hkUpdateSceneObject)> UpdateWallsObject = { };
	inline CInlineHookObj<decltype(&hkGetRenderFov)> GetRenderFov = { };
	inline CInlineHookObj<decltype(&hkLevelInit)> LevelInit = { };
	inline CInlineHookObj<decltype(&hkRenderFlashbangOverlay)> RenderFlashBangOverlay = { };
	inline CInlineHookObj<decltype(&hkDrawSmokeArray)> DrawSmokeArray = { };
	inline CInlineHookObj<decltype(&hkDrawScopeOverlay)> DrawScopeOverlay = { };
	inline CInlineHookObj<decltype(&hkDrawLegs)> DrawLegs = { };
	inline CInlineHookObj<decltype(&hkDrawViewModel)> DrawViewModel = { };
	inline CInlineHookObj<decltype(&hkLightingModulate)> LightingModulate = { };
	inline CInlineHookObj<decltype(&hkMouseInputEnabled)> MouseInputEnabled = { };
	inline CInlineHookObj<decltype(&hkIsRelativeMouseMode)> IsRelativeMouseMode = {};
	inline CInlineHookObj<decltype(&hkUpdateSkybox)> UpdateSkybox = {};


	// inline hooks
	inline int  oGetWeaponData;
	inline void* (__fastcall* ogGetBaseEntity)(void*, int);
	inline C_CSPlayerPawn* (__fastcall* oGetLocalPlayer)(int);

	class Hooks {
	public:
		void init();
	};
}
