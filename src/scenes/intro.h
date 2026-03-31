
#pragma once

#include <asw/asw.h>

#include "./scenes.h"

class IntroScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    static constexpr float FADE_IN = 0.5F;
    static constexpr float HOLD = 1.0F;
    static constexpr float FADE_OUT = 0.5F;
    static constexpr float FRAME_DURATION = FADE_IN + HOLD + FADE_OUT;
    static constexpr int NUM_FRAMES = 2;
    static constexpr float INTRO_DURATION = FRAME_DURATION * NUM_FRAMES;

    void init() override
    {
        intro_ = asw::assets::load_texture("assets/images/intro.png");
        splash_ = asw::assets::load_texture("assets/images/splash.png");
        background_ = asw::assets::load_texture("assets/images/background.png");
        time_acc_ = 0.0F;
    }

    void update(float dt) override
    {
        using namespace asw::input;
        Scene::update(dt);
        time_acc_ += dt;

        if (asw::input::keyboard.any_pressed || time_acc_ >= INTRO_DURATION) {
            manager.set_next_scene(GameState::Menu);
        }
    }

    void draw() override
    {
        asw::display::set_blend_mode(asw::BlendMode::Blend);

        // Determine which frame we're on and the local time within it
        const auto frame = static_cast<int>(time_acc_ / FRAME_DURATION);
        const float local = time_acc_ - (static_cast<float>(frame) * FRAME_DURATION);

        // Show the appropriate image for this frame
        if (frame == 0) {
            asw::draw::sprite(background_, asw::Vec2f(0, 0));
            asw::draw::sprite(intro_, asw::Vec2f(0, 0));
        } else {
            asw::draw::sprite(splash_, asw::Vec2f(0, 0));
        }

        // Compute alpha: fade in → hold → fade out
        // Fade in -> hold → fade out
        float alpha = 0.0F;
        if (local < FADE_IN) {
            alpha = 1.0F - asw::easing::ease_in_quad(local / FADE_IN);
        } else if (local < FADE_IN + HOLD) {
            alpha = 0.0F;
        } else {
            alpha = asw::easing::ease_out_quad((local - FADE_IN - HOLD) / FADE_OUT);
        }

        // Draw rect
        const auto color = asw::Color::from_float(0, 0, 0, alpha);
        asw::draw::rect_fill({ 0, 0, 1280, 960 }, color);
    }

private:
    asw::Texture intro_;
    asw::Texture splash_;
    asw::Texture background_;

    float time_acc_ { 0.0F };
};