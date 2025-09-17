#include "hud.h"
#include "../../../external/imgui/imgui.h"
#include "../config/config.h"
#include "../hooks/hooks.h"
#include <ctime>
#include <string>
#include <sstream>
#include <DirectXMath.h>
#include "../../../external/imgui/imgui_internal.h"

Hud::Hud() {

}

float CalculateFovRadius(float fovDegrees, float screenWidth, float screenHeight, float gameVerticalFOV) {
    float aspectRatio = screenWidth / screenHeight;
    float fovRadians = fovDegrees * (DirectX::XM_PI / 180.0f);

    float screenRadius = std::tan(fovRadians / 2.0f) * (screenHeight / 2.0f) / std::tan(gameVerticalFOV * (DirectX::XM_PI / 180.0f) / 2.0f);

    static float flScalingMultiplier = 2.5f;

    return screenRadius * flScalingMultiplier;
}

void RenderFovCircle(ImDrawList* drawList, float fov, ImVec2 screenCenter, float screenWidth, float screenHeight, float thickness) {
    float radius = CalculateFovRadius(fov, screenWidth, screenHeight, H::g_flActiveFov);
    uint32_t color = ImGui::ColorConvertFloat4ToU32(Config::fovCircleColor);
    drawList->AddCircle(screenCenter, radius, color, 100, thickness);
}

void Hud::DrawCustomScope() {
    ImGuiIO& io = ImGui::GetIO();
    if (Config::ScopeRemove) {

        C_CSPlayerPawn* lp = H::oGetLocalPlayer(0);

        if (!lp || lp->getHealth() <= 0)
            return;

        if (Config::ScopeRemove && lp->getInScoped()) {
            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            ImVec2 screenCenter(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

            drawList->AddLine(
                ImVec2(0, screenCenter.y),
                ImVec2(io.DisplaySize.x, screenCenter.y),
                ImColor(0, 0, 0, 255),
                1.f
            );

            drawList->AddLine(
                ImVec2(screenCenter.x, 0),
                ImVec2(screenCenter.x, io.DisplaySize.y),
                ImColor(0, 0, 0, 255),
                1.f
            );
        }
    }
}

static ImFont* g_MediumFont = nullptr;
void Hud::InitializeFonts() {
    ImGuiIO& io = ImGui::GetIO();

    g_MediumFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 24.0f);
    io.Fonts->Build();
}

void Hud::render() {
    // Time
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    char timeBuffer[9];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &localTime);
    
    // FPS
    float fps = ImGui::GetIO().Framerate;
    std::ostringstream fpsStream;
    fpsStream << static_cast<int>(fps) << " FPS";

    // WaterMark
    std::string watermarkText = "Templeware | " + fpsStream.str() + " | " + timeBuffer;

    ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());
    float padding = 5.0f;
    ImVec2 pos = ImVec2(10, 10);
    ImVec2 rectSize = ImVec2(textSize.x + padding * 2, textSize.y + padding * 2);

    ImU32 bgColor = IM_COL32(50, 50, 50, 200);
    ImU32 borderColor = IM_COL32(153, 76, 204, 255);
    ImU32 textColor = IM_COL32(255, 255, 255, 255);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    drawList->AddRectFilled(pos, ImVec2(pos.x + rectSize.x, pos.y + rectSize.y), bgColor);

    float lineThickness = 2.0f;
    drawList->AddLine(pos, ImVec2(pos.x, pos.y + rectSize.y), borderColor, lineThickness);
    drawList->AddLine(ImVec2(pos.x + rectSize.x, pos.y), ImVec2(pos.x + rectSize.x, pos.y + rectSize.y), borderColor, lineThickness);

    ImVec2 textPos = ImVec2(pos.x + padding, pos.y + padding);
    drawList->AddText(textPos, textColor, watermarkText.c_str());

    if (Config::fov_circle) {
        ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);
        RenderFovCircle(drawList, Config::aimbot_fov, Center, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, 1.f);
    }

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImVec2 bindListPos = ImVec2(screenSize.x * 0.01f, screenSize.y * 0.5f);
    float verticalSpacing = 30.0f;

    if (g_MediumFont) {
        ImGui::PushFont(g_MediumFont);
        drawList->AddText(
            bindListPos,
            Config::aimbot ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255),
            "AIMBOT"
        );

        drawList->AddText(
            ImVec2(bindListPos.x, bindListPos.y + verticalSpacing),
            Config::triggerBot ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255),
            "TRIGGER"
        );

        ImGui::PopFont();
    }
}
