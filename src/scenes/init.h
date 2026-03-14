
#pragma once

#include <asw/asw.h>

#include "./scenes.h"

class InitScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        using namespace asw::input;

        asw::display::set_title("Mazes");
        asw::display::set_icon("assets/mazes.ico");

        // Bind global actions
        bind_action("right", KeyBinding { Key::Right });
        bind_action("right", KeyBinding { Key::D });
        bind_action("right", ControllerButtonBinding { ControllerButton::DPadRight, 0 });
        bind_action("right", ControllerAxisBinding { ControllerAxis::LeftX, 0, 0.1F, true });

        bind_action("left", KeyBinding { Key::Left });
        bind_action("left", KeyBinding { Key::A });
        bind_action("left", ControllerButtonBinding { ControllerButton::DPadLeft, 0 });
        bind_action("left", ControllerAxisBinding { ControllerAxis::LeftX, 0, -0.1F, true });

        bind_action("up", KeyBinding { Key::Up });
        bind_action("up", KeyBinding { Key::W });
        bind_action("up", ControllerButtonBinding { ControllerButton::DPadUp, 0 });
        // bind_action("up", ControllerAxisBinding { ControllerAxis::LeftY, 0, -0.1F, false });

        bind_action("down", KeyBinding { Key::Down });
        bind_action("down", KeyBinding { Key::S });
        bind_action("down", ControllerButtonBinding { ControllerButton::DPadDown, 0 });
        // bind_action("down", ControllerAxisBinding { ControllerAxis::LeftY, 0, 0.1F, true });

        bind_action("interact", KeyBinding { Key::Return });
        bind_action("interact", KeyBinding { Key::Space });
        bind_action("interact", ControllerButtonBinding { ControllerButton::A, 0 });

        bind_action("back", KeyBinding { Key::Escape });
        bind_action("back", ControllerButtonBinding { ControllerButton::Back, 0 });

        bind_action("pause", KeyBinding { Key::P });
        bind_action("pause", ControllerButtonBinding { ControllerButton::Start, 0 });
    }

    void update(float dt) override
    {
        manager.set_next_scene(GameState::Intro);
    }
};