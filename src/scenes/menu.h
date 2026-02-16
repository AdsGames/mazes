#pragma once

#include <asw/asw.h>
#include <format>
#include <memory>
#include <string>

#include "../button.h"
#include "./scenes.h"

class MenuScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        // Load sprites
        auto background = createObject<asw::game::Sprite>();
        background->setTexture(asw::assets::loadTexture("assets/images/menu.png"));

        // Setup buttons
        btn_start = createObject<Button>();
        btn_help = createObject<Button>();
        btn_quit = createObject<Button>();

        btn_start->setImages(
            "assets/images/buttons/start.png", "assets/images/buttons/start_hover.png");
        btn_help->setImages(
            "assets/images/buttons/help.png", "assets/images/buttons/help_hover.png");
        btn_quit->setImages(
            "assets/images/buttons/quit.png", "assets/images/buttons/quit_hover.png");

        btn_start->transform.setPosition(380, 240);
        btn_help->transform.setPosition(380, 380);
        btn_quit->transform.setPosition(380, 520);

        // Help
        spr_help = createObject<asw::game::Sprite>();
        spr_help->setTexture(asw::assets::loadTexture("assets/images/help.png"));
        spr_help->active = false;
    }

    void update(float deltaTime) override
    {
        using namespace asw::input;

        Scene::update(deltaTime);

        // Checks for mouse press
        if (getMouseButtonDown(MouseButton::Left) && btn_start->hover()) {
            sceneManager.setNextScene(GameState::LevelSelect);
        }

        if (getMouseButtonDown(MouseButton::Left) && btn_help->hover() && !spr_help->active) {
            spr_help->active = !spr_help->active;
        } else if (getMouseButtonDown(MouseButton::Left) && spr_help->active) {
            spr_help->active = false;
        }

        if (getMouseButtonDown(MouseButton::Left) && btn_quit->hover()) {
            asw::core::exit = true;
        }

        // Help
        if (spr_help->active && keyboard.anyPressed) {
            spr_help->active = false;
        } else if (getKeyDown(Key::Escape)) {
            asw::core::exit = true;
        }
    }

private:
    // Creates Buttons
    std::shared_ptr<Button> btn_start;
    std::shared_ptr<Button> btn_help;
    std::shared_ptr<Button> btn_quit;

    // Help open
    std::shared_ptr<asw::game::Sprite> spr_help;
};