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

        // Load fonts
        font_small = asw::assets::loadFont("assets/fonts/jersey-10.ttf", 48);
        font_large = asw::assets::loadFont("assets/fonts/jersey-10.ttf", 100);

        // Load sprites
        auto background = createObject<asw::game::Sprite>();
        background->setTexture(asw::assets::loadTexture("assets/images/background.png"));

        // Setup buttons
        btn_start = createObject<Button>();
        btn_start->setColor(palette::yellow);
        btn_start->setText("Start");
        btn_start->setFont(font_small);
        btn_start->transform.setPosition(40, 656);
        btn_start->transform.setSize(200, 64);

        btn_help = createObject<Button>();
        btn_help->setColor(palette::purple_blue);
        btn_help->setText("Help");
        btn_help->setFont(font_small);
        btn_help->transform.setPosition(40, 756);
        btn_help->transform.setSize(200, 64);

        btn_quit = createObject<Button>();
        btn_quit->setColor(palette::red);
        btn_quit->setText("Quit");
        btn_quit->setFont(font_small);
        btn_quit->transform.setPosition(40, 856);
        btn_quit->transform.setSize(200, 64);

        // Add text
        auto title_text = createObject<asw::game::Text>();
        title_text->setFont(font_large);
        title_text->setText("Mazes");
        title_text->setColor(palette::white);
        title_text->setJustify(asw::TextJustify::LEFT);
        title_text->transform.setPosition(40, 540);

        auto copyright_text = createObject<asw::game::Text>();
        copyright_text->setFont(font_small);
        copyright_text->setText("© 2014 A.D.S. Games");
        copyright_text->setColor(palette::white);
        copyright_text->setJustify(asw::TextJustify::RIGHT);
        copyright_text->transform.setPosition(1240, 900);

        // Help
        spr_help = createObject<asw::game::Sprite>();
        spr_help->setTexture(asw::assets::loadTexture("assets/images/help.png"));
        spr_help->active = false;
    }

    void update(float dt) override
    {
        using namespace asw::input;

        Scene::update(dt);

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

    // Fonts
    asw::Font font_small;
    asw::Font font_large;
};