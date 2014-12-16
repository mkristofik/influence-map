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
#include <algorithm>

namespace
{
    const int xRegions = 8;
    const int yRegions = 4;
    const SDL_Point xyInvalid = {-1, -1};
}

SimpleMap::SimpleMap(int width, int height, int numTeams)
    : width_{width},
    height_{height},
    numTeams_{numTeams},
    influence_(xRegions * yRegions * numTeams, 0)
{
}

void SimpleMap::draw(SdlSurface &surf) const
{
    SdlLockSurface guard{surf};

    auto p = reinterpret_cast<Uint32 *>(surf->pixels);
    for (int i = 0; i < width_ * height_; ++i, ++p) {
        const auto color = getColor(i);
        *p = SDL_MapRGB(surf->format, color.r, color.g, color.b);
    }
}

void SimpleMap::setInfluence(int region, Team team, int value)
{
    influence_[region * numTeams_ + static_cast<int>(team)] = value;
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

SDL_Color SimpleMap::getColor(int a) const
{
    const auto reg = regionFromAry(a);
    const auto p = pixelFromAry(a);

    const auto regN = regionFromPixel({p.x, p.y - 1});
    if (regN != -1 && regN != reg) {
        return getBorderColor(reg, regN);
    }
    const auto regNE = regionFromPixel({p.x + 1, p.y - 1});
    if (regNE != -1 && regNE != reg) {
        return getBorderColor(reg, regNE);
    }
    const auto regE = regionFromPixel({p.x + 1, p.y});
    if (regE != -1 && regE != reg) {
        return getBorderColor(reg, regE);
    }
    const auto regSE = regionFromPixel({p.x + 1, p.y + 1});
    if (regSE != -1 && regSE != reg) {
        return getBorderColor(reg, regSE);
    }
    const auto regS = regionFromPixel({p.x, p.y + 1});
    if (regS != -1 && regS != reg) {
        return getBorderColor(reg, regS);
    }
    const auto regSW = regionFromPixel({p.x - 1, p.y + 1});
    if (regSW != -1 && regSW != reg) {
        return getBorderColor(reg, regSW);
    }
    const auto regW = regionFromPixel({p.x - 1, p.y});
    if (regW != -1 && regW != reg) {
        return getBorderColor(reg, regW);
    }
    const auto regNW = regionFromPixel({p.x - 1, p.y - 1});
    if (regNW != -1 && regNW != reg) {
        return getBorderColor(reg, regNW);
    }

    return GREY;
}

SDL_Color SimpleMap::getBorderColor(int reg1, int reg2) const
{
    const auto owner1 = getOwner(reg1);
    const auto owner2 = getOwner(reg2);

    if (owner1 == owner2) {
        return BLACK;
    }
    else if (owner1 == -1) {
        return teamColors[owner2];
    }

    return teamColors[owner1];
}

int SimpleMap::getOwner(int region) const
{
    auto begin = &influence_[region * numTeams_];
    auto end = &influence_[(region + 1) * numTeams_];
    auto maxPtr = std::max_element(begin, end);
    if (*maxPtr > 0) {
        return std::distance(begin, maxPtr);
    }

    return -1;
}
