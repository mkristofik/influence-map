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
#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include "sdl_utils.h"
#include <memory>

// Wrapper around SDL_Window and SDL_Renderer.
class SdlWindow
{
public:
    SdlWindow(int width, int height, const char *caption);

    void clear();
    void draw();

    SDL_Window * get();
    SDL_Renderer * getRenderer();

    void drawRect(const SDL_Rect &rect, const SDL_Color &color);
    void fillRect(const SDL_Rect &rect, const SDL_Color &color);
    void drawLine(int x1, int y1, int x2, int y2, const SDL_Color &color);

    // Create a blank surface with the same pixel format as the window.
    SdlSurface createBlankSurface(int width, int height) const;
    SdlSurface createBlankSurface() const;  // same size as window

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};

#endif
