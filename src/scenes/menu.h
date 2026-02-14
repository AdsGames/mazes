#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "./scenes.h"

class MenuScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        // Sets button images
        btn_start.SetImages(
            "assets/images/buttons/start.png", "assets/images/buttons/start_hover.png");
        btn_help.SetImages(
            "assets/images/buttons/help.png", "assets/images/buttons/help_hover.png");
        btn_quit.SetImages(
            "assets/images/buttons/quit.png", "assets/images/buttons/quit_hover.png");
        btn_mode.SetImages(
            "assets/images/buttons/mode_3d.png", "assets/images/buttons/mode_3d_hover.png");

        // Sets button positions
        btn_start.SetX(380);
        btn_help.SetX(380);
        btn_quit.SetX(380);
        btn_mode.SetX(380);

        btn_start.SetY(240);
        btn_help.SetY(380);
        btn_quit.SetY(520);
        btn_mode.SetY(660);

        // Load font
        font = asw::assets::loadFont("assets/fonts/dosis.ttf", 16);

        // Load sprites
        menu = asw::assets::loadTexture("assets/images/menu.png");
        help_menu = asw::assets::loadTexture("assets/images/help.png");

        // Help open
        helpOpen = false;
    }

    void update(float deltaTime) override
    {
        Scene::update(deltaTime);

        if (helpOpen) {
            if (asw::input::keyboard.anyPressed
                || asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
                helpOpen = false;
            }
        } else if (asw::input::getKeyDown(asw::input::Key::Escape)) {
            asw::core::exit = true;
        }

        // Checks for mouse press
        if (asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
            if (btn_start.Hover()) {
                sceneManager.setNextScene(GameState::LevelSelect);
            } else if (btn_help.Hover() && !helpOpen) {
                helpOpen = true;
            } else if (btn_quit.Hover()) {
                asw::core::exit = true;
            } else if (btn_mode.Hover()) {
                if (perspective == 0) {
                    perspective = 1;
                    btn_mode.SetImages("assets/images/buttons/mode_2d.png",
                        "assets/images/buttons/mode_2d_hover.png");
                } else if (perspective == 1) {
                    perspective = 0;
                    btn_mode.SetImages("assets/images/buttons/mode_3d.png",
                        "assets/images/buttons/mode_3d_hover.png");
                }
            }
        }
    }

    void draw() override
    {
        // Draws Menu
        asw::draw::sprite(menu, asw::Vec2<float>(0, 0));

        // Draws Buttons
        btn_start.draw();
        btn_help.draw();
        btn_quit.draw();
        btn_mode.draw();

        if (helpOpen) {
            asw::draw::sprite(help_menu, asw::Vec2<float>(0, 0));
        }
    }

private:
    // Creates Buttons
    Button btn_start;
    Button btn_help;
    Button btn_quit;
    Button btn_mode;

    asw::Font font;

    asw::Texture help_menu;
    asw::Texture menu;

    // Help open
    bool helpOpen;
};