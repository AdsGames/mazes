#pragma once

#include <array>
#include <asw/asw.h>
#include <cmath>
#include <ctime>
#include <format>
#include <sstream>
#include <string>

#include "../colors.h"
#include "../globals.h"
#include "../tilemap.h"
#include "./scenes.h"

class GameScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    // Constants
    static constexpr float ROBOT_MOVE_INTERVAL = 2.080F / 4.0F; // 4 moves per beat @ 125 BPM
    static constexpr float CHARACTER_MOVE_INTERVAL = 0.1F;
    static constexpr int DEFAULT_LIVES = 3;

    // State
    inline static int level = 1;

    void init() override
    {
        using namespace asw::assets;
        using namespace asw::sound;

        time_acc = 0.0F;
        move_acc = 0.0F;
        paused = false;
        lives = DEFAULT_LIVES;
        position = asw::Vec2i(40, 40);
        rotation = 0;
        has_broom = false;
        broom_active = false;
        won = false;
        level_complete = false;

        font = load_font("assets/fonts/dosis.ttf", 16);
        font_pause = load_font("assets/fonts/jersey-10.ttf", 32);

        // Tiles
        character[0] = load_texture("assets/images/blocks/character_down.png");
        character[1] = load_texture("assets/images/blocks/character_up.png");
        character[2] = load_texture("assets/images/blocks/character_left.png");
        character[3] = load_texture("assets/images/blocks/character_right.png");
        broom = load_texture("assets/images/blocks/broom.png");

        // Sounds and music
        sweep = load_sample("assets/sfx/sweep.wav");
        explode = load_sample("assets/sfx/explode.wav");
        trash = load_sample("assets/sfx/trash.wav");
        oof = load_sample("assets/sfx/oof.wav");
        winsound = load_sample("assets/sfx/winsound.wav");
        door = load_sample("assets/sfx/door.wav");
        hitwall = load_sample("assets/sfx/hitwall.wav");
        boxhitwall = load_sample("assets/sfx/boxhitwall.wav");
        boxslide = load_sample("assets/sfx/boxslide.wav");

        song = load_music("assets/sfx/music.ogg");

        score = 0;

        // Render config
        tilemap.load(std::format("assets/levels/level{}.json", level));
        tilemap.setRenderConfig(
            { .tile_size = 40, .render_size = 60, .offset_x = 0, .offset_y = 0 });

        // Background Music
        asw::sound::play_music(song, 1.0F, 1000);
    }

    void update(float dt) override
    {
        using namespace asw::input;

        Scene::update(dt);

        // Toggle pause
        if (is_action_pressed("pause")) {
            paused = !paused;
        }

        // Skip updates when paused
        if (paused) {
            // TO menu
            if (is_action_pressed("back")) {
                manager.set_next_scene(GameState::Menu);
            }
            return;
        }

        // Ticker for character movement and robot movement
        time_acc += dt;
        move_acc += dt;
        if (time_acc >= ROBOT_MOVE_INTERVAL) {
            time_acc -= ROBOT_MOVE_INTERVAL;
            update_robots();
        }

        // Use broom
        broom_active = is_action_down("interact") && has_broom;

        // Character movement
        character_move();

        // Die
        if (tilemap.atPixel(position) == TileType::Robot) {
            lives -= 1;
            position.x = 40;
            position.y = 40;
            asw::sound::play(oof);
        }

        // Restart Map
        if (is_action_pressed("restart")) {
            changeMap();
        }

        // Finish Level
        if (level_complete) {
            asw::sound::play(winsound);
            level += 1;
            lives += 1;
            changeMap();
        }

        // Die
        if (lives <= 0) {
            manager.set_next_scene(GameState::Menu);
        }
    }

    void draw() override
    {
        using namespace asw::draw;

        // Draw background
        tilemap.renderBackground();

        // Draws Tiles
        for (int t = 0; t < TileMap::HEIGHT; t++) {
            for (int i = 0; i < TileMap::WIDTH; i++) {
                if (position.x / 40 == i && position.y / 40 == t) {
                    draw_character();
                }

                tilemap.render({ i, t });
            }
        }

        // Draws Stats
        rect_fill(asw::Quadf(0, 0, 1280, 20), asw::Color(0, 0, 0, 200));

        text(font, std::format("Score: {}", score), asw::Vec2f(0, 0), palette::white);
        text(font, std::format("Lives: {}", lives), asw::Vec2f(100, 0), palette::white);
        text(font, tilemap.getLevelText(), asw::Vec2f(640, 0), palette::white,
            asw::TextJustify::Center);

        // Robot progress meter
        {
            const float meterWidth = 150.0F;
            const float meterHeight = 12.0F;
            const float meterX = 1280.0F - meterWidth - 5.0F;
            const float meterY = 4.0F;

            const float progress = tilemap.getProgress();

            // Background
            rect_fill(asw::Quadf(meterX, meterY, meterWidth, meterHeight), palette::dark_gray);

            // Filled portion (red when robots remain, green when cleared)
            const auto barColor = tilemap.getCompleted() ? palette::green : palette::red;

            rect_fill(asw::Quadf(meterX, meterY, meterWidth * progress, meterHeight), barColor);

            // Border
            rect(asw::Quadf(meterX, meterY, meterWidth, meterHeight), palette::white);

            // Label
            text(font,
                std::format("Robots: {}/{}", tilemap.getRobotsCaptured(), tilemap.getRobotsTotal()),
                asw::Vec2f(meterX - 5.0F, 0), palette::white, asw::TextJustify::Right);
        }

        // Pause Game
        if (paused) {
            rect_fill(asw::Quadf(300, 300, 680, 360), palette::very_dark_green);
            text(font_pause, "Paused press ESC to resume. Press M to go to the Menu.",
                asw::Vec2f(640, 480), palette::white, asw::TextJustify::Center);
        }
    }

    void draw_character()
    {
        using namespace asw::draw;
        using namespace asw::input;

        const auto float_pos
            = asw::Vec2f(static_cast<float>(position.x), static_cast<float>(position.y));

        // Draws Character
        if (rotation == 0) {
            sprite(character[0], float_pos);
        } else if (rotation == 128) {
            sprite(character[1], float_pos);
        } else if (rotation == 64) {
            sprite(character[2], float_pos);
        } else if (rotation == 192) {
            sprite(character[3], float_pos);
        }

        // Draws broom if needed
        if (broom_active) {
            rotate_sprite(broom, float_pos + asw::Vec2f(10, 10), rotation);
        }
    }

private:
    // Controls Character Movements
    void character_move()
    {
        using namespace asw::input;

        if (is_action_down("up")) {
            rotation = 128;
            move_towards({ 0, -1 });
        }
        if (is_action_down("down")) {
            rotation = 0;
            move_towards({ 0, 1 });
        }
        if (is_action_down("left")) {
            rotation = 64;
            move_towards({ -1, 0 });
        }
        if (is_action_down("right")) {
            rotation = 192;
            move_towards({ 1, 0 });
        }
    }

    void move_towards(const asw::Vec2i& target)
    {
        using namespace asw::input;

        if (move_acc < CHARACTER_MOVE_INTERVAL) {
            return;
        }

        move_acc = 0;

        auto player_pos = asw::Vec2i(position.x / 40, position.y / 40);
        auto tile_pos = player_pos + target;

        auto tile = tilemap.at(tile_pos);

        // Allow walking into
        if (tile == TileType::Empty || tile == TileType::WallWalkable || tile == TileType::Robot) {
            position += target * 40;
            return;
        }

        if (tile == TileType::Wall) {
            asw::sound::play(hitwall);
            return;
        }

        if (tile == TileType::JanitorRoom && !has_broom) {
            asw::sound::play(door);
            has_broom = true;
            tilemap.setValue(tile_pos, TileType::JanitorRoomOpen);
            return;
        }

        if (tile == TileType::JanitorRoomOpen && tilemap.getCompleted()) {
            level_complete = true;
            return;
        }

        // Check for box pushing or broom sweeping
        auto tile_next_pos = player_pos + target * 2;
        auto next_tile = tilemap.at(tile_next_pos);

        if (tile == TileType::Box && next_tile == TileType::Empty) {
            asw::sound::play(boxslide);
            tilemap.setValue(tile_pos, TileType::Empty);
            tilemap.setValue(tile_next_pos, TileType::Box);
            position += target * 40;
            return;
        }

        if (tile == TileType::Box && next_tile != TileType::Empty) {
            if (next_tile == TileType::Robot) {
                asw::sound::play(explode);
                tilemap.setValue(tile_next_pos, TileType::Scrap);
                score += 100;
            } else {
                asw::sound::play(boxhitwall);
            }

            return;
        }

        if (tile == TileType::Scrap && next_tile == TileType::Empty && broom_active) {
            tilemap.setValue(tile_pos, TileType::Empty);
            tilemap.setValue(tile_next_pos, TileType::Scrap);
            asw::sound::play(sweep);
            return;
        }

        if (tile == TileType::Scrap && next_tile == TileType::GarbageCan && broom_active) {
            tilemap.setValue(tile_pos, TileType::Empty);
            score += 50;
            tilemap.incrementRobotsCaptured();
            asw::sound::play(sweep);
            asw::sound::play(trash);
            return;
        }
    }

    void update_robots()
    {
        const auto player_pos = position / 40;

        // Collect all robot positions first to avoid double-processing
        std::vector<asw::Vec2i> robots;

        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                if (tilemap.at({ i, t }) == TileType::Robot) {
                    robots.emplace_back(i, t);
                }
            }
        }

        for (auto const& r : robots) {
            // Robot may have been destroyed since we collected positions
            if (tilemap.at(r) != TileType::Robot) {
                continue;
            }

            // Build list of valid moves
            const std::array<asw::Vec2i, 4> moves
                = { { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } } };
            std::vector<asw::Vec2i> valid;

            for (const auto& m : moves) {
                const auto next_pos = r + m;
                if (next_pos.x < 0 || next_pos.x >= TileMap::WIDTH || next_pos.y < 0
                    || next_pos.y >= TileMap::HEIGHT) {
                    continue;
                }

                if (tilemap.at(next_pos) == TileType::Empty) {
                    valid.push_back(m);
                }
            }

            if (valid.empty()) {
                continue;
            }

            // 50% chance to move toward the player, 50% random
            const int random_index = asw::random::between(0, valid.size() - 1);
            asw::Vec2i chosen = valid[random_index];

            if (asw::random::chance()) {
                auto bestDist = player_pos.distance(r + chosen);

                for (auto const& v : valid) {
                    auto dist = player_pos.distance(r + v);
                    if (dist < bestDist) {
                        bestDist = dist;
                        chosen = v;
                    }
                }
            }

            const auto next_pos = r + chosen;
            tilemap.setValue(next_pos, TileType::Robot);
            tilemap.setValue(r, TileType::Empty);
        }
    }

    // Change tiles
    void changeMap()
    {
        level_complete = false;
        has_broom = false;
        position = { 40, 40 };

        if (!tilemap.load(std::format("assets/levels/level{}.json", level))) {
            manager.set_next_scene(GameState::Win);
        }
    }

    // Game State
    int lives;
    bool won;
    bool level_complete;
    bool paused;
    float time_acc;
    float move_acc;

    // Player
    asw::Vec2i position;
    int rotation;
    bool has_broom;
    bool broom_active;
    std::array<asw::Texture, 4> character;
    asw::Texture broom;

    // Sounds
    asw::Music song;
    asw::Sample hitwall;
    asw::Sample boxhitwall;
    asw::Sample boxslide;
    asw::Sample explode;
    asw::Sample sweep;
    asw::Sample trash;
    asw::Sample oof;
    asw::Sample winsound;
    asw::Sample door;

    // Font
    asw::Font font;
    asw::Font font_pause;

    // Tilemap
    TileMap tilemap;
};