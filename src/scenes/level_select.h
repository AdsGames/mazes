#pragma once

#include <asw/asw.h>
#include <fstream>
#include <memory>

#include "../colors.h"
#include "../globals.h"
#include "../tilemap.h"
#include "./game.h"
#include "./scenes.h"

class LevelSelectScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        // Load font
        const auto font = asw::assets::load_font("assets/fonts/jersey-10.ttf", 48);

        // Build UI tree
        _ui_root = asw::ui::Root();
        _ui_root.root.transform.set_size(1280, 960);
        _ui_root.root.bg_image = asw::assets::load_texture("assets/images/background.png");

        // Setup buttons
        auto& btn_left = _ui_root.root.add_child<asw::ui::Button>();
        btn_left.text = "<";
        btn_left.font = font;
        btn_left.transform.set_position(100, 420);
        btn_left.transform.set_size(64, 64);
        btn_left.on_click = [this]() { previous_level(); };

        auto& btn_play = _ui_root.root.add_child<asw::ui::Button>();
        btn_play.transform.set_position(320, 220);
        btn_play.transform.set_size(640, 480);
        btn_play.on_click = [this]() { manager.set_next_scene(GameState::Game); };

        auto& btn_right = _ui_root.root.add_child<asw::ui::Button>();
        btn_right.text = ">";
        btn_right.font = font;
        btn_right.transform.set_position(1280 - 64 - 100, 420);
        btn_right.transform.set_size(64, 64);
        btn_right.on_click = [this]() { next_level(); };

        auto& back = _ui_root.root.add_child<asw::ui::Button>();
        back.text = "Back";
        back.font = font;
        back.transform.set_position(40, 856);
        back.transform.set_size(200, 64);
        back.on_click = [this]() { manager.set_next_scene(GameState::Menu); };

        // Level text
        auto& level_text = _ui_root.root.add_child<asw::ui::Label>();
        level_text.font = font;
        level_text.color = palette::white;
        level_text.justify = asw::TextJustify::Center;
        level_text.transform.set_position(640, 760);
        _level_text_ref = &level_text;

        // Load sprites
        // Load tilemap
        _tilemap.load(std::format("assets/levels/level{}.json", GameScene::level));
        _tilemap.setRenderConfig(
            { .tile_size = 20, .render_size = 30, .offset_x = 320, .offset_y = 220 });
    }

    void update(float dt) override
    {
        Scene::update(dt);

        // Text
        _level_text_ref->text
            = std::format("Level {}: {}", GameScene::level, _tilemap.getLevelText());

        // UI State
        _ui_root.update();

        // Keyboard shortcuts
        if (asw::input::is_action_pressed("back")) {
            manager.set_next_scene(GameState::Menu);
        }
        if (asw::input::is_action_pressed("left")) {
            previous_level();
        }
        if (asw::input::is_action_pressed("right")) {
            next_level();
        }
        if (asw::input::is_action_pressed("interact")) {
            select_level();
        }
    }

    void draw() override
    {
        Scene::draw();

        _ui_root.draw();

        _tilemap.renderBackground();

        // Mini tiles tiles
        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                _tilemap.render({ i, t });
            }
        }
    }

private:
    asw::ui::Root _ui_root;
    asw::ui::Label* _level_text_ref;
    asw::Sample _click;

    TileMap _tilemap;

    void next_level()
    {
        const auto file_path = std::format("assets/levels/level{}.json", GameScene::level + 1);
        if (_tilemap.load(file_path)) {
            asw::sound::play(_click);
            GameScene::level++;
        }
    }

    void previous_level()
    {
        const auto file_path = std::format("assets/levels/level{}.json", GameScene::level - 1);
        if (_tilemap.load(file_path)) {
            asw::sound::play(_click);
            GameScene::level--;
        }
    }

    void select_level()
    {
        manager.set_next_scene(GameState::Game);
    }
};