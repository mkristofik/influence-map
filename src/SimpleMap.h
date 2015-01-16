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
#ifndef SIMPLE_MAP_H
#define SIMPLE_MAP_H

#include "sdl_utils.h"
#include "team_color.h"
#include <vector>

struct MapEntity
{
    int id;
    int region;
    int influence;
    Team team;
};


class SimpleMap
{
public:
    SimpleMap(int width, int height, int numTeams,
              const SdlSurface &blankUpdateSurf);

    SdlSurface update();

    void addEntity(MapEntity entity);
    void moveEntity(int id, int toReg);
    int getRegion(int entityId) const;

    // Return the center pixel of the given region.
    SDL_Point pixelFromRegion(int reg) const;

private:
    SDL_Point pixelFromAry(int a) const;
    int regionFromPixel(const SDL_Point &p) const;
    int regionFromAry(int a) const;

    SDL_Color getColor(int a) const;
    SDL_Color getBorderColor(int reg1, int reg2) const;

    // Return the team number with the most influence in a region, or -1 if all
    // teams have the same influence.
    int getOwner(int region) const;

    // Return the index of the team's data in the influence map.
    int teamOffset(int region, Team team) const;

    // Spread each entity's influence to neighboring regions.
    void addInfluence(int region, Team team, int value);
    void relaxInfluence();

    const MapEntity * findEntity(int id) const;
    MapEntity * findEntity(int id);

    int width_;
    int height_;
    int numTeams_;
    std::vector<int> influence_;
    std::vector<MapEntity> entities_;
    SdlSurface updateSurf_;
};

#endif
