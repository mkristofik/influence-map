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

#include "SdlWindow.h"
#include "sdl_utils.h"
#include <cstdlib>
#include <iostream>

int real_main(int argc, char **argv)
{
    SdlWindow win(1280, 768, "Influence Map Test");
    win.clear();
    win.draw();

    bool isDone = false;
    SDL_Event event;
    while (!isDone) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isDone = true;
                    break;
            }
        }

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
