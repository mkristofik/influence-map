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
#include "SimpleMap.h"

namespace
{
    const int xRegions = 6;
    const int yRegions = 4;
    const SDL_Point xyInvalid = {-1, -1};
}

SimpleMap::SimpleMap(int width, int height)
    : width_{width},
    height_{height}
{
}

void SimpleMap::draw(SdlSurface &surf) const
{
    // for each pixel in surface
    // if one of 8 neighbors is in a different region, color black
    // else, color light grey

    /*
     SDL_CreateTexture(renderer,
                       SDL_PIXELFORMAT_ARGB8888,
                       SDL_TEXTUREACCESS_STREAMING,
                       width,
                       height);

     Uint32 * surface->pixels
     SDL_MapRGB(surface->format, r, g, b);
     SDL_UpdateTexture(sdlTexture, NULL, surface->pixels, surface->pitch);
     SDL_RenderClear(sdlRenderer);  --> SdlWindow::clear
     SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
     SDL_RenderPresent(sdlRenderer);  --> SdlWindow::draw
     */
}

SDL_Point SimpleMap::pixelFromAry(int a) const
{
    if (a < 0 || a >= width_ * height_) {
        return xyInvalid;
    }

    return {a % width_, a / width_};
}

int SimpleMap::regionFromPixel(const SDL_Point &p) const
{
    if (p.x < 0 || p.x >= width_ || p.y < 0 || p.y >= height_) {
        return -1;
    }

    const int rx = p.x * xRegions / width_;
    const int ry = p.y * yRegions / height_;
    return ry * xRegions + rx;
}

int SimpleMap::regionFromAry(int a) const
{
    return regionFromPixel(pixelFromAry(a));
}

bool SimpleMap::neighborsDiff(int a) const
{
    const auto reg = regionFromAry(a);
    const auto p = pixelFromAry(a);

    const auto regN = regionFromPixel({p.x, p.y - 1});
    if (regN != -1 && regN != reg) {
        return true;
    }
    const auto regNE = regionFromPixel({p.x + 1, p.y - 1});
    if (regNE != -1 && regNE != reg) {
        return true;
    }
    const auto regE = regionFromPixel({p.x + 1, p.y});
    if (regE != -1 && regE != reg) {
        return true;
    }
    const auto regSE = regionFromPixel({p.x + 1, p.y + 1});
    if (regSE != -1 && regSE != reg) {
        return true;
    }
    const auto regS = regionFromPixel({p.x, p.y + 1});
    if (regS != -1 && regS != reg) {
        return true;
    }
    const auto regSW = regionFromPixel({p.x - 1, p.y + 1});
    if (regSW != -1 && regSW != reg) {
        return true;
    }
    const auto regW = regionFromPixel({p.x - 1, p.y});
    if (regW != -1 && regW != reg) {
        return true;
    }
    const auto regNW = regionFromPixel({p.x - 1, p.y - 1});
    if (regNW != -1 && regNW != reg) {
        return true;
    }

    return false;
}
