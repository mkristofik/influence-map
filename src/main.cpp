/*
    Copyright (C) 2014 by Michael Kristofik <kristo605@gmail.com>
    Part of the influence-map project.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    or at your option any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.

    See the COPYING.txt file for more details.
*/
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
    std::unique_ptr<SimpleMap> advMap;
    int rPlayer1 = 1;
    int rPlayer2 = 31;
    bool isDirty = true;
}

void handleKeyUp(const SDL_KeyboardEvent &event)
{
    advMap->setInfluence(rPlayer1, Team::BLUE, 0);
    advMap->setInfluence(rPlayer2, Team::RED, 0);

    switch (event.keysym.sym) {
        case SDLK_a:
            if (rPlayer1 > 0) {
                --rPlayer1;
                isDirty = true;
            }
            break;
        case SDLK_d:
            if (rPlayer1 < 31) {
                ++rPlayer1;
                isDirty = true;
            }
            break;
        case SDLK_h:
            if (rPlayer2 > 0) {
                --rPlayer2;
                isDirty = true;
            }
            break;
        case SDLK_l:
            if (rPlayer2 < 31) {
                ++rPlayer2;
                isDirty = true;
            }
            break;
    }

    advMap->setInfluence(rPlayer1, Team::BLUE, 8);
    advMap->setInfluence(rPlayer2, Team::RED, 8);
}

int real_main(int argc, char **argv)
{
    SdlWindow win{winWidth, winHeight, "Influence Map Test"};
    advMap.reset(new SimpleMap{winWidth, winHeight, 2, win});
    advMap->setInfluence(rPlayer1, Team::BLUE, 8);
    advMap->setInfluence(rPlayer2, Team::RED, 8);

    SdlTextureStream advMapImg{advMap->draw(), win};

    auto img1 = applyTeamColor(sdlLoadImage("cavalier.png"), Team::BLUE);
    auto player1 = SdlTexture{img1, win};
    auto img2 = applyTeamColor(sdlLoadImage("orc-grunt.png"), Team::RED);
    auto player2 = SdlTexture{img2, win};

    bool isDone = false;
    SDL_Event event;
    while (!isDone) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP:
                    handleKeyUp(event.key);
                    break;
                case SDL_QUIT:
                    isDone = true;
                    break;
            }
        }

        if (isDirty) {
            win.clear();
            advMapImg.update(advMap->draw());
            advMapImg.draw(0, 0);
            player1.draw(204, 60);
            player2.draw(1164, 636);
            win.draw();
            isDirty = false;
        }

        SDL_Delay(1);
    }

    advMap.reset();
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
