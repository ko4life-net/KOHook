#pragma once
#if defined(ENABLE_IMGUI)

#include <d3d9.h>
#include <memory>

class GameUI
{
public:
    GameUI();
    ~GameUI();

    bool Initialize(IDirect3DDevice9 * device);

    void Tick();
    void Render();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    void RenderUI();
    void RenderDockingSpace();

private:
    IDirect3DDevice9 *    m_DeviceRef;
    D3DPRESENT_PARAMETERS m_PresentParameters;

    static WNDPROC s_GlobalWndProc;
};

#endif // #if defined(ENABLE_IMGUI)