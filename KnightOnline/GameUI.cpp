#include "pch.h"
#if defined(ENABLE_IMGUI)
#include "GameUI.h"
#include "App.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Resources/fonts/IconsFontAwesome5.h"
#include "Resources/fonts/CalibriBold.h"
#include "Resources/fonts/fa-solid-900.h"

WNDPROC GameUI::s_GlobalWndProc = nullptr;

GameUI::GameUI()
    : m_DeviceRef(nullptr)
    , m_PresentParameters{}
{ }

GameUI::~GameUI() = default;

bool GameUI::Initialize(IDirect3DDevice9 * device)
{
    LOG_INFO("Initializing ImGui...");
    if (!device)
    {
        LOG_ERROR("IDirect3DDevice9 was nullptr.");
        return false;
    }
    m_DeviceRef = device;

    HRESULT hr = D3D_OK;

    // Get the swap-chain that created this device in order to retrieve present parameters for useful info.
    IDirect3DSwapChain9 * swapChain;
    hr = m_DeviceRef->GetSwapChain(0, &swapChain);
    if (hr != D3D_OK || !swapChain)
    {
        LOG_ERROR("GetSwapChain failed.");
        return false;
    }

    hr = swapChain->GetPresentParameters(&m_PresentParameters);
    if (hr != D3D_OK)
    {
        LOG_ERROR("GetPresentParameters failed.");
        return false;
    }

    // Forward messages from device handle window to us
    s_GlobalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_PresentParameters.hDeviceWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&GameUI::WndProc)));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.IniFilename = "app.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // Keep the game cursor unchanged.
    //io.ConfigDockingTransparentPayload = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    // Setting ConfigViewportsNoAutoMerge to false allows for window's transparency, but then the clicks are weirdly going through the parent window to child.
    io.ConfigViewportsNoAutoMerge = true;

    // Font
    fs::path temp(fs::temp_directory_path());
    std::string calibri = (temp / "CalibriBold.ttf").string();
    std::string faAwesome = (temp / "fa_solid_900.ttf").string();
    auto file = std::fstream(calibri.c_str(), std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char *>(Font_CalibriBold), sizeof(Font_CalibriBold));
    file.close();
    file = std::fstream(faAwesome.c_str(), std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char *>(Font_fa_solid_900), sizeof(Font_fa_solid_900));
    file.close();

    io.Fonts->Clear();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig         fontConfigs;
    fontConfigs.MergeMode = true;
    fontConfigs.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(calibri.c_str(), 16.0f);
    io.Fonts->AddFontFromFileTTF(faAwesome.c_str(), 14.0f, &fontConfigs, icons_ranges);

    // NOTE: When loading from memory, for some reason app crashes when closing the app on debug mode.
    //static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    //ImFontConfig         fontConfigs;
    //fontConfigs.MergeMode = true;
    //fontConfigs.PixelSnapH = true;
    //io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<const char *>(Font_CalibriBold), sizeof(Font_CalibriBold), 15.0f);
    //io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<const char *>(Font_fa_solid_900), sizeof(Font_fa_solid_900), 16.0f, &fontConfigs, icons_ranges);

    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle & style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    //ImGui::SetNextWindowBgAlpha(1.0f);
    style.WindowTitleAlign = ImVec2{ 0.50f, 0.50f };
    ImGui_ImplWin32_Init(m_PresentParameters.hDeviceWindow);
    ImGui_ImplDX9_Init(m_DeviceRef);

    return true;
}

void GameUI::Tick()
{
}

void GameUI::Render()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    RenderDockingSpace();

    RenderUI();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GameUI::RenderUI()
{
    ImGui::Begin("FPS Control  " ICON_FA_TV);
    ImGui::Text("Game average %.3f ms/frame (%.1f FPS)", *App::s_Data.fSecPerFrm * 1000.0f, *App::s_Data.fFrmPerSec);
    ImGui::Text("UI average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // [Frame] SecPerFrame=11235
    float fSecPerFrm = App::s_Data.fSecPerFrmPrecise * 1000000.0f;
    if (ImGui::SliderFloat("User Defined FPS", &fSecPerFrm, 1, 100000, ""))
        App::s_Data.fSecPerFrmPrecise = fSecPerFrm / 1000000.0f;

    if (ImGui::Button(ICON_FA_HISTORY" Restore Defaults"))
    {
        App::s_Data.fSecPerFrmPrecise = 0.01400000f;
        App::s_Data.bFPSControlEnabled = true;
    }
    ImGui::SameLine();
    ImGui::Checkbox("FPS Control Enabled", &App::s_Data.bFPSControlEnabled);

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void GameUI::RenderDockingSpace()
{
    static ImGuiWindowFlags s_dockWindowFlags =
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // Make sure the docking window fully covers the whole region.
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    // Foreground window style: we don't need it visible, so disable some props.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Begin
    ImGui::Begin("Dockspace", nullptr, s_dockWindowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    // Dockspace
    ImGuiID   dockId = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // End
    ImGui::End();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GameUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT * suggested_rect = (RECT *)lParam;
            ::SetWindowPos(hWnd, NULL,
                suggested_rect->left, suggested_rect->top,
                suggested_rect->right - suggested_rect->left,
                suggested_rect->bottom - suggested_rect->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }

    return CallWindowProc(s_GlobalWndProc, hWnd, uMsg, wParam, lParam);
}

#endif // #if defined(ENABLE_IMGUI)
