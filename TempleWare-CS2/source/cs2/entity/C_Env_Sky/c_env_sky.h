#pragma once

#include "../C_AggregateSceneObject/C_AggregateSceneObject.h"
#include "../C_BaseEntity/C_BaseEntity.h"
#include "../../../templeware/utils/C_Color/Color.h"

class c_env_sky : public C_BaseEntity {
public:
	schema(Color_t, m_tint_color, "C_EnvSky->m_vTintColor");
	schema(float, m_brightness_scale, "C_EnvSky->m_flBrightnessScale");
	schema(int, m_fog_type, "C_EnvSky->m_nFogType");
};