#pragma once

#include <array>
#include <asw/asw.h>
#include <cmath>
#include <ctime>
#include <format>
#include <sstream>
#include <string>

#include "../button.h"
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
        position = asw::Vec2<int>(40, 40);
        rotation = 0;
        has_broom = false;
        broom_active = false;
        won = false;
        level_complete = false;

        font = loadFont("assets/fonts/dosis.ttf", 16);
        font_pause = loadFont("assets/fonts/jersey-10.ttf", 32);

        // Tiles
        character[0] = loadTexture("assets/images/blocks/character_down.png");
        character[1] = loadTexture("assets/images/blocks/character_up.png");
        character[2] = loadTexture("assets/images/blocks/character_left.png");
        character[3] = loadTexture("assets/images/blocks/character_right.png");
        broom = loadTexture("assets/images/blocks/broom.png");

        // Sounds and music
        sweep = loadSample("assets/sfx/sweep.wav");
        explode = loadSample("assets/sfx/explode.wav");
        trash = loadSample("assets/sfx/trash.wav");
        oof = loadSample("assets/sfx/oof.wav");
        winsound = loadSample("assets/sfx/winsound.wav");
        door = loadSample("assets/sfx/door.wav");
        hitwall = loadSample("assets/sfx/hitwall.wav");
        boxhitwall = loadSample("assets/sfx/boxhitwall.wav");
        boxslide = loadSample("assets/sfx/boxslide.wav");

        song = loadMusic("assets/sfx/music.ogg");

        score = 0;

        // Render config
        tilemap.load(std::format("assets/levels/level{}.json", level));
        tilemap.setRenderConfig(
            { .tile_size = 40, .render_size = 60, .offset_x = 0, .offset_y = 0 });

        // Background Music
        fadeInMusic(song, 1.0F, 1000);
    }

    void update(float dt) override
    {
        using namespace asw::input;

        Scene::update(dt);

        // Toggle pause
        if (getKeyDown(Key::Escape)) {
            paused = !paused;
        }

        // Skip updates when paused
        if (paused) {
            // TO menu
            if (getKeyDown(Key::M)) {
                sceneManager.setNextScene(GameState::Menu);
            }
            return;
        }

        // Ticker for character movement and robot movement
        time_acc += dt;
        move_acc += dt;
        if (time_acc >= ROBOT_MOVE_INTERVAL) {
            time_acc -= ROBOT_MOVE_INTERVAL;
            updateRobots();
        }

        // Use broom
        broom_active = getKey(Key::Space) && has_broom;

        // Character movement
        characterMove();

        // Die
        if (tilemap.atPixel(position) == TileType::Robot) {
            lives -= 1;
            position.x = 40;
            position.y = 40;
            asw::sound::play(oof);
        }

        // Restart Map
        if (getKeyDown(Key::R)) {
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
            sceneManager.setNextScene(GameState::Menu);
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
                    drawCharacter();
                }

                tilemap.render({ i, t });
            }
        }

        // Draws Stats
        rectFill(asw::Quad<float>(0, 0, 1280, 20), asw::Color(0, 0, 0, 200));

        text(font, std::format("Score: {}", score), asw::Vec2<float>(0, 0), palette::white);
        text(font, std::format("Lives: {}", lives), asw::Vec2<float>(100, 0), palette::white);
        text(font, tilemap.getLevelText(), asw::Vec2<float>(640, 0), palette::white,
            asw::TextJustify::CENTER);

        // Robot progress meter
        {
            const float meterWidth = 150.0F;
            const float meterHeight = 12.0F;
            const float meterX = 1280.0F - meterWidth - 5.0F;
            const float meterY = 4.0F;

            const float progress = tilemap.getProgress();

            // Background
            rectFill(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight), palette::dark_gray);

            // Filled portion (red when robots remain, green when cleared)
            const auto barColor = tilemap.getCompleted() ? palette::green : palette::red;

            rectFill(
                asw::Quad<float>(meterX, meterY, meterWidth * progress, meterHeight), barColor);

            // Border
            rect(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight), palette::white);

            // Label
            text(font,
                std::format("Robots: {}/{}", tilemap.getRobotsCaptured(), tilemap.getRobotsTotal()),
                asw::Vec2<float>(meterX - 5.0F, 0), palette::white, asw::TextJustify::RIGHT);
        }

        // Pause Game
        if (paused) {
            rectFill(asw::Quad<float>(300, 300, 680, 360), palette::very_dark_green);
            text(font_pause, "Paused press ESC to resume. Press M to go to the Menu.",
                asw::Vec2<float>(640, 480), palette::white, asw::TextJustify::CENTER);
        }
    }

    void drawCharacter()
    {
        using namespace asw::draw;
        using namespace asw::input;

        const auto float_pos
            = asw::Vec2<float>(static_cast<float>(position.x), static_cast<float>(position.y));

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
            rotateSprite(broom, float_pos + asw::Vec2<float>(10, 10), rotation);
        }
    }

private:
    // Controls Character Movements
    void characterMove()
    {
        using namespace asw::input;

        if (getKey(Key::Up) || getKey(Key::W)) {
            rotation = 128;
            moveTowards({ 0, -1 });
        } else if (getKey(Key::Down) || getKey(Key::S)) {
            rotation = 0;
            moveTowards({ 0, 1 });
        } else if (getKey(Key::Left) || getKey(Key::A)) {
            rotation = 64;
            moveTowards({ -1, 0 });
        } else if (getKey(Key::Right) || getKey(Key::D)) {
            rotation = 192;
            moveTowards({ 1, 0 });
        }
    }

    void moveTowards(const asw::Vec2<int>& target)
    {
        using namespace asw::input;

        if (move_acc < CHARACTER_MOVE_INTERVAL) {
            return;
        }

        move_acc = 0;

        auto player_pos = asw::Vec2<int>(position.x / 40, position.y / 40);
        auto tile_pos = player_pos + target;
        auto tile_next_pos = player_pos + target * 2;

        auto tile = tilemap.at(tile_pos);
        auto next_tile = tilemap.at(tile_next_pos);

        // Allow walking into
        if (tile == TileType::Empty || tile == TileType::WallWalkable || tile == TileType::Robot) {
            position += target * 40;
            return;
        }

        if (tile == TileType::Wall) {
            asw::sound::play(hitwall);
            return;
        }

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
    }

    void updateRobots()
    {
        const auto player_pos = position / 40;

        // Collect all robot positions first to avoid double-processing
        std::vector<asw::Vec2<int>> robots;

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
            std::array<asw::Vec2<int>, 4> moves = { { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } } };
            std::vector<asw::Vec2<int>> valid;

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
            asw::Vec2<int> chosen = valid[random_index];

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
            sceneManager.setNextScene(GameState::Win);
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
    asw::Vec2<int> position;
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