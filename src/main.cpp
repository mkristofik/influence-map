/*
    Copyright (C) 2014-2015 by Michael Kristofik <kristo605@gmail.com>
    Part of the influence-map project.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    or at your option any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.

    See the COPYING.txt file for more details.
*/
#include "GameWindow.h"
#include "SdlTextureStream.h"
#include "SdlWindow.h"
#include "SimpleMap.h"
#include "sdl_utils.h"
#include "team_color.h"
#include <cstdlib>
#include <iostream>
#include <memory>

namespace
{
    const int winWidth = 1280;
    const int winHeight = 768;
}


class Game
{
public:
    Game();
    void loadScenario();
    void update();

    void handleKeyUp(const SDL_KeyboardEvent &event);

private:
    bool isDirty_;
    GameWindow win_;
    SimpleMap advMap_;
};

Game::Game()
    : isDirty_{true},
    win_{winWidth, winHeight, "Influence Map Test"},
    advMap_{winWidth, winHeight, 2, win_.getBlankMap()}
{
}

void Game::loadScenario()
{
    advMap_.addEntity(MapEntity{1, 1, 8, Team::BLUE});
    advMap_.addEntity(MapEntity{2, 30, 8, Team::RED});
    advMap_.addEntity(MapEntity{3, 24, 0, Team::NONE});

    auto img1 = applyTeamColor(sdlLoadImage("cavalier.png"), Team::BLUE);
    win_.addEntity(1, advMap_.pixelFromRegion(advMap_.getRegion(1)), img1);
    auto img2 = applyTeamColor(sdlLoadImage("orc-grunt.png"), Team::RED);
    win_.addEntity(2, advMap_.pixelFromRegion(advMap_.getRegion(2)), img2);
    auto img3 = applyTeamColor(applyFlagColor(sdlLoadImage("flag.png")), Team::NONE);
    win_.addEntity(3, advMap_.pixelFromRegion(advMap_.getRegion(3)), img3);
}

void Game::update()
{
    if (!isDirty_) {
        return;
    }

    win_.updateMap(advMap_.update());
    win_.draw();
    isDirty_ = false;
}

void Game::handleKeyUp(const SDL_KeyboardEvent &event)
{
    auto rPlayer1 = advMap_.getRegion(1);
    auto rPlayer2 = advMap_.getRegion(2);

    switch (event.keysym.sym) {
        case SDLK_a:
            if (rPlayer1 > 0) {
                --rPlayer1;
                win_.moveEntity(1, advMap_.pixelFromRegion(rPlayer1));
                advMap_.moveEntity(1, rPlayer1);
                isDirty_ = true;
            }
            break;
        case SDLK_d:
            if (rPlayer1 < 31) {
                ++rPlayer1;
                win_.moveEntity(1, advMap_.pixelFromRegion(rPlayer1));
                advMap_.moveEntity(1, rPlayer1);
                isDirty_ = true;
            }
            break;
        case SDLK_h:
            if (rPlayer2 > 0) {
                --rPlayer2;
                win_.moveEntity(2, advMap_.pixelFromRegion(rPlayer2));
                advMap_.moveEntity(2, rPlayer2);
                isDirty_ = true;
            }
            break;
        case SDLK_l:
            if (rPlayer2 < 31) {
                ++rPlayer2;
                win_.moveEntity(2, advMap_.pixelFromRegion(rPlayer2));
                advMap_.moveEntity(2, rPlayer2);
                isDirty_ = true;
            }
            break;
    }
}


int real_main(int argc, char **argv)
{
    Game game;
    game.loadScenario();

    bool isDone = false;
    SDL_Event event;
    while (!isDone) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP:
                    game.handleKeyUp(event.key);
                    break;
                case SDL_QUIT:
                    isDone = true;
                    break;
            }
        }

        game.update();
        SDL_Delay(1);
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)  // two-arg form required by SDL
{
    try {
        sdlInit();
        return real_main(argc, argv);
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
