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
    const auto grey = SDL_MapRGB(surf->format, 192, 192, 192);
    const auto black = SDL_MapRGB(surf->format, 0, 0, 0);
    auto p = reinterpret_cast<Uint32 *>(surf->pixels);
    for (int i = 0; i < width_ * height_; ++i, ++p) {
        if (neighborsDiff(i)) {
            *p = black;
        }
        else {
            *p = grey;
        }
    }
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
