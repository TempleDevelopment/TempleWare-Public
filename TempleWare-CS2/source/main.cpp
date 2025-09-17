#include "debug/debug.h"

#include "includes.h"
#include "templeware/templeware.h"
#include "templeware/renderer/icons.h"
#include "templeware/menu/menu.h"

#include "../external/kiero/minhook/include/MinHook.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TempleWare templeWare;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }

    if (Menu::IsMenuOpen()) {
        switch (uMsg) {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_CHAR:
            return 0;
        }
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
#ifdef TEMPLEDEBUG
            initDebug();
#endif


            templeWare.init(window, pDevice, pContext, mainRenderTargetView);
            init = true;
        }
        else
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    ImFontConfig imIconsConfig;
    imIconsConfig.RasterizerMultiply = 1.2f;

    constexpr ImWchar wIconRanges[] =
    {
        0xE000, 0xF8FF, // Private Use Area
        0
    };

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        templeWare.renderer.menu.toggleMenu();
    }

    templeWare.renderer.menu.render();
    templeWare.renderer.hud.render();

    // Always call esp() to allow individual components to be rendered
    templeWare.renderer.visuals.esp();
    templeWare.renderer.hud.DrawCustomScope();

    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

void init_console() {
    if (::AllocConsole()) {
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);  // Redirect stdout to console
        freopen_s(&f, "CONOUT$", "w", stderr);  // Redirect stderr to console
        ::SetConsoleTitleW(L"TempleWare");

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf(R"(___________                   .__         __      __                       
\__    ___/___   _____ ______ |  |   ____/  \    /  \_____ _______   ____  
  |    |_/ __ \ /     \\____ \|  | _/ __ \   \/\/   /\__  \\_  __ \_/ __ \ 
  |    |\  ___/|  Y Y  \  |_> >  |_\  ___/\        /  / __ \|  | \/\  ___/ 
  |____| \___  >__|_|  /   __/|____/\___  >\__/\  /  (____  /__|    \___  >
             \/      \/|__|             \/      \/        \/            \/ 
)");

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("[");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("+");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("] Console allocated successfully!\n");
    }
}
DWORD WINAPI MainThread(LPVOID lpReserved)
{
    bool init_hook = false;
    do
    {
        init_console();

        // hook hkPresent and init cheat
        if (!init_hook) {
            if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
            {
                kiero::bind(8, (void**)&oPresent, hkPresent);
                init_hook = true;
            }
        }
    } while (!GetAsyncKeyState(VK_F4));

    if (oWndProc != nullptr)
    {
        // restore wnd proc
        SetWindowLongPtrW(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));

        // invalidate old wnd proc
        oWndProc = nullptr;
    }

    kiero::shutdown();

    // destroy minhook
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    // free allocated memory for console
    ::FreeConsole();

    // close console window
    if (const HWND hConsoleWindow = ::GetConsoleWindow(); hConsoleWindow != nullptr)
        ::PostMessageW(hConsoleWindow, WM_CLOSE, 0U, 0L);

    fclose(stdout);
    fclose(stderr);

    // close thread
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpReserved), EXIT_SUCCESS);

    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        break;
    }
    return TRUE;
}
