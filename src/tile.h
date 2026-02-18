#pragma once

#include <asw/asw.h>

// Tile types
enum class TileType {
    Empty = 0,
    Wall = 1,
    WallWalkable = 2,
    JanitorRoom = 3,
    JanitorRoomOpen = 4,
    Box = 11,
    Scrap = 21,
    GarbageCan = 22,
    Robot = 31
};

// Convert integer value to tile type
inline TileType tileTypeFromValue(int value)
{
    switch (value) {
    case 0:
        return TileType::Empty;
    case 1:
        return TileType::Wall;
    case 2:
        return TileType::WallWalkable;
    case 3:
        return TileType::JanitorRoom;
    case 4:
        return TileType::JanitorRoomOpen;
    case 11:
        return TileType::Box;
    case 21:
        return TileType::Scrap;
    case 22:
        return TileType::GarbageCan;
    case 31:
        return TileType::Robot;
    default:
        asw::log::warn("Warning: Invalid tile value " + std::to_string(value));
        return TileType::Empty;
    }
}
