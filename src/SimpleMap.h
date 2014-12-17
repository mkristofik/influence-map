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
#ifndef SIMPLE_MAP_H
#define SIMPLE_MAP_H

#include "SdlWindow.h"
#include "sdl_utils.h"
#include "team_color.h"
#include <vector>

class SimpleMap
{
public:
    SimpleMap(int width, int height, int numTeams, const SdlWindow &win);
    SdlSurface draw();
    void setInfluence(int region, Team team, int value);

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

    // Spread each team's influence to neighboring regions.
    void relaxInfluence();

    int width_;
    int height_;
    int numTeams_;
    std::vector<int> influence_;
    std::vector<int> influenceToDraw_;
    SdlSurface updateSurf_;
};

#endif
