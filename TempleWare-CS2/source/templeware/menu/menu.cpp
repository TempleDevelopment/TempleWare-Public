#include "menu.h"
#include "../config/config.h"

#include <iostream>
#include <vector>
#include "../config/configmanager.h"

#include "../keybinds/keybinds.h"

#include "../utils/logging/log.h"

#include "../interfaces/interfaces.h"

#include "../hooks/hooks.h"

void ApplyImGuiTheme() {
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImVec4 primaryColor = ImVec4(0.44f, 0.23f, 0.78f, 1.0f);
    ImVec4 outlineColor = ImVec4(0.54f, 0.33f, 0.88f, 0.7f);

    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);

    colors[ImGuiCol_Button] = primaryColor;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.54f, 0.33f, 0.88f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.13f, 0.68f, 1.0f);

    colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.50f, 1.00f, 1.0f);
    colors[ImGuiCol_SliderGrab] = primaryColor;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.33f, 0.88f, 1.0f);

    colors[ImGuiCol_Header] = primaryColor;
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.54f, 0.33f, 0.88f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.34f, 0.13f, 0.68f, 1.0f);

    colors[ImGuiCol_Separator] = ImVec4(0.34f, 0.13f, 0.68f, 1.0f);
    colors[ImGuiCol_SeparatorHovered] = primaryColor;
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.54f, 0.33f, 0.88f, 1.0f);

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.17f, 0.21f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.44f, 0.23f, 0.78f, 0.8f);
    colors[ImGuiCol_TabActive] = primaryColor;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.17f, 0.17f, 0.21f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.34f, 0.13f, 0.68f, 1.0f);

    colors[ImGuiCol_Border] = outlineColor;
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.PopupRounding = 0.0f;

    style.ItemSpacing = ImVec2(8, 4);
    style.FramePadding = ImVec2(4, 3);
    style.WindowPadding = ImVec2(8, 8);

    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;

    style.GrabMinSize = 7.0f;
}

Menu* Menu::instance = nullptr;

Menu::Menu() {
    instance = this;
    activeTab = 0;
    showMenu = true;
    stack = false;
}


void Menu::init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyImGuiTheme();

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    std::cout << "initialized menu\n";
}

void Menu::render() {
    keybind.pollInputs();
    if (showMenu) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);

        ImGui::Begin("Internal", nullptr, window_flags);

        {
            float windowWidth = ImGui::GetWindowWidth();
            float rightTextWidth = ImGui::CalcTextSize("Templeware").x;

            ImGui::Text("Internal");

            ImGui::SameLine(windowWidth - rightTextWidth - 10);
            ImGui::Text("Templeware");
        }

        ImGui::Separator();

        const char* tabNames[] = { "Aim", "Visuals", "Misc", "Config" };

        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_NoTooltip)) {
            for (int i = 0; i < 4; i++) {
                if (ImGui::BeginTabItem(tabNames[i])) {
                    activeTab = i;
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        ImGui::BeginChild("ContentRegion", ImVec2(0, 0), false);

        switch (activeTab) {
        case 0:
        {
            ImGui::BeginChild("AimLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("General");
            ImGui::Separator();

            ImGui::Checkbox("Enable##navodka", &Config::aimbot);
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::aimbot);

            if (Config::aimbot) {
                ImGui::Checkbox("offset aim", &Config::offsetaim);
            }

            ImGui::SliderFloat("FOV", &Config::aimbot_fov, 0.f, 90.f);
            ImGui::Checkbox("Draw FOV Circle", &Config::fov_circle);
            if (Config::fov_circle) {
                ImGui::ColorEdit4("Circle Color##FovColor", (float*)&Config::fovCircleColor);
            }
            ImGui::Checkbox("Recoil Control", &Config::rcs);


            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("AimRight", ImVec2(0, 0), true);
            ImGui::Text("TriggerBot");
            ImGui::Separator();
            // ImGui::Text("No additional settings");

            ImGui::Checkbox("Trigger", &Config::triggerBot);
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::triggerBot);

            ImGui::EndChild();
        }
        break;

        case 1:
        {
            ImGui::BeginChild("VisualsLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("Player ESP");
            ImGui::Separator();

            ImGui::Checkbox("Box", &Config::esp);
            ImGui::SliderFloat("Thickness", &Config::espThickness, 1.0f, 5.0f);
            ImGui::Checkbox("Box Fill", &Config::espFill);
            if (Config::espFill) {
                ImGui::SliderFloat("Fill Opacity", &Config::espFillOpacity, 0.0f, 1.0f);
            }
            ImGui::ColorEdit4("ESP Color##BoxColor", (float*)&Config::espColor);
            ImGui::Checkbox("Team Check", &Config::teamCheck);
            ImGui::Checkbox("Health Bar", &Config::showHealth);
            ImGui::Checkbox("Name Tags", &Config::showNameTags);

            ImGui::Spacing();
            ImGui::Text("World");
            ImGui::Separator();

            ImGui::Checkbox("SkyBox", &Config::skybox);
            if (Config::skybox) {
                ImGui::ColorEdit4("SkyBox Color", (float*)&Config::skyboxcolor);
            }
            
            ImGui::Checkbox("Fog rendering", &Config::fog);

            ImGui::Checkbox("Night Mode", &Config::Night);
            if (Config::Night) {
                ImGui::ColorEdit4("Night Color", (float*)&Config::NightColor);
            }

            ImGui::Checkbox("Lighting", &Config::light);
            if (Config::light) {
                ImGui::ColorEdit4("Lighting Color", (float*)&Config::DrawLight);
            }

            ImGui::Checkbox("Custom FOV", &Config::fovEnabled);
            if (Config::fovEnabled) {
                ImGui::SliderFloat("FOV Value##FovSlider", &Config::fov, 20.0f, 160.0f, "%1.0f");
            }

            ImGui::Checkbox("View model", &Config::viewmodell);
            if (Config::viewmodell) {
                ImGui::SliderFloat("X", &Config::viewx, -20.f, 20.f);
                ImGui::SliderFloat("Y", &Config::viewy, -20.f, 20.f);
                ImGui::SliderFloat("Z", &Config::viewz, -20.f, 20.f);
                ImGui::SliderFloat("Fov view model", &Config::ifov, 60.f, 150.f);
            }

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("VisualsRight", ImVec2(0, 0), true);
            ImGui::Text("Chams");
            ImGui::Separator();

            const char* chamsMaterials[] = { "Flat", "Illuminate", "Glow" };
            ImGui::Combo("Material", &Config::chamsMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::Checkbox("Visible##ChamsCheckbox", &Config::enemyChams);
            if (Config::enemyChams) {
                ImGui::ColorEdit4("Chams Color##ChamsColor", (float*)&Config::colVisualChams);
            }
            ImGui::Checkbox("Invisible", &Config::enemyChamsInvisible);
            if (Config::enemyChamsInvisible) {
                ImGui::ColorEdit4("XQZ Color##ChamsXQZColor", (float*)&Config::colVisualChamsIgnoreZ);
            }

            ImGui::Spacing();
            ImGui::Text("Hand Chams");
            ImGui::Separator();

            ImGui::Checkbox("Hand Chams", &Config::armChams);
            if (Config::armChams) {
                ImGui::ColorEdit4("Hand Color##HandChamsColor", (float*)&Config::colArmChams);
            }
            ImGui::Checkbox("Viewmodel Chams", &Config::viewmodelChams);
            if (Config::viewmodelChams) {
                ImGui::ColorEdit4("Viewmodel Color##ViewModelChamsColor", (float*)&Config::colViewmodelChams);
                ImGui::Checkbox("Rainbow", &Config::rainbow);
            }

            ImGui::Spacing();
            ImGui::Text("Removals");
            ImGui::Separator();
            ImGui::Checkbox("Anti Flash", &Config::antiflash);
            ImGui::Checkbox("Remove Smoke", &Config::SmokeRemove);
            ImGui::Checkbox("Remove Scope", &Config::ScopeRemove);
            ImGui::Checkbox("Remove Legs", &Config::removelegs);
            ImGui::EndChild();
        }
        break;

        case 2:
        {
            ImGui::BeginChild("MiscLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("Movement");
            ImGui::Separator();
            ImGui::Text("No additional settings");

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("MiscRight", ImVec2(0, 0), true);
            ImGui::Text("Other");

            ImGui::Checkbox("Enable Spammer", &Config::spammer);

            const char* spamOptions[] = { "SharkHack", "AimWare", "Luno" };
            ImGui::Combo("Spammer Type", &Config::spammingcho, spamOptions, IM_ARRAYSIZE(spamOptions));

            ImGui::Separator();

            // ImGui::Text("No additional settings");

            ImGui::EndChild();
        }
        break;

        case 3:
        {
            ImGui::BeginChild("ConfigLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("General");
            ImGui::Separator();

            static char configName[128] = "";
            static std::vector<std::string> configList = internal_config::ConfigManager::ListConfigs();
            static int selectedConfigIndex = -1;

            ImGui::InputText("Config Name", configName, IM_ARRAYSIZE(configName));

            if (ImGui::Button("Refresh")) {
                configList = internal_config::ConfigManager::ListConfigs();
            }
            ImGui::SameLine();
            if (ImGui::Button("Load")) {
                internal_config::ConfigManager::Load(configName);
            }
            ImGui::SameLine();
            if (ImGui::Button("Save")) {
                internal_config::ConfigManager::Save(configName);
                configList = internal_config::ConfigManager::ListConfigs();
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                internal_config::ConfigManager::Remove(configName);
                configList = internal_config::ConfigManager::ListConfigs();
            }

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("ConfigRight", ImVec2(0, 0), true);
            ImGui::Text("Saved Configs");
            ImGui::Separator();

            for (int i = 0; i < static_cast<int>(configList.size()); i++) {
                if (ImGui::Selectable(configList[i].c_str(), selectedConfigIndex == i)) {
                    selectedConfigIndex = i;
                    strncpy_s(configName, sizeof(configName), configList[i].c_str(), _TRUNCATE);
                }
            }

            ImGui::EndChild();
        }
        break;
        }

        ImGui::EndChild();
        ImGui::End();
    }
}


void* __fastcall H::hkIsRelativeMouseMode(void* pThisptr, bool bActive) {
    auto res = IsRelativeMouseMode.GetOriginal();
    Menu::sdk = bActive;
    return res(pThisptr, Menu::IsMenuOpen() ? false : bActive);
}

bool __fastcall H::hkMouseInputEnabled(void* rcx) {
    auto res = MouseInputEnabled.GetOriginal();
    return Menu::ShouldBlockMouseInput() ? false : res(rcx);
}


void Menu::toggleMenu() {
    showMenu = !showMenu;
    auto repet = H::IsRelativeMouseMode.GetOriginal();
    repet(I::InputSys, Menu::IsMenuOpen() ? false : Menu::sdk);
}
