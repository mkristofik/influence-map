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
#include "SdlWindow.h"
#include <iostream>
#include <stdexcept>

SdlWindow::SdlWindow(int width, int height, const char *caption)
    : window_{nullptr, SDL_DestroyWindow},
    renderer_{nullptr, SDL_DestroyRenderer}
{
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    if (SDL_CreateWindowAndRenderer(width, height, 0, &win, &ren) < 0) {
        std::cerr << "Error creating window:" << SDL_GetError();
        throw std::runtime_error("SdlWindow constructor failed.");
    }

    SDL_SetWindowTitle(win, caption);
    window_.reset(win);
    renderer_.reset(ren);
}

void SdlWindow::clear()
{
    if (SDL_RenderClear(renderer_.get()) < 0) {
        std::cerr << "Error clearing window: " << SDL_GetError();
    }
}

void SdlWindow::draw()
{
    SDL_RenderPresent(renderer_.get());
}

SDL_Window * SdlWindow::get()
{
    return window_.get();
}

SDL_Renderer * SdlWindow::getRenderer()
{
    return renderer_.get();
}

void SdlWindow::drawRect(const SDL_Rect &rect, const SDL_Color &color)
{
    auto ren = getRenderer();
    SdlDrawColor pen{ren, color.r, color.g, color.b};
    if (SDL_RenderDrawRect(ren, &rect) < 0) {
        std::cerr << "Warning, draw rect failed: " << SDL_GetError();
    }
}

void SdlWindow::fillRect(const SDL_Rect &rect, const SDL_Color &color)
{
    auto ren = getRenderer();
    SdlDrawColor pen{ren, color.r, color.g, color.b};
    if (SDL_RenderFillRect(ren, &rect) < 0) {
        std::cerr << "Warning, fill rect failed: " << SDL_GetError();
    }
}

void SdlWindow::drawLine(int x1, int y1, int x2, int y2, const SDL_Color &color)
{
    auto ren = getRenderer();
    SdlDrawColor pen{ren, color.r, color.g, color.b};
    if (SDL_RenderDrawLine(ren, x1, y1, x2, y2) < 0) {
        std::cerr << "Warning, draw line failed: " << SDL_GetError();
    }
}

SdlSurface SdlWindow::createBlankSurface(int width, int height) const
{
    auto winSurf = SDL_GetWindowSurface(window_.get());
    auto surf = SDL_CreateRGBSurface(0,
                                     width,
                                     height,
                                     winSurf->format->BitsPerPixel,
                                     winSurf->format->Rmask,
                                     winSurf->format->Gmask,
                                     winSurf->format->Bmask,
                                     winSurf->format->Amask);
    if (!surf) {
        std::cerr << "Error creating blank surface: " << SDL_GetError();
        return {};
    }

    return make_surface(surf);
}

SdlSurface SdlWindow::createBlankSurface() const
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window_.get(), &width, &height);
    return createBlankSurface(width, height);
}
