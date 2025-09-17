#include <algorithm>
#include <iostream>
#include "../../hooks/hooks.h"
#include "../../config/config.h"
#include "../../../../external/imgui/imgui.h"


void reset_walls(C_AggregateSceneObject* object) {
	for (int i = 0; i < object->m_nCount; i++) {
		object->m_pData[i].r = 255;
		object->m_pData[i].g = 255;
		object->m_pData[i].b = 255;
	}
}

void apply_walls(C_AggregateSceneObject* object, ImVec4 colors) {
	for (int i = 0; i < object->m_nCount; i++) {
		object->m_pData[i].r = static_cast<uint8_t>(colors.x * 255.0f);
		object->m_pData[i].g = static_cast<uint8_t>(colors.y * 255.0f);
		object->m_pData[i].b = static_cast<uint8_t>(colors.z * 255.0f);
	}
}
/*
void* __fastcall H::hkUpdateSceneObject(C_AggregateSceneObject* object, void* unk)
{
	static auto update_walls_object = UpdateWallsObject.GetOriginal();
	auto result = update_walls_object(object, unk);
	auto colors = Config::NightColor;

	if (object) {
		if (Config::Night) {
			apply_walls(object, colors);
		}
		else {
			reset_walls(object);
		}
	}

	return result;
}
*/
