#include <sstream>
#include "App.h"
#include "ConstantBuffer.h"

#include "DXResource.h"
#include "DirectionalLight.h"
#include "WaterSurface.h"

App::App()
    : m_window(1000, 600, L"Window Sample"),
      m_keyboard(&m_window),
      m_mouse(&m_window),
      m_mainTimer()
{
    m_window.SetKeyboard(&m_keyboard);
    m_window.SetMouse(&m_mouse);
    m_window.SetTimer(&m_mainTimer);

    auto& cameraCoord = m_camera.GetCoordinate();
    cameraCoord.SetPosition({0.0f, 10.0f, 0.0f});
    cameraCoord.SetOrientation({0.0f, 0.0f, 0.0f});
}

App::~App() { }

int App::Run()
{
    MSG msg {};
    msg.message = WM_NULL;
    BOOL bGotMsg;

    while (WM_QUIT != msg.message) {
        bGotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

        if (bGotMsg) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            float dt = static_cast<float>(m_mainTimer.Mark());
            DebugHelpWindowTitle(dt);
            HandleInput(dt);
            RunFrame(dt);
        }
    }

    if (bGotMsg == -1) {
        throw Hardware::Window::HrException(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}

void App::HandleInput(float dt)
{
    static constexpr float cameraRotationSpeed = 10.0f;
    static constexpr float cameraMoveSpeed     = 10.0f;

    // keyboard input handles
    while (const auto e = m_keyboard.ReadKey()) {
        // omitted
    }

    auto&      cameraCoord = m_camera.GetCoordinate();
    const auto step        = cameraMoveSpeed * dt;
    if (m_keyboard.KeyIsPressed('W')) cameraCoord.GoForward(step);
    if (m_keyboard.KeyIsPressed('S')) cameraCoord.GoBackward(step);
    if (m_keyboard.KeyIsPressed('Q')) cameraCoord.GoUp(step);
    if (m_keyboard.KeyIsPressed('E')) cameraCoord.GoDown(step);
    if (m_keyboard.KeyIsPressed('D')) cameraCoord.GoRight(step);
    if (m_keyboard.KeyIsPressed('A')) cameraCoord.GoLeft(step);

    // mouse input handles
    while (const auto xy = m_mouse.Read()) {
    }
    while (const auto dxy = m_mouse.ReadDelta()) {
        const auto& cr_xy = dxy.value();
        auto dyaw   = cameraRotationSpeed * cr_xy.first / m_window.GetWidth();
        auto dpitch = cameraRotationSpeed * cr_xy.second / m_window.GetHeight();
        // m_camera.Rotate(dpitch, dyaw, 0.0f);
        cameraCoord.Rotate({dpitch, dyaw, 0.0f});
    }
}

void App::RunFrame(float dt)
{
    using namespace DirectX;
    using namespace Hardware::DX;
    auto& renderer = m_window.GetRenderer();


    std::vector<std::unique_ptr<World::Object::Object>> objects;
    auto waterSurface = std::make_unique<World::Object::WaterSurface>(
        150, 150, 0.5f,
        World::Object::WaterSurface::TestWaveGenerator(
            static_cast<float>(m_mainTimer.Time())));
    objects.push_back(std::move(waterSurface));

    for (auto& obj : objects) {
        obj->Bind();
    }

    auto directionalLight = std::make_unique<World::Object::DirectionalLight>();
    directionalLight->SetLightColor(0.6f, 0.7f, 0.7f, 1.0f);
    directionalLight->GetCoordinate().SetOrientation({ -XM_PIDIV2, 0.0f, 0.0f });
    directionalLight->Bind();

    DXResource::GetContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    renderer.ClearBuffer(0.0f, 0.0f, 0.0f);
    m_camera.Bind();
    renderer.DrawObjects(m_camera, objects);
    renderer.EndFrame();
}

void App::DebugHelpWindowTitle(float dt)
{
    // write times at the window title
    double                  totalTime = m_mainTimer.TimeSinceStart();
    static int              i         = 0;
    static constexpr double interval  = 0.1;
    static int              frames    = 0;
    ++frames;
    if (totalTime > interval * i) {
        double fps = frames / interval;
        frames     = 0;
        ++i;

        std::stringstream mouseStr;
        mouseStr << '(' << m_mouse.GetNormalizedX() << ','
                 << m_mouse.GetNormalizedY() << ')';

        std::stringstream cameraPosStr;
        auto&             cameraCoord = m_camera.GetCoordinate();
        cameraPosStr << '(' << cameraCoord.GetPositionX() << ','
                     << cameraCoord.GetPositionY() << ','
                     << cameraCoord.GetPositionZ() << ')';

        std::stringstream cameraOriStr;
        cameraOriStr << '(' << cameraCoord.GetOrientationX() << ','
                     << cameraCoord.GetOrientationY() << ','
                     << cameraCoord.GetOrientationZ() << ')';

        std::stringstream wo;
        wo << "Timer : " << std::left << std::setw(16) << totalTime
           << "fps: " << std::left << std::setw(16) << fps;
        wo << "Mouse : " << std::left << std::setw(30) << mouseStr.str();
        wo << "Camera.pos : " << std::left << std::setw(30)
           << cameraPosStr.str();
        wo << "Camera.ori : " << std::left << std::setw(30)
           << cameraOriStr.str();

        auto         str = wo.str();
        std::wstring title(str.begin(), str.end());
        m_window.SetTitle(title);
    }
}
