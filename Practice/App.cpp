#include <sstream>
#include "App.h"
#include "ConstantBuffer.h"

#include "DXResource.h"
#include "DirectionalLight.h"
#include "WaterSurface.h"
#include "HemisphericAmbientLight.h"

App::App()
    : m_window(1000, 600, L"Window Sample"),
      m_keyboard(&m_window),
      m_mouse(&m_window),
      m_mainTimer(),
      m_appState(AppState::Initialize)
{
    m_window.SetKeyboard(&m_keyboard);
    m_window.SetMouse(&m_mouse);
    m_window.SetTimer(&m_mainTimer);

    auto& cameraCoord = m_camera.GetCoordinate();
    cameraCoord.SetPosition(0.0f, 10.0f, 0.0f);
    cameraCoord.SetOrientation(0.0f, 0.0f, 0.0f);
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
            switch (m_appState) {
            case AppState::Run:
            {
                float dt = static_cast<float>(m_mainTimer.Mark());
                RunFrame(dt);
                break;
            }
            case AppState::Initialize:
                InitializeApp();
                m_appState = AppState::Run;
                break;
            case AppState::Reset:
                ResetApp();
                m_appState = AppState::Run;
                break;
            default:
                break;
            }
        }
    }

    if (bGotMsg == -1) {
        throw Hardware::Window::HrException(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}

void App::HandleInput(float dt)
{
    // keyboard input handles
    while (const auto e = m_keyboard.ReadKey()) {
        // omitted
    }

    m_camera.HandleInputFromKeyboard(m_keyboard, dt);

    // mouse input handles
    while (const auto xy = m_mouse.Read()) {
    }
    while (const auto deltaMouseXY = m_mouse.ReadDelta()) {
        const auto& [deltaMouseX, deltaMouseY] = deltaMouseXY.value();

        m_camera.HandleInputFromMouseMovement(deltaMouseX, deltaMouseY);
    }
}

void App::RunFrame(float dt)
{
    DebugHelpWindowTitle(dt);
    HandleInput(dt);

    m_map.UpdateDynamicObject(dt);

    m_map.Bind();
    m_camera.Bind();

    auto& renderer = m_window.GetRenderer();

    Hardware::DX::DXResource::GetContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    renderer.ClearBuffer(0.0f, 0.0f, 0.0f);

    renderer.DrawDynamicObjects(dt, m_camera, m_map.m_dynamicObjects);
    renderer.EndFrame();
}

void App::DebugHelpWindowTitle(float dt)
{
    // write times at the window title
    static double           prevTime       = 0.0;
    static double           prevTimeForFPS = 0.0;
    static constexpr double intervalTime   = 1.0;
    static int              frames         = 0;

    ++frames;

    double totalTime       = m_mainTimer.TimeSinceStart();
    double deltaTime       = totalTime - prevTime;
    double deltaTimeForFPS = totalTime - prevTimeForFPS;

    static std::string fpsStr;
    if (1.0 < deltaTimeForFPS) {
        prevTimeForFPS = totalTime;

        double fps = frames;
        frames     = 0;

        std::stringstream wo;
        wo << "fps: " << std::left << std::setw(16) << fps;
        fpsStr = wo.str();
    }

    static std::string titleStr;
    if (0.1 < deltaTime) {
        prevTime = totalTime;

        std::stringstream mouseStr;
        mouseStr << '(' << m_mouse.GetNormalizedX() << ','
                 << m_mouse.GetNormalizedY() << ')';

        std::stringstream cameraPosStr;
        auto&             cameraCoord = m_camera.GetCoordinate();
        cameraPosStr << '(' << cameraCoord.GetPositionX() << ','
                     << cameraCoord.GetPositionY() << ','
                     << cameraCoord.GetPositionZ() << ')';

        std::stringstream cameraOriStr;
        cameraOriStr << '(' << m_camera.GetPitch() << ',' << m_camera.GetYaw()
                     << ',' << 0.0f << ')';

        std::stringstream wo;
        wo << "Timer : " << std::left << std::setw(16) << totalTime;

        wo << "Mouse : " << std::left << std::setw(30) << mouseStr.str();
        wo << "Camera.pos : " << std::left << std::setw(30)
           << cameraPosStr.str();
        wo << "Camera.ori : " << std::left << std::setw(30)
           << cameraOriStr.str();

        titleStr = fpsStr + wo.str();
        m_window.SetTitle(titleStr);
    }
}

void App::InitializeApp()
{
    auto gerstnerWaveGenerator =
        World::Object::Simulation::GerstnerWaveContainer::TestWaveGenerator();
    auto waterSurface = std::make_unique<World::Object::WaterSurface>(
        150, 150, 0.5f, gerstnerWaveGenerator());

    m_map.m_dynamicObjects.push_back(std::move(waterSurface));

    auto directionalLight = std::make_unique<World::Object::DirectionalLight>();
    directionalLight->SetLightColor(0.6f, 0.7f, 0.7f, 1.0f);
    directionalLight->GetCoordinate().SetOrientation(DirectX::XM_PIDIV2, 0.0f,
                                                     0.0f);

    m_map.m_directionalLightObjects.push_back(std::move(directionalLight));

    auto ambientLight =
        std::make_unique<World::Object::HemisphericAmbientLight>();
    ambientLight->SetLightBaseColor(0.6f, 0.6f, 0.6f, 1.0f);
    ambientLight->SetLightColorRange(0.3f, 0.3f, 0.3f, 1.0f);

    m_map.m_ambientLightObjects.push_back(std::move(ambientLight));
}

void App::ResetApp() { }
