#pragma once

#include <asw/asw.h>
#include <format>
#include <memory>
#include <string>

#include "./scenes.h"

class MenuScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {

        // Load fonts
        const asw::Font font_small = asw::assets::load_font("assets/fonts/jersey-10.ttf", 48);
        const asw::Font font_large = asw::assets::load_font("assets/fonts/jersey-10.ttf", 100);

        // Build UI tree
        ui_root_ = asw::ui::Root();
        ui_root_.root.transform = { 0, 0, 1280, 960 };
        ui_root_.root.bg_image = asw::assets::load_texture("assets/images/background.png");

        auto& column = ui_root_.root.add_child<asw::ui::VBox>();
        column.transform = { 40, 700, 420, 400 };
        column.padding = 0;
        column.gap = 10;

        auto& play = column.add_child<asw::ui::Button>();
        play.text = "Play";
        play.transform.size.y = 48;
        play.on_click = [this]() { manager.set_next_scene(GameState::LevelSelect); };
        play.font = font_small;

        auto& help = column.add_child<asw::ui::Button>();
        help.text = "Help";
        help.transform.size.y = 48;
        help.on_click = [this]() { spr_help_->visible = !spr_help_->visible; };
        help.font = font_small;

        auto& quit = column.add_child<asw::ui::Button>();
        quit.text = "Quit";
        quit.transform.size.y = 48;
        quit.on_click = []() { asw::core::exit = true; };
        quit.font = font_small;

        // Add text
        auto& title_text = ui_root_.root.add_child<asw::ui::Label>();
        title_text.font = font_large;
        title_text.text = "Mazes";
        title_text.color = palette::white;
        title_text.justify = asw::TextJustify::Left;
        title_text.transform.set_position(40, 540);

        auto& copyright_text = ui_root_.root.add_child<asw::ui::Label>();
        copyright_text.font = font_small;
        copyright_text.text = "© 2014 A.D.S. Games";
        copyright_text.color = palette::white;
        copyright_text.justify = asw::TextJustify::Right;
        copyright_text.transform.set_position(1240, 900);

        // Help
        spr_help_ = &ui_root_.root.add_child<asw::ui::Panel>();
        spr_help_->bg_image = asw::assets::load_texture("assets/images/help.png");
        spr_help_->transform.set_size(1280, 960);
        spr_help_->visible = false;
    }

    void update(float dt) override
    {
        Scene::update(dt);

        if (spr_help_->visible
            && (asw::input::keyboard.any_pressed
                || asw::input::get_mouse_button(asw::input::MouseButton::Left))) {
            spr_help_->visible = false;
            return;
        }

        ui_root_.update();
    }

    void draw() override
    {
        Scene::draw();
        ui_root_.draw();
    }

private:
    // Help open
    asw::ui::Panel* spr_help_ { nullptr };
    asw::ui::Root ui_root_;
};