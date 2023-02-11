#include "Game.h"

#include "Utility.h"
#include <imgui.h>

Game::Game(unsigned windowWidth, unsigned windowHeight)
    : m_spriteRenderer(windowWidth, windowHeight)
{
    // clang-format off
    m_sceneShader   .loadFromFile("SceneVertex.glsl",   "SceneFragment.glsl");
    m_terrain       .bufferMesh(createTerrainMesh(256, 256, true));
    m_lightCube     .bufferMesh(createCubeMesh({10.5f, 10.5f, 10.5f}));
    m_texture       .loadFromFile("OpenGLLogo.png", 8);

    // Get window width and height from imgui
    ImGui::GetIO().DisplaySize.x;

    float aspect = (float)windowWidth / (float)windowHeight;
    m_projectionMatrix = createProjectionMatrix(aspect, 90.0f);

    m_cameraTransform = { {0, 15, 0}, {0, 0, 0} };
    m_lightTransform = { {0, 15, 0}, {0, 0, 0} };

    // clang-format on
}

void Game::onInput(const Keyboard& keyboard, const sf::Window& window, bool isMouseActive)
{
    Transform& camera = m_cameraTransform;

    float PLAYER_SPEED = 0.5f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED = 5.0f;
    }
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        camera.position += forwardsVector(camera.rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        camera.position += backwardsVector(camera.rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        camera.position += leftVector(camera.rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        camera.position += rightVector(camera.rotation) * PLAYER_SPEED;
    }

    if (!isMouseActive) {
        return;
    }
    static auto lastMousePosition = sf::Mouse::getPosition();
    auto change = sf::Mouse::getPosition() - lastMousePosition;
    camera.rotation.x -= static_cast<float>(change.y * 0.5);
    camera.rotation.y += static_cast<float>(change.x * 0.5);
    sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2},
                           window);
    //  lastMousePosition.x = (int)window.getSize().x / 2;
    //  lastMousePosition.y = (int)window.getSize().y / 2;
    lastMousePosition = sf::Mouse::getPosition();

    camera.rotation.x = glm::clamp(camera.rotation.x, -89.9f, 89.9f);
    camera.rotation.y = (int)camera.rotation.y % 360;
}

void Game::onUpdate()
{
    m_lightTransform.position.y += std::sin(m_timer.getElapsedTime().asSeconds());
}

void Game::onRender(const sf::Window& window)
{
    setClearColour(COLOUR_SKY_BLUE);

    auto viewMatrix = createViewMartix(m_cameraTransform, {0, 1, 0});
    auto projectionViewMatrix = m_projectionMatrix * viewMatrix;
    m_frustum.update(projectionViewMatrix);

    Framebuffer::unbind(window.getSize().x, window.getSize().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderScene(projectionViewMatrix);

    m_spriteRenderer.render(m_texture, 0, 0, 100, 100);
}

void Game::onGUI() { guiDebugScreen(m_cameraTransform); }

void Game::onResize(int width, int height)
{
    float aspect = (float)width / (float)height;
    m_projectionMatrix = createProjectionMatrix(aspect, 90.0f);

    m_spriteRenderer.onWindowResize(width, height);
}

void Game::renderScene(const glm::mat4& projectionViewMatrix)
{
    // Normal stuff
    glDisable(GL_CULL_FACE);
    m_sceneShader.bind();
    m_sceneShader.set("projectionViewMatrix", projectionViewMatrix);
    m_sceneShader.set("isLight", false);
    m_sceneShader.set("lightColour", glm::vec3{1.0, 1.0, 1.0});
    m_sceneShader.set("eyePosition", m_cameraTransform.position);

    m_texture.bind(0);

    glm::mat4 terrainModel{1.0f};
    terrainModel = glm::translate(terrainModel, {0, 0, 0});
    m_sceneShader.set("modelMatrix", terrainModel);
    m_sceneShader.set("lightPosition", m_lightTransform.position);
    m_terrain.getRendable().drawElements();

    glEnable(GL_CULL_FACE);
    auto lightModel = createModelMatrix(m_lightTransform);
    m_sceneShader.set("modelMatrix", lightModel);
    m_sceneShader.set("isLight", true);
    m_lightCube.getRendable().drawElements();
}
