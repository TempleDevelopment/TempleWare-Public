#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"

class Menu {
public:
    Menu();

    static bool IsMenuOpen() { return instance ? instance->showMenu : false; }
    static bool ShouldBlockMouseInput() { return IsMenuOpen(); }
    static bool ShouldUseRelativeMouseMode() { return instance ? !instance->showMenu && instance->stack : false; }
    static inline bool sdk = false;

    void init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView);
    void render();
    void toggleMenu();

private:
    static Menu* instance;
    bool showMenu;
    int activeTab;
    bool stack;
};