#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../templeware/hooks/hooks.h"
#include "../../../templeware/config/config.h"

#include <chrono>
#include <Windows.h>
#include <limits>
#include <pplwin.h>
#include <random>

Vector_t GetEntityEyePos(const C_CSPlayerPawn* Entity) {
    if (!Entity)
        return {};

    uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)Entity +
        SchemaFinder::Get(HASH("C_BaseEntity->m_pGameSceneNode")));

    auto Origin = *reinterpret_cast<Vector_t*>(game_scene_node +
        SchemaFinder::Get(HASH("CGameSceneNode->m_vecAbsOrigin")));
    auto ViewOffset = *reinterpret_cast<Vector_t*>((uintptr_t)Entity +
        SchemaFinder::Get(HASH("C_BaseModelEntity->m_vecViewOffset")));

    uintptr_t model = *reinterpret_cast<uintptr_t*>(game_scene_node +
        SchemaFinder::Get(HASH("CSkeletonInstance->m_modelState")) + 0x80);

    auto head = *reinterpret_cast<Vector_t*>(model + static_cast<unsigned long long>(6) * 32);

    Vector_t result;
    result = head;
    if (Config::offsetaim)
        result = Origin + ViewOffset;

    if (!std::isfinite(result.x) || !std::isfinite(result.y) || !std::isfinite(result.z))
        return {};

    return result;
}

inline QAngle_t CalcAngles(Vector_t viewPos, Vector_t aimPos) {
    QAngle_t angle = { 0, 0, 0 };
    Vector_t delta = aimPos - viewPos;

    angle.x = -asin(delta.z / delta.Length()) * (180.0f / 3.141592654f);
    angle.y = atan2(delta.y, delta.x) * (180.0f / 3.141592654f);

    return angle;
}

inline float GetFov(const QAngle_t& viewAngle, const QAngle_t& aimAngle) {
    QAngle_t delta = (aimAngle - viewAngle).Normalize();
    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}


void Triggerbot() {
    if (!Config::triggerBot) return;

    C_CSPlayerPawn* local = H::oGetLocalPlayer(0);
    if (!local || local->getHealth() <= 0) return;

    static uintptr_t dwForceAttack = modules.getModule("client") + 0x18518E0;
    static bool needRelease = false;

    const int targetIdx = local->getIDEntIndex();
    bool shouldAttack = false;

    if (targetIdx != -1) {

        if (auto target = I::GameEntity->Instance->Get(targetIdx)) {

            if (target->handle().valid()) {

                SchemaClassInfoData_t* ci = nullptr;
                target->dump_class_info(&ci);

                if (ci && HASH(ci->szName) == HASH("C_CSPlayerPawn")) {

                    auto pawn = reinterpret_cast<C_CSPlayerPawn*>(target);

                    if (pawn->getTeam() != local->getTeam() && pawn->getHealth() > 0) {
                        shouldAttack = true;
                    }
                }
            }
        }
    }
    if (shouldAttack) {
        *reinterpret_cast<int*>(dwForceAttack) = 65537; // +attack
        needRelease = true;
    }

    else if (needRelease) {
        *reinterpret_cast<int*>(dwForceAttack) = 256; // -attack
        needRelease = false;
    }
}

void FakeSpamm() {
    if (!Config::spammer)
        return;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    switch (Config::spammingcho) {
        case 0: // SharkHack
        {
            const char* messages[] = {
                "Download SharkHack cheat right now!",
                "You just got owned by SharkHack cheat",
                "I'm dominating with SharkHack cheat xD",
                "Get SharkHack and wreck everyone!"
            };
            std::uniform_int_distribution<>dist(0, IM_ARRAYSIZE(messages) - 1);
            I::EngineClient->send_cmd(std::format("say {}", messages[dist(gen)]).c_str());
            break;
        }
        case 1: // AimWare
        {
            const char* messages[] = {
                "www.AIMWARE.net | Premium CS2 Cheat",
                "Get rekt by $me using Aimware.net",
                "Aimware.net is the superior solution.",
                "Aimware.net, your next premium cheat."
            };
            std::uniform_int_distribution<>dist(0, IM_ARRAYSIZE(messages) - 1);
            I::EngineClient->send_cmd(std::format("say {}", messages[dist(gen)]).c_str());
            break;
        }
        case 2: // Luno
        {
            const char* messages[] = {
                "Luno - new era in cs2",
                "Get Luno right now",
                "You seriously paying for cheats every month XD!?!?",
                "i - snoop, you - dog"
            };
            std::uniform_int_distribution<>dist(0, IM_ARRAYSIZE(messages) - 1);
            I::EngineClient->send_cmd(std::format("say {}", messages[dist(gen)]).c_str());
            break;
        }
    }
}


void Aimbot() {
    if (!Config::aimbot)
        return;

    C_CSPlayerPawn* localPlayer = H::oGetLocalPlayer(0);
    if (!localPlayer || localPlayer->m_iHealth() <= 0)
        return;

    static QAngle_t oldpunch = { 0.f, 0.f, 0.f };

    Vector_t localEyePos = GetEntityEyePos(localPlayer);
    QAngle_t* viewAngles = (QAngle_t*)(modules.getModule("client") + 0x1A78650);

    float bestFov = Config::aimbot_fov;
    C_CSPlayerPawn* bestTarget = nullptr;
    QAngle_t bestAngle = { 0, 0, 0 };

    for (int i = 1; i <= I::GameEntity->Instance->GetHighestEntityIndex(); i++) {
        auto entity = I::GameEntity->Instance->Get(i);
        if (!entity || !entity->handle().valid())
            continue;

        SchemaClassInfoData_t* classInfo = nullptr;
        entity->dump_class_info(&classInfo);
        if (!classInfo || HASH(classInfo->szName) != HASH("C_CSPlayerPawn"))
            continue;

        C_CSPlayerPawn* pawn = reinterpret_cast<C_CSPlayerPawn*>(entity);

        if (pawn == localPlayer ||
            pawn->m_iHealth() <= 0 ||
            pawn->m_iTeamNum() == localPlayer->m_iTeamNum())
            continue;

        Vector_t enemyEyePos = GetEntityEyePos(pawn);
        QAngle_t angleToEnemy = CalcAngles(localEyePos, enemyEyePos);

        angleToEnemy = angleToEnemy.Normalize();

        float currentFov = GetFov(*viewAngles, angleToEnemy);
        if (!std::isfinite(currentFov))
            continue;

        if (currentFov < bestFov) {
            bestFov = currentFov;
            bestTarget = pawn;
            bestAngle = angleToEnemy;
        }

    }

    if (bestTarget) {

        if (Config::rcs) {
            QAngle_t punchAngles = *(QAngle_t*)((uintptr_t)localPlayer + SchemaFinder::Get(hash_32_fnv1a_const("C_CSPlayerPawn->m_aimPunchAngle")));
            bestAngle -= punchAngles * 2.f;
        }

        bestAngle.z = 0;
        *viewAngles = bestAngle.Normalize();
    }
}
