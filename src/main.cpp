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
    std::unique_ptr<SimpleMap> advMap;
    bool isDirty = true;
}

void handleKeyUp(const SDL_KeyboardEvent &event)
{
    const int rPlayer1 = advMap->getRegion(1);
    const int rPlayer2 = advMap->getRegion(2);

    switch (event.keysym.sym) {
        case SDLK_a:
            if (rPlayer1 > 0) {
                advMap->moveEntity(1, rPlayer1 - 1);
                isDirty = true;
            }
            break;
        case SDLK_d:
            if (rPlayer1 < 31) {
                advMap->moveEntity(1, rPlayer1 + 1);
                isDirty = true;
            }
            break;
        case SDLK_h:
            if (rPlayer2 > 0) {
                advMap->moveEntity(2, rPlayer2 - 1);
                isDirty = true;
            }
            break;
        case SDLK_l:
            if (rPlayer2 < 31) {
                advMap->moveEntity(2, rPlayer2 + 1);
                isDirty = true;
            }
            break;
    }
}

int real_main(int argc, char **argv)
{
    GameWindow win{winWidth, winHeight, "Influence Map Test"};

    advMap.reset(new SimpleMap{winWidth, winHeight, 2, win.getBlankMap()});
    advMap->addEntity(MapEntity{1, 1, 8, Team::BLUE});
    advMap->addEntity(MapEntity{2, 30, 8, Team::RED});

    auto img1 = applyTeamColor(sdlLoadImage("cavalier.png"), Team::BLUE);
    win.addEntity(1, advMap->pixelFromRegion(advMap->getRegion(1)), img1);
    auto img2 = applyTeamColor(sdlLoadImage("orc-grunt.png"), Team::RED);
    win.addEntity(2, advMap->pixelFromRegion(advMap->getRegion(2)), img2);

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
            win.updateMap(advMap->update());
            win.moveEntity(1, advMap->pixelFromRegion(advMap->getRegion(1)));
            win.moveEntity(2, advMap->pixelFromRegion(advMap->getRegion(2)));
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
