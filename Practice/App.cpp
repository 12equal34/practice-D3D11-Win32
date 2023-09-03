#include <sstream>
#include "App.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

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
      m_appState(AppState::Initialize),
      m_shouldRenderImgui(true)
{
    m_window.SetKeyboard(&m_keyboard);
    m_window.SetMouse(&m_mouse);
    m_window.SetTimer(&m_mainTimer);

    m_camera.SetNearFarZ(0.5f, 200.0f);

    auto& cameraCoord = m_camera.GetCoordinate();
    cameraCoord.SetPosition(0.0f, 30.0f, 0.0f);
    m_camera.SetPitchYaw(-DirectX::XM_PIDIV4, 0.0f);
}

App::~App() { }

int App::Run()
{
    SetupImgui();

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

    CleanupImgui();

    if (bGotMsg == -1) {
        throw Hardware::Window::HrException(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}

void App::HandleInput(float dt)
{
    // keyboard input handles
    while (const auto e = m_keyboard.ReadKey()) {
        const auto& kbEvent = e.value();

        // Bind or Unbind camera rotation from mouse movement.
        if (kbEvent.GetCode() == 'G' && kbEvent.IsRelease()) {
            m_camera.SetBindingMouseDeltaInput(
                !m_camera.IsBoundMouseDeltaInput());
        }
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
    if (m_shouldRenderImgui) {
        RunImgui(dt);
    }

    DebugHelpWindowTitle(dt);
    HandleInput(dt);

    m_map.UpdateDynamicObject(dt);
    m_map.Bind();
    m_camera.Bind();

    RenderFrame();
}

void App::RenderFrame()
{
    // Rendering
    auto& renderer = m_window.GetRenderer();

    renderer.ClearBuffer(0.0f, 0.0f, 0.0f);

    renderer.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.DrawDynamicObjects(m_camera, m_map.m_dynamicObjects);

    if (m_shouldRenderImgui) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

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
        const auto&       cameraCoord = m_camera.GetCoordinate();
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
        1000, 1000, 0.5f, gerstnerWaveGenerator());
    waterSurface->SetCenterPosition(m_camera.GetCoordinate().GetPosition());
    waterSurface->GetCoordinate().SetPositionY(0.0f);

    m_map.m_dynamicObjects.push_back(std::move(waterSurface));

    auto directionalLight = std::make_unique<World::Object::DirectionalLight>();

    m_map.m_directionalLightObjects.push_back(std::move(directionalLight));

    auto ambientLight =
        std::make_unique<World::Object::HemisphericAmbientLight>();
    ambientLight->SetLightBaseColor(0.6f, 0.6f, 0.6f, 1.0f);
    ambientLight->SetLightColorRange(0.3f, 0.3f, 0.3f, 1.0f);

    m_map.m_ambientLightObjects.push_back(std::move(ambientLight));
}

void App::ResetApp() { }

void App::SetupImgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_window.GetHwnd());
    ImGui_ImplDX11_Init(Hardware::DX::DXResource::GetDevice().Get(),
                        Hardware::DX::DXResource::GetContext().Get());
}

void App::CleanupImgui()
{
    // Cleanup Dear Imgui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void App::RunImgui(float dt)
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static gp::math::Orientation3D newDirLightOri {DirectX::XM_PIDIV4, 0.0f,
                                                   0.0f};
    static DirectX::XMFLOAT4       newDirLightColor {1.0f, 0.9f, 0.8f, 1.0f};
    static DirectX::XMFLOAT4 newAmbientLightBaseColor {0.3f, 0.3f, 0.8f, 1.0f};
    static DirectX::XMFLOAT4 newAmbientLightColorRange {0.2f, 0.2f, 0.2f, 1.0f};
    static float             newCameraMovementSpeed = 40.0f;
    static float             newCameraRotationSpeed = 10.0f;

    if (!ImGui::Begin("Water Simulation Program Written By Gyeongrok Min")) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SeparatorText("How to Control the Camera in Map:");
        ImGui::BulletText("camera movement : Input 'WASDQE' keys!");
        ImGui::BulletText("camera orientation : Move your mouse cursor!");
        ImGui::BulletText("camera orientation fix/unfix : Input a 'G' key!");

        ImGui::SeparatorText("Camera Controller:");
        ImGui::SliderFloat("camera movement speed", &newCameraMovementSpeed,
                           10.0f, 100.0f);
        m_camera.SetMoveSpeed(newCameraMovementSpeed);
        ImGui::SliderFloat("camera rotation speed", &newCameraRotationSpeed,
                           5.0f, 30.0f);
        m_camera.SetRotationSpeed(newCameraRotationSpeed);
    }

    if (ImGui::CollapsingHeader("Lights", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SeparatorText("Directional Light");
        ImGui::DragFloat3("Orientation", newDirLightOri.data, 0.1f);
        m_map.m_directionalLightObjects[0]->GetCoordinate().SetOrientation(
            newDirLightOri);

        ImGui::ColorEdit4("Color", (float*)&newDirLightColor);
        m_map.m_directionalLightObjects[0]->SetLightColor(
            newDirLightColor.x, newDirLightColor.y, newDirLightColor.z,
            newDirLightColor.w);

        ImGui::SeparatorText("Ambient Light");
        ImGui::ColorEdit4("Base Color", (float*)&newAmbientLightBaseColor);
        m_map.m_ambientLightObjects[0]->SetLightBaseColor(
            newAmbientLightBaseColor.x, newAmbientLightBaseColor.y,
            newAmbientLightBaseColor.z, newAmbientLightBaseColor.w);

        ImGui::ColorEdit4("Color Range", (float*)&newAmbientLightColorRange);
        m_map.m_ambientLightObjects[0]->SetLightColorRange(
            newAmbientLightColorRange.x, newAmbientLightColorRange.y,
            newAmbientLightColorRange.z, newAmbientLightColorRange.w);
    }

    ImGui::End();
}
