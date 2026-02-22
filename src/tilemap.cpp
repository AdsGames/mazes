#include "tilemap.h"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace asw::assets;

TileType TileMap::at(const asw::Vec2<int>& pos) const
{
    return tiles[pos.x][pos.y];
}

TileType TileMap::atPixel(const asw::Vec2<int>& pos) const
{
    return tiles[pos.x / render_config.tile_size][pos.y / render_config.tile_size];
}

TileType TileMap::getValue(const asw::Vec2<int>& pos) const
{
    return tiles[pos.x][pos.y];
}

void TileMap::setValue(const asw::Vec2<int>& pos, TileType type)
{
    tiles[pos.x][pos.y] = type;
}

bool TileMap::load(const std::string& path)
{
    std::ifstream file(path);
    if (file.fail()) {
        return false;
    }

    // Create buffer
    nlohmann::json doc = nlohmann::json::parse(file);

    // Check layers
    if (!doc.contains("layers")) {
        asw::log::warn("Error: No layers found in file {}", path);
        file.close();
        return false;
    }

    if (doc["layers"].size() != 1) {
        asw::log::warn("Error: Invalid number of layers in file {}", path);
        file.close();
        return false;
    }

    // Read properties if they exist
    level_text = "";
    robots_total = 0;
    robots_captured = 0;
    if (doc.contains("properties")) {
        for (auto& prop : doc["properties"]) {
            if (prop["name"] == "title") {
                level_text = prop["value"];
            }
            if (prop["name"] == "robot_target") {
                robots_total = prop["value"];
            }
        }
    }

    // Background color
    background = asw::Color(179, 185, 209); // Default to gray
    if (doc.contains("backgroundcolor")) {
        background = asw::Color::from_hex(doc["backgroundcolor"]);
    }

    // Load data into vector
    const std::vector<int> tile_layer = doc["layers"][0]["data"];

    // 3) Tile values (next 24 lines, each with 32 integers)
    int x = 0;
    int y = 0;
    for (auto value : tile_layer) {
        tiles[x][y] = tileTypeFromValue(value);
        x++;
        if (x >= WIDTH) {
            x = 0;
            y++;
        }
    }

    file.close();

    return true;
}

void TileMap::render(const asw::Vec2<int>& pos) const
{
    tile_renderer.renderTile(tiles[pos.x][pos.y], pos.x, pos.y, render_config);
}

void TileMap::renderBackground() const
{
    const auto pos = asw::Quad<float>(render_config.offset_x, render_config.offset_y,
        WIDTH * render_config.tile_size, HEIGHT * render_config.tile_size);

    asw::draw::rect_fill(pos, background);
}