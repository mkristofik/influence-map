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
#include <array>
#include <iostream> //TODO

namespace
{
    const int xRegions = 8;
    const int yRegions = 4;
    const SDL_Point xyInvalid = {-1, -1};

    std::array<int, 4> regionNeighbors(int reg)
    {
        std::array<int, 4> nbrs;
        nbrs.fill(-1);

        const int regN = reg - xRegions;
        if (regN >= 0) {
            nbrs[0] = regN;
        }
        const int regE = reg + 1;
        if (regE % xRegions != 0) {
            nbrs[1] = regE;
        }
        const int regS = reg + xRegions;
        if (regS < xRegions * yRegions) {
            nbrs[2] = regS;
        }
        const int regW = reg - 1;
        if (reg % xRegions != 0) {
            nbrs[3] = regW;
        }

        return nbrs;
    }
}

SimpleMap::SimpleMap(int width, int height, int numTeams,
                     const SdlSurface &blankUpdateSurf)
    : width_{width},
    height_{height},
    numTeams_{numTeams},
    influence_(xRegions * yRegions * numTeams_, 0),
    entities_{},
    updateSurf_{blankUpdateSurf}
{
}

SdlSurface SimpleMap::update()
{
    relaxInfluence();

    SdlLockSurface guard{updateSurf_};
    auto p = static_cast<Uint8 *>(updateSurf_->pixels);
    const auto bpp = updateSurf_->format->BytesPerPixel;
    for (int i = 0; i < width_ * height_; ++i, p += bpp) {
        sdlSetPixel(updateSurf_, p, getColor(i));
    }

    return updateSurf_;
}

void SimpleMap::addEntity(MapEntity entity)
{
    // TODO: this needs to be sorted
    entities_.push_back(entity);
}

void SimpleMap::moveEntity(int id, int toReg)
{
    auto entity = findEntity(id);
    if (entity) {
        entity->region = toReg;
    }
}

int SimpleMap::getRegion(int entityId) const
{
    auto entity = findEntity(entityId);
    if (!entity) {
        return -1;
    }
    return entity->region;
}

SDL_Point SimpleMap::pixelFromRegion(int reg) const
{
    if (reg < 0 || reg >= xRegions * yRegions) {
        return xyInvalid;
    }

    const int rx = reg % xRegions;
    const int ry = reg / xRegions;
    const int rWidth = width_ / xRegions;
    const int rHeight = height_ / yRegions;
    return {rx * rWidth + rWidth / 2, ry * rHeight + rHeight / 2};
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
        if (owner1 != -1) {
            return BORDER_FG;
        }
        else {
            return BLACK;
        }
    }
    else if (owner1 == -1) {
        return teamColors[owner2];
    }

    return teamColors[owner1];
}

int SimpleMap::getOwner(int region) const
{
    auto maxInfl = 0;
    auto owner = -1;
    auto team = 0;
    for (int i = region * numTeams_; i < (region + 1) * numTeams_; ++i, ++team) {
        if (influence_[i] > maxInfl) {
            maxInfl = influence_[i];
            owner = team;
        }
        else if (influence_[i] == maxInfl) {
            owner = -1;
        }
    }

    return owner;
}

int SimpleMap::teamOffset(int region, Team team) const
{
    return region * numTeams_ + static_cast<int>(team);
}

void SimpleMap::addInfluence(int region, Team team, int value)
{
    influence_[teamOffset(region, team)] += value;
}

void SimpleMap::relaxInfluence()
{
    fill(begin(influence_), end(influence_), 0);

    for (const auto &e : entities_) {
        addInfluence(e.region, e.team, e.influence);
        for (auto r: regionNeighbors(e.region)) {
             if (r != -1) {
                 addInfluence(r, e.team, e.influence / 4);
             }
        }
    }
}

const MapEntity * SimpleMap::findEntity(int id) const
{
    auto it = lower_bound(begin(entities_), end(entities_), id,
        [] (const MapEntity &elem, int id) { return elem.id < id; });

    if (it != end(entities_) && it->id == id) {
        return &*it;
    }

    return nullptr;
}

MapEntity * SimpleMap::findEntity(int id)
{
    return const_cast<MapEntity *>(const_cast<const SimpleMap &>(*this).findEntity(id));
}
