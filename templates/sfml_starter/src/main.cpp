#include "Keyboard.h"
#include "Screen.h"
#include "ScreenMainMenu.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui.h>

int main()
{
    sf::RenderWindow window({1280, 720}, "SFML");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    ImGui::SFML::Init(window);

    Keyboard keyboard;
    ScreenManager screens;
    screens.pushScreen(std::make_unique<ScreenMainMenu>(&screens));
    screens.update();

    sf::Clock updateClock;
    while (window.isOpen() && !screens.isEmpty()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            ImGui::SFML::ProcessEvent(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyReleased:
                    if (e.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                default:
                    break;
            }
        }
        auto dt = updateClock.restart();
        ImGui::SFML::Update(window, dt);

        screens.peekScreen().onInput();
        screens.peekScreen().onUpdate(dt);

        window.clear({0, 200, 100});
        screens.peekScreen().onRender(&window);

        screens.peekScreen().onGUI();
        ImGui::SFML::Render(window);

        window.display();
        screens.update();
    }
}