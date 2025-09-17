#include "../../../hooks/hooks.h"
#include "../../../config/config.h"
#include "../../../interfaces/interfaces.h"
/*
void resetlight(CSceneLightObject* pSceneLightObject) {
	pSceneLightObject->r = 255;
	pSceneLightObject->g = 255;
	pSceneLightObject->b = 255;
}

void Lighting(CSceneLightObject* pSceneLightObject) {
	pSceneLightObject->r = static_cast<uint8_t>(Config::DrawLight.x * 255.f);
	pSceneLightObject->g = static_cast<uint8_t>(Config::DrawLight.y * 255.f);
	pSceneLightObject->b = static_cast<uint8_t>(Config::DrawLight.z * 255.f);
}
*/



void __fastcall H::hkLightingModulate(void* pLightBinnerGPU, CSceneLightObject* pSceneLightObject, void* a3) {
	static auto res = LightingModulate.GetOriginal();

	if (Config::light) {
		pSceneLightObject->r = static_cast<uint8_t>(Config::DrawLight.x * 5.f);
		pSceneLightObject->g = static_cast<uint8_t>(Config::DrawLight.y * 5.f);
		pSceneLightObject->b = static_cast<uint8_t>(Config::DrawLight.z * 5.f);
	}
	else {
		pSceneLightObject->r = static_cast<uint8_t>(Config::DrawLight.x / 5.f);
		pSceneLightObject->g = static_cast<uint8_t>(Config::DrawLight.y / 5.f);
		pSceneLightObject->b = static_cast<uint8_t>(Config::DrawLight.z / 5.f);
	}

	return res(pLightBinnerGPU, pSceneLightObject, a3);
}