#include <algorithm>
#include <iostream>
#include "chams.h"
#include "../../hooks/hooks.h"
#include "../../config/config.h"
#include "../../../../external/imgui/imgui.h"
#include "../../utils/math/utlstronghandle/utlstronghandle.h"
#include "../../../cs2/entity/C_Material/C_Material.h"
#include "../../interfaces/interfaces.h"
#include "../../interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../cs2/datatypes/keyvalues/keyvalues.h"
#include "../../../cs2/datatypes/cutlbuffer/cutlbuffer.h"
#include <algorithm>

ImVec4 HSVtoRGB(float h, float s, float v, float a = 1.0f) {
    h = std::fmod(h, 1.0f);
    int i = int(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i % 6) {
    case 0: return ImVec4(v, t, p, a);
    case 1: return ImVec4(q, v, p, a);
    case 2: return ImVec4(p, v, t, a);
    case 3: return ImVec4(p, q, v, a);
    case 4: return ImVec4(t, p, v, a);
    case 5: return ImVec4(v, p, q, a);
    }
    return ImVec4(0, 0, 0, a);
}

static ImVec4 GetRainbowColorHSV(float speed = 1.0f, float alpha = 1.0f)
{
    static float time = 0.0f;
    time += 0.001f * speed;
    return HSVtoRGB(time, 1.0f, 1.0f, alpha);
}

CStrongHandle<CMaterial2> chams::create(const char* name, const char szVmatBuffer[])
{
    CKeyValues3* pKeyValues3 = nullptr;

    pKeyValues3 = pKeyValues3->create_material_from_resource();

    pKeyValues3->LoadFromBuffer(szVmatBuffer);

    CStrongHandle<CMaterial2> pCustomMaterial = {};

    I::CreateMaterial(nullptr, &pCustomMaterial, name, pKeyValues3, 0, 1);

    return pCustomMaterial;
}

struct resource_material_t
{
    CStrongHandle<CMaterial2> mat;
    CStrongHandle<CMaterial2> mat_invs;
};

static resource_material_t resourceMaterials[ChamsType::MAXCOUNT];
bool loaded_materials = false;
bool chams::Materials::init()
{
    // flat
    resourceMaterials[FLAT] = resource_material_t{
        .mat = create("materials/dev/flat.vmat", R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
        {
            Shader = "csgo_unlitgeneric.vfx"
        
            F_IGNOREZ = 0
            F_DISABLE_Z_WRITE = 0
            F_DISABLE_Z_BUFFERING = 0
            F_BLEND_MODE = 1
            F_TRANSLUCENT = 1
            F_RENDER_BACKFACES = 0

            g_vColorTint = [1.000000, 1.000000, 1.000000, 1.000000]
            g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
            g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
            g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
            g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
            g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
            g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
        })"),
        .mat_invs = create("materials/dev/flat_i.vmat", R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
        {
            Shader = "csgo_unlitgeneric.vfx"
            F_IGNOREZ = 1
            F_DISABLE_Z_WRITE = 1
            F_DISABLE_Z_BUFFERING = 1
            F_BLEND_MODE = 1
            F_TRANSLUCENT = 1
            g_vColorTint = [1.000000, 1.000000, 1.000000, 0.000000]
            g_vGlossinessRange = [0.000000, 1.000000, 0.000000, 0.000000]
            g_vNormalTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
            g_vTexCoordOffset = [0.000000, 0.000000, 0.000000, 0.000000]
            g_vTexCoordScale = [1.000000, 1.000000, 0.000000, 0.000000]
            g_tColor = resource:"materials/dev/primary_white_color_tga_f7b257f6.vtex"
            g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
        })")
    };
    resourceMaterials[ILLUMINATE] = resource_material_t{
    .mat = create("materials/dev/primary_white.vmat",  R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_complex.vfx"

	F_SELF_ILLUM = 1
	F_PAINT_VERTEX_COLORS = 1
	F_TRANSLUCENT = 1

	g_vColorTint = [ 1.000000, 1.000000, 1.000000, 1.000000 ]
	g_flSelfIllumScale = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
	g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 10.000000, 10.000000, 10.000000, 10.000000 ]

	g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
	g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
})"),
        .mat_invs = create("materials/dev/primary_white.vmat", R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
	shader = "csgo_complex.vfx"

	F_SELF_ILLUM = 1
	F_PAINT_VERTEX_COLORS = 1
	F_TRANSLUCENT = 1
    F_DISABLE_Z_BUFFERING = 1

	g_vColorTint = [ 1.000000, 1.000000, 1.000000, 1.000000 ]
	g_flSelfIllumScale = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
	g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 10.000000, 10.000000, 10.000000, 10.000000 ]

	g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
	TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
	g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
})")
    };

    resourceMaterials[GLOW] = resource_material_t{
    .mat = create("materials/dev/primary_white.vmat",  R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
				shader = "csgo_effects.vfx"
                g_flFresnelExponent = 7.0
                g_flFresnelFalloff = 10.0
                g_flFresnelMax = 0.1
                g_flFresnelMin = 1.0
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_flToolsVisCubemapReflectionRoughness = 1.0
                g_flBeginMixingRoughness = 1.0
                g_vColorTint = [ 1.000000, 1.000000, 1.000000, 0 ]
                F_IGNOREZ = 0
	            F_TRANSLUCENT = 1

                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 1
                F_RENDER_BACKFACES = 0
})"),

        .mat_invs = create("materials/dev/primary_white.vmat", R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
				shader = "csgo_effects.vfx"
                g_flFresnelExponent = 7.0
                g_flFresnelFalloff = 10.0
                g_flFresnelMax = 0.1
                g_flFresnelMin = 1.0
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_flToolsVisCubemapReflectionRoughness = 1.0
                g_flBeginMixingRoughness = 1.0
                g_vColorTint = [ 1.000000, 1.000000, 1.000000, 0 ]
                F_IGNOREZ = 1
	            F_TRANSLUCENT = 1

                F_DISABLE_Z_WRITE = 1
                F_DISABLE_Z_BUFFERING = 1
                F_RENDER_BACKFACES = 0
})")
    };

    return true;
}

ChamsEntity chams::GetTargetType(C_BaseEntity* render_ent) noexcept {
    auto local = H::oGetLocalPlayer(0);
    if (!local)
        return ChamsEntity::INVALID;

    if (render_ent->IsViewmodelAttachment())
        return ChamsEntity::HANDS;

    if (render_ent->IsViewmodel())
        return ChamsEntity::VIEWMODEL;

    if (!render_ent->IsBasePlayer() && !render_ent->IsPlayerController())
        return ChamsEntity::INVALID;

    auto player = (C_CSPlayerPawn*)render_ent;
    if (!player)
        return ChamsEntity::INVALID;

    auto alive = player->m_iHealth() > 0;
    if (!alive)
        return ChamsEntity::INVALID;

    if (player->m_iTeamNum() == local->m_iTeamNum())
        return ChamsEntity::INVALID;

    return ChamsEntity::ENEMY;
}

CMaterial2* GetMaterial(int type, bool invisible) { return invisible ? resourceMaterials[type].mat_invs : resourceMaterials[type].mat; }

void __fastcall chams::hook(void* a1, void* a2, CMeshData* pMeshScene, int nMeshCount, void* pSceneView, void* pSceneLayer, void* pUnk)
{
    static auto original = H::DrawArray.GetOriginal();

    if (!I::EngineClient->valid())
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    auto local_player = H::oGetLocalPlayer(0);
    if (!local_player)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    if (!pMeshScene)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    if (!pMeshScene->pSceneAnimatableObject)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    if (nMeshCount < 1)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    CMeshData* render_data = pMeshScene;
    if (!render_data)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    if (!render_data->pSceneAnimatableObject)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    auto render_ent = render_data->pSceneAnimatableObject->Owner();
    if (!render_ent.valid())
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    auto entity = I::GameEntity->Instance->Get(render_ent);
    if (!entity)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    const auto target = GetTargetType(entity);

    if (target == ChamsEntity::VIEWMODEL && Config::viewmodelChams) {
        pMeshScene->pMaterial = GetMaterial(Config::chamsMaterial, false);

        if (Config::rainbow) {
            ImVec4 rainbowColor = GetRainbowColorHSV(1.0f, Config::colViewmodelChams.w);
            pMeshScene->color.r = static_cast<uint8_t>(rainbowColor.x * 255.0f);
            pMeshScene->color.g = static_cast<uint8_t>(rainbowColor.y * 255.0f);
            pMeshScene->color.b = static_cast<uint8_t>(rainbowColor.z * 255.0f);
        }
        else {
            pMeshScene->color.r = static_cast<uint8_t>(Config::colViewmodelChams.x * 255.0f);
            pMeshScene->color.g = static_cast<uint8_t>(Config::colViewmodelChams.y * 255.0f);
            pMeshScene->color.b = static_cast<uint8_t>(Config::colViewmodelChams.z * 255.0f);
        }

        pMeshScene->color.a = static_cast<uint8_t>(Config::colViewmodelChams.w * 255.0f);
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    }

    if (target == ChamsEntity::HANDS && Config::armChams) {
        pMeshScene->pMaterial = GetMaterial(Config::chamsMaterial, false);
        pMeshScene->color.r = static_cast<uint8_t>(Config::colArmChams.x * 255.0f);
        pMeshScene->color.g = static_cast<uint8_t>(Config::colArmChams.y * 255.0f);
        pMeshScene->color.b = static_cast<uint8_t>(Config::colArmChams.z * 255.0f);
        pMeshScene->color.a = static_cast<uint8_t>(Config::colArmChams.w * 255.0f);
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    }

    if (target != ENEMY)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    bool og = !Config::enemyChams && !Config::enemyChamsInvisible;
    if (og)
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);

    if (Config::enemyChamsInvisible) {
        pMeshScene->pMaterial = GetMaterial(Config::chamsMaterial, true);
        pMeshScene->color.r = static_cast<uint8_t>(Config::colVisualChamsIgnoreZ.x * 255.0f);
        pMeshScene->color.g = static_cast<uint8_t>(Config::colVisualChamsIgnoreZ.y * 255.0f);
        pMeshScene->color.b = static_cast<uint8_t>(Config::colVisualChamsIgnoreZ.z * 255.0f);
        pMeshScene->color.a = static_cast<uint8_t>(Config::colVisualChamsIgnoreZ.w * 255.0f);

        original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    }

    if (Config::enemyChams) {
        pMeshScene->pMaterial = GetMaterial(Config::chamsMaterial, false);
        pMeshScene->color.r = static_cast<uint8_t>(Config::colVisualChams.x * 255.0f);
        pMeshScene->color.g = static_cast<uint8_t>(Config::colVisualChams.y * 255.0f);
        pMeshScene->color.b = static_cast<uint8_t>(Config::colVisualChams.z * 255.0f);
        pMeshScene->color.a = static_cast<uint8_t>(Config::colVisualChams.w * 255.0f);
        return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
    }

    // If we get here, neither chams type is enabled, so just render normally
    return original(a1, a2, pMeshScene, nMeshCount, pSceneView, pSceneLayer, pUnk);
}
