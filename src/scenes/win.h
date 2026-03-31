
#pragma once

#include <asw/asw.h>

#include "../colors.h"
#include "../globals.h"
#include "./scenes.h"

class WinScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        winscreen = asw::assets::load_texture("assets/images/winscreen.png");
        font = asw::assets::load_font("assets/fonts/jersey-10.ttf", 64);
    }

    void update(float dt) override
    {
        Scene::update(dt);

        if (asw::input::get_action_down("interact") || asw::input::get_action_down("back")) {
            manager.set_next_scene(GameState::Menu);
        }
    }

    void draw() override
    {
        asw::draw::sprite(winscreen, asw::Vec2f(0, 0));
        asw::draw::text(font, std::to_string(score), asw::Vec2f(620, 850), palette::black);
    }

private:
    asw::Texture winscreen;
    asw::Font font;
};