#pragma once

#include <array>
#include <asw/asw.h>
#include <cmath>
#include <ctime>
#include <format>
#include <sstream>
#include <string>

#include "../button.h"
#include "../id.h"

#include "../globals.h"
#include "./scenes.h"

class GameScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    // Constants
    static constexpr int ROBOT_MOVE_INTERVAL = 2080 / 4; // 4 moves per beat
    static constexpr int CHARACTER_MOVE_INTERVAL = 100;
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

        if (perspective == 0) {
            character[0] = loadTexture("assets/images/blocks/3d/character_down.png");
            character[1] = loadTexture("assets/images/blocks/3d/character_up.png");
            character[2] = loadTexture("assets/images/blocks/3d/character_left.png");
            character[3] = loadTexture("assets/images/blocks/3d/character_right.png");
            broom = loadTexture("assets/images/blocks/3d/broom.png");
        }

        if (perspective == 1) {
            character[0] = loadTexture("assets/images/blocks/2d/character.png");
            character[1] = loadTexture("assets/images/blocks/2d/character.png");
            character[2] = loadTexture("assets/images/blocks/2d/character.png");
            character[3] = loadTexture("assets/images/blocks/2d/character.png");
            broom = loadTexture("assets/images/blocks/2d/broom.png");
        }

        background = loadTexture("assets/images/background.png");

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

        level = 1;
        score = 0;

        // Background Music
        fadeInMusic(song, 1.0F, 1000);
    }

    void update(float deltaTime) override
    {
        using namespace asw::input;

        Scene::update(deltaTime);

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
        time_acc += deltaTime;
        move_acc += deltaTime;
        if (time_acc >= ROBOT_MOVE_INTERVAL) {
            time_acc -= ROBOT_MOVE_INTERVAL;
            updateRobots();
        }

        // Use broom
        broom_active = getKey(Key::Space) && has_broom;

        // Character movement
        characterMove();

        // Die
        if (tilemap.atPixel(position.x, position.y).value == V_ROBOT) {
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
        sprite(background, asw::Vec2<float>(0, 0));

        // Draws Tiles
        for (int t = 0; t < TileMap::HEIGHT; t++) {
            for (int i = 0; i < TileMap::WIDTH; i++) {
                if (position.x / 40 == i && position.y / 40 == t) {
                    drawCharacter();
                }

                if (perspective == 0) {
                    stretchSprite(tilemap.at(i, t).image, asw::Quad<float>(i * 40, t * 40, 60, 60));
                } else {
                    stretchSprite(tilemap.at(i, t).image, asw::Quad<float>(i * 40, t * 40, 40, 40));
                }
            }
        }

        // Draws Stats
        rectFill(asw::Quad<float>(0, 0, 1280, 20), asw::util::makeColor(0, 0, 0, 200));

        text(font, std::format("Score: {}", score), asw::Vec2<float>(0, 0),
            asw::util::makeColor(255, 255, 255));

        text(font, std::format("Lives: {}", lives), asw::Vec2<float>(100, 0),
            asw::util::makeColor(255, 255, 255));

        textCenter(font, tilemap.level_text, asw::Vec2<float>(640, 0),
            asw::util::makeColor(255, 255, 255));

        // Robot progress meter
        {
            const float meterWidth = 150.0F;
            const float meterHeight = 12.0F;
            const float meterX = 1280.0F - meterWidth - 5.0F;
            const float meterY = 4.0F;

            const float progress = tilemap.robots_total > 0
                ? static_cast<float>(tilemap.robots_captured)
                    / static_cast<float>(tilemap.robots_total)
                : 1.0F;

            // Background
            rectFill(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight),
                asw::util::makeColor(60, 60, 60));

            // Filled portion (red when robots remain, green when cleared)
            const auto barColor = tilemap.robots_captured < tilemap.robots_total
                ? asw::util::makeColor(200, 50, 50)
                : asw::util::makeColor(50, 200, 50);

            rectFill(
                asw::Quad<float>(meterX, meterY, meterWidth * progress, meterHeight), barColor);

            // Border
            rect(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight),
                asw::util::makeColor(255, 255, 255));

            // Label
            textRight(font,
                std::format("Robots: {}/{}", tilemap.robots_captured, tilemap.robots_total),
                asw::Vec2<float>(meterX - 5.0F, 0), asw::util::makeColor(255, 255, 255));
        }

        // Pause Game
        if (paused) {
            rectFill(asw::Quad<float>(300, 300, 680, 360), asw::util::makeColor(0, 0, 0, 200));
            textCenter(font, "Paused press ESC to resume. Press M to go to the Menu.",
                asw::Vec2<float>(640, 480), asw::util::makeColor(255, 255, 255));
        }
    }

    void drawCharacter()
    {
        using namespace asw::draw;
        using namespace asw::input;

        const auto float_pos
            = asw::Vec2<float>(static_cast<float>(position.x), static_cast<float>(position.y));

        // Draws Character
        if (perspective == 0) {
            if (rotation == 0) {
                sprite(character[0], float_pos);
            } else if (rotation == 128) {
                sprite(character[1], float_pos);
            } else if (rotation == 64) {
                sprite(character[2], float_pos);
            } else if (rotation == 192) {
                sprite(character[3], float_pos);
            }
        } else {
            sprite(character[0], float_pos);
        }

        // Draws broom if needed
        if (broom_active) {
            if (perspective == 0) {
                rotateSprite(broom, float_pos + asw::Vec2<float>(10, 10), rotation);
            } else {
                sprite(broom, float_pos);
            }
        }
    }

