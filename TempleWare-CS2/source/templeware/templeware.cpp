#include "templeware.h"

#include "utils/module/module.h"

#include <iostream>

void TempleWare::init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    auto printWithPrefix = [&](const char* message) {
        std::cout << "[";
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "+";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "] " << message << std::endl;
        };

    printWithPrefix("Initializing modules...");
    modules.init();

    printWithPrefix("Initializing menu...");
    renderer.menu.init(window, pDevice, pContext, mainRenderTargetView);

    printWithPrefix("Initializing schema...");
    schema.init("client.dll", 0);

    printWithPrefix("Initializing Interfaces...");
    interfaces.init();

    printWithPrefix("Initializing fonts...");
    renderer.hud.InitializeFonts();

    printWithPrefix("Initializing visuals...");
    renderer.visuals.init();

    printWithPrefix("Initializing materials...");
    materials.init();

    printWithPrefix("Initializing hooks...");
    hooks.init();

    printWithPrefix("Success...");
}
