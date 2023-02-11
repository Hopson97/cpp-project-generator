#include "GUI.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_wrapper.h"
#include <imgui.h>
#include <iomanip>
#include <locale>
#include <sstream>

#include "Game.h"
#include "Maths.h"
#include "Utility.h"

template <class T>
std::string formatNumber(T value)
{
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << value;
    return ss.str();
}

sf::Clock fpsClock;
float frames = 0;
float fps;
float frameTime = 0;

int window_flags = 0;

void guiInit(sf::Window& window)
{
    ImGui_SFML_OpenGL3::init(window);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2;
    style.FrameRounding = 0;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.TabRounding = 6;
}

void guiShutdown() { ImGui_SFML_OpenGL3::shutdown(); }

void guiBeginFrame() { ImGui_SFML_OpenGL3::startFrame(); }

void guiProcessEvent(sf::Event& event) { ImGui_ImplSfml_ProcessEvent(event); }

void guiEndFrame() { ImGui_SFML_OpenGL3::endFrame(); }

void guiDebugScreen(const Transform& transform)
{
    static bool wireframe = false;

    if (ImGui::Begin("Debug Window", nullptr, window_flags)) {
        ImGui::Text("Player Position: (%.*f, %.*f, %.*f)", 2, transform.position[0], 2,
                    transform.position[1], 2, transform.position[2]);

        ImGui::Text("Player Rotation: (%.*f, %.*f, %.*f)", 2, transform.rotation[0], 2,
                    transform.rotation[1], 2, transform.rotation[2]);

        ImGui::Text("FPS: %f", fps);
        ImGui::Text("Frame Time: %f", frameTime);

        ImGui::Checkbox("Wireframe", &wireframe);
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        ImGui::End();
    }
}

SpriteRenderer::SpriteRenderer(unsigned width, unsigned height)
{
    m_guiShader.loadFromFile("GUIVertex.glsl", "GUIFragment.glsl");
    m_guiBorder.loadFromFile("GUIBorder.png", 1);
    onWindowResize(width, height);
    m_guiShader.bind();
    m_guiShader.set("guiTexture", 0);
    m_guiShader.set("borderTexture", 1);
}

void SpriteRenderer::onWindowResize(unsigned width, unsigned height)
{
    glm::mat4 orthoMatrix{1.0f};
    orthoMatrix = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
    m_guiShader.bind();
    m_guiShader.set("orthographicMatrix", orthoMatrix);
}

void SpriteRenderer::render(const Texture2D& texture, float x, float y, float width,
                            float height)
{
    // glEnable(GL_BLEND);
    texture.bind(0);
    m_guiBorder.bind(1);
    m_guiShader.bind();

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, {x, y, 0});
    modelMatrix = glm::scale(modelMatrix, {width, height, 1.0});
    m_guiShader.set("transform", modelMatrix);

    m_guiQuad.getRendable().drawArrays(6);
    // glDisable(GL_BLEND);
}