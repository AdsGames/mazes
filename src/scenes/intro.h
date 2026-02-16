
#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "./scenes.h"

class IntroScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    static constexpr float FADE_IN = 500.0F;
    static constexpr float HOLD = 1000.0F;
    static constexpr float FADE_OUT = 500.0F;
    static constexpr float FRAME_DURATION = FADE_IN + HOLD + FADE_OUT;
    static constexpr int NUM_FRAMES = 2;
    static constexpr float INTRO_DURATION = FRAME_DURATION * NUM_FRAMES;

    void init() override
    {
        intro = asw::assets::loadTexture("assets/images/intro.png");
        splash = asw::assets::loadTexture("assets/images/splash.png");
        background = asw::assets::loadTexture("assets/images/background.png");
        time_acc = 0.0F;
    }

    void update(float deltaTime) override
    {
        using namespace asw::input;
        Scene::update(deltaTime);
        time_acc += deltaTime;

        if (asw::input::keyboard.anyPressed || time_acc >= INTRO_DURATION) {
            sceneManager.setNextScene(GameState::Menu);
        }
    }

    void draw() override
    {
        asw::display::setBlendMode(asw::BlendMode::BLEND);

        // Determine which frame we're on and the local time within it
        const auto frame = static_cast<int>(time_acc / FRAME_DURATION);
        const float local = time_acc - static_cast<float>(frame) * FRAME_DURATION;

        // Show the appropriate image for this frame
        if (frame == 0) {
            asw::draw::sprite(background, asw::Vec2<float>(0, 0));
            asw::draw::sprite(intro, asw::Vec2<float>(0, 0));
        } else {
            asw::draw::sprite(splash, asw::Vec2<float>(0, 0));
        }

        // Compute alpha: fade in → hold → fade out
        // Fade in -> hold → fade out
        float alpha = 0.0F;
        if (local < FADE_IN) {
            alpha = 1.0F - asw::easing::easeInQuad(local / FADE_IN);
        } else if (local < FADE_IN + HOLD) {
            alpha = 0.0F;
        } else {
            alpha = asw::easing::easeOutQuad((local - FADE_IN - HOLD) / FADE_OUT);
        }

        // Draw rect
        const auto color = asw::Color::fromFloat(0, 0, 0, alpha);
        asw::draw::rectFill({ 0, 0, 1280, 960 }, color);
    }

private:
    asw::Texture intro;
    asw::Texture splash;
    asw::Texture background;

    float time_acc;
};