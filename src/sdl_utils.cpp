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
#include "sdl_utils.h"

#include "SDL_image.h"
#include "boost/filesystem.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace
{
    // Get the full path to an image file.
    std::string getImagePath(const char *filename)
    {
        boost::filesystem::path imagePath{"../img"};
        imagePath /= filename;
        return imagePath.string();
    }
}


void sdlInit()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError();
        throw std::runtime_error("sdlInit failed.");
    }
    atexit(SDL_Quit);

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Error initializing SDL_image: " << IMG_GetError();
        throw std::runtime_error("sdlInit failed.");
    }
    atexit(IMG_Quit);
}

SdlSurface make_surface(SDL_Surface *surf)
{
    return SdlSurface(surf, SDL_FreeSurface);
}

SdlSurface sdlLoadImage(const char *filename)
{
    assert(SDL_WasInit(SDL_INIT_VIDEO) != 0);

    auto img = make_surface(IMG_Load(getImagePath(filename).c_str()));
    if (!img) {
        std::cerr << "Error loading image " << filename
            << "\n    " << IMG_GetError();
    }
    return img;
}

SdlSurface sdlLoadImage(const std::string &filename)
{
    return sdlLoadImage(filename.c_str());
}

bool sdlInsideRect(int px, int py, const SDL_Rect &rect)
{
    return px >= rect.x &&
           py >= rect.y &&
           px < rect.x + rect.w &&
           py < rect.y + rect.h;
}

SDL_Rect sdlBoundRect(SDL_Rect src, const SDL_Rect &bounds)
{
    assert(src.w <= bounds.w && src.h <= bounds.h);

    if (src.x < bounds.x) {
        src.x = bounds.x;
    }
    else if (src.x + src.w > bounds.x + bounds.w) {
        src.x = bounds.x + bounds.w - src.w;
    }
    if (src.y < bounds.y) {
        src.y = bounds.y;
    }
    else if (src.y + src.h > bounds.y + bounds.h) {
        src.y = bounds.y + bounds.h - src.h;
    }

    return src;
}

double sdlDistance(const SDL_Point &p1, const SDL_Point &p2)
{
    const double dx = p1.x - p2.x;
    const double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

SdlClipRect::SdlClipRect(SDL_Renderer *renderer, const SDL_Rect &clip)
    : ren_{renderer},
    orig_{}
{
    SDL_RenderGetClipRect(ren_, &orig_);
    SDL_RenderSetClipRect(ren_, &clip);
}

SdlClipRect::~SdlClipRect()
{
    SDL_RenderSetClipRect(ren_, &orig_);
}

SdlDrawColor::SdlDrawColor(SDL_Renderer *ren, Uint8 r, Uint8 g, Uint8 b)
    : ren_{ren},
    origR_{0},
    origG_{0},
    origB_{0}
{
    Uint8 ignore;
    SDL_GetRenderDrawColor(ren_, &origR_, &origG_, &origB_, &ignore);
    SDL_SetRenderDrawColor(ren_, r, g, b, SDL_ALPHA_OPAQUE);
}

SdlDrawColor::~SdlDrawColor()
{
    SDL_SetRenderDrawColor(ren_, origR_, origG_, origB_, SDL_ALPHA_OPAQUE);
}

SdlLockSurface::SdlLockSurface(SdlSurface &surf)
    : surf_{surf},
    locked_{false}
{
    if (SDL_MUSTLOCK(surf_.get())) {
        if (SDL_LockSurface(surf_.get()) == 0) {
            locked_ = true;
        }
        else {
            std::cerr << "Error locking surface: " << SDL_GetError();
        }
    }
}

SdlLockSurface::~SdlLockSurface()
{
    if (locked_ && SDL_MUSTLOCK(surf_.get())) {
        SDL_UnlockSurface(surf_.get());
    }
}
