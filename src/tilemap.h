#pragma once

#include <array>
#include <string>

#include "tile.h"

struct TileRenderConfig {
    int tile_size { 40 };
    int render_size { 60 };
    int offset_x { 0 };
    int offset_y { 0 };
};

class TileRenderer {

public:
    TileRenderer()
    {
        robot = asw::assets::loadTexture("assets/images/blocks/robot.png");
        box = asw::assets::loadTexture("assets/images/blocks/box.png");
        scrap = asw::assets::loadTexture("assets/images/blocks/scrap.png");
        wall = asw::assets::loadTexture("assets/images/blocks/wall.png");
        garbagecan = asw::assets::loadTexture("assets/images/blocks/garbagecan.png");
        janitorroom = asw::assets::loadTexture("assets/images/blocks/janitor_room.png");
        wall2 = asw::assets::loadTexture("assets/images/blocks/wall2.png");
        janitorroomopen = asw::assets::loadTexture("assets/images/blocks/janitor_room_open.png");
    }

    void renderTile(const TileType& tile, int x, int y, const TileRenderConfig& config) const
    {
        auto texture = getTileTexture(tile);

        if (texture) {
            const auto position = asw::Quad<float>(x * config.tile_size + config.offset_x,
                y * config.tile_size + config.offset_y, config.render_size, config.render_size);
            asw::draw::stretchSprite(texture, position);
        }
    }

private:
    asw::Texture getTileTexture(const TileType& type) const
    {
        switch (type) {
        case TileType::Robot:
            return robot;
        case TileType::Box:
            return box;
        case TileType::Scrap:
            return scrap;
        case TileType::Wall:
            return wall;
        case TileType::GarbageCan:
            return garbagecan;
        case TileType::JanitorRoom:
            return janitorroom;
        case TileType::WallWalkable:
            return wall2;
        case TileType::JanitorRoomOpen:
            return janitorroomopen;
        default:
            return nullptr;
        }
    }

    asw::Texture robot;
    asw::Texture box;
    asw::Texture scrap;
    asw::Texture wall;
    asw::Texture garbagecan;
    asw::Texture janitorroom;
    asw::Texture wall2;
    asw::Texture janitorroomopen;
};

class TileMap {
public:
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 24;

    // Grid coordinate access
    TileType at(const asw::Vec2<int>& pos) const;

    // Pixel-to-grid coordinate access
    TileType atPixel(const asw::Vec2<int>& pos) const;

    // Coordinate checking helpers
    TileType getValue(const asw::Vec2<int>& pos) const;
    void setValue(const asw::Vec2<int>& pos, TileType type);

    // Map file loading
    bool load(const std::string& path);

    // Draw at position
    void render(const asw::Vec2<int>& pos) const;
    void renderBackground() const;

    // Set render config
    void setRenderConfig(const TileRenderConfig& config)
    {
        render_config = config;
    }

    // Get level text
    const std::string& getLevelText() const
    {
        return level_text;
    }

    int getRobotsCaptured() const
    {
        return robots_captured;
    }

    int getRobotsTotal() const
    {
        return robots_total;
    }

    void incrementRobotsCaptured()
    {
        if (robots_captured < robots_total) {
            robots_captured++;
        }
    }

    float getProgress() const
    {
        return robots_total > 0 ? static_cast<float>(robots_captured) / robots_total : 1.0F;
    }

    bool getCompleted() const
    {
        return robots_captured >= robots_total;
    }

private:
    int robots_captured { 0 };
    int robots_total { 0 };

    std::string level_text;
    asw::Color background;

    TileRenderConfig render_config;

    std::array<std::array<TileType, HEIGHT>, WIDTH> tiles {};
    TileRenderer tile_renderer;
};