private:
    // Controls Character Movements
    void characterMove()
    {
        using namespace asw::input;

        if (getKey(Key::Up) || getKey(Key::W)) {
            rotation = 128;
            moveTowards(0, -1);
        } else if (getKey(Key::Down) || getKey(Key::S)) {
            rotation = 0;
            moveTowards(0, 1);
        } else if (getKey(Key::Left) || getKey(Key::A)) {
            rotation = 64;
            moveTowards(-1, 0);
        } else if (getKey(Key::Right) || getKey(Key::D)) {
            rotation = 192;
            moveTowards(1, 0);
        }
    }

    void moveTowards(int dx, int dy)
    {
        using namespace asw::input;

        if (move_acc < CHARACTER_MOVE_INTERVAL) {
            return;
        }

        move_acc = 0;

        auto& tile = tilemap.at(position.x / 40 + dx, position.y / 40 + dy);
        auto& next_tile = tilemap.at(position.x / 40 + dx * 2, position.y / 40 + dy * 2);

        // Allow walking into
        if (tile.value == V_EMPTY || tile.value == V_WALL2 || tile.value == V_ROBOT) {
            position.x += dx * 40;
            position.y += dy * 40;
            return;
        }

        if (tile.value == V_WALL) {
            asw::sound::play(hitwall);
            return;
        }

        if (tile.value == V_BOX && next_tile.value == V_EMPTY) {
            asw::sound::play(boxslide);

            tilemap.setValue(tile, V_EMPTY);
            tilemap.setValue(next_tile, V_BOX);
            position.x += dx * 40;
            position.y += dy * 40;
            return;
        }

        if (tile.value == V_BOX && next_tile.value != 0) {
            if (next_tile.value == V_ROBOT) {
                asw::sound::play(explode);
                tilemap.setValue(next_tile, V_SCRAP);
                score += 100;
            } else {
                asw::sound::play(boxhitwall);
            }

            return;
        }

        if (tile.value == V_SCRAP && next_tile.value == V_EMPTY && broom_active) {
            tilemap.setValue(tile, V_EMPTY);
            tilemap.setValue(next_tile, V_SCRAP);
            asw::sound::play(sweep);
            return;
        }

        if (tile.value == V_SCRAP && next_tile.value == V_GARBAGECAN && broom_active) {
            tilemap.setValue(tile, V_EMPTY);
            score += 50;
            if (tilemap.robots_captured < tilemap.robots_total) {
                tilemap.robots_captured += 1;
            }
            asw::sound::play(sweep);
            asw::sound::play(trash);
            return;
        }

        if (tile.value == V_JANITORROOM && !has_broom) {
            asw::sound::play(door);
            has_broom = true;
            tilemap.setValue(tile, V_JANITORROOMOPEN);
            return;
        }

        if (tile.value == V_JANITORROOMOPEN && tilemap.robots_captured == tilemap.robots_total) {
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
                if (tilemap.at(i, t).value == V_ROBOT) {
                    robots.push_back({ i, t });
                }
            }
        }

        for (auto const& r : robots) {
            // Robot may have been destroyed since we collected positions
            if (tilemap.at(r.x, r.y).value != V_ROBOT) {
                continue;
            }

            // Build list of valid moves
            std::array<asw::Vec2<int>, 4> moves = { { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } } };
            std::vector<asw::Vec2<int>> valid;

            for (const auto& m : moves) {
                int nx = r.x + m.x;
                int ny = r.y + m.y;
                if (nx < 0 || nx >= TileMap::WIDTH || ny < 0 || ny >= TileMap::HEIGHT) {
                    continue;
                }

                if (tilemap.at(nx, ny).value == V_EMPTY) {
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
                int bestDist = player_pos.distance(r + chosen);

                for (auto const& v : valid) {
                    int dist = player_pos.distance(r + v);
                    if (dist < bestDist) {
                        bestDist = dist;
                        chosen = v;
                    }
                }
            }

            tilemap.setValue(r.x + chosen.x, r.y + chosen.y, V_ROBOT);
            tilemap.setValue(r.x, r.y, V_EMPTY);
        }
    }

    // Change tiles
    void changeMap()
    {
        level_complete = false;
        has_broom = false;
        position = { 40, 40 };

        if (!tilemap.load(std::format("assets/levels/level{}.map", level))) {
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

    // Background
    asw::Texture background;

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
};