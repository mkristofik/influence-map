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
#include <cmath>
#include <ctime>
#include <random>

namespace
{
    std::minstd_rand randgen(static_cast<unsigned int>(std::time(nullptr)));
}

/* TODO:
   generate voronoi diagram at the pixel level
   lots of small regions around the outside edges (water)
   - use 5% of the width/height on each side?
   larger regions on the inside (land)
   any pixel that borders another region gets colored black
*/

// Two groups: many centers around the edges, fewer centers inset in the
// middle.
std::vector<SDL_Point> randomCenters(int width, int height)
{
    typedef std::uniform_int_distribution<int> RandDist;
    std::vector<SDL_Point> centers;

    const auto xInsetMin = static_cast<int>(width * 0.05);
    const auto xInsetMax = static_cast<int>(width * 0.95);
    const auto yInsetMin = static_cast<int>(height * 0.05);
    const auto yInsetMax = static_cast<int>(height * 0.95);

    RandDist horizEdgeX(0, width - 1);
    RandDist topEdgeY(0, yInsetMin);
    RandDist bottomEdgeY(yInsetMax, height - 1);
    for (int i = 0; i < 20; ++i) {
        const int x1 = horizEdgeX(randgen);
        const int x2 = horizEdgeX(randgen);
        const int y1 = topEdgeY(randgen);
        const int y2 = bottomEdgeY(randgen);
        centers.push_back(SDL_Point{x1, y1});
        centers.push_back(SDL_Point{x2, y2});
    }

    RandDist leftEdgeX(0, xInsetMin);
    RandDist rightEdgeX(xInsetMax, width - 1);
    RandDist vertEdgeY(0, height - 1);
    for (int i = 0; i < 10; ++i) {
        const int x1 = leftEdgeX(randgen);
        const int x2 = rightEdgeX(randgen);
        const int y1 = vertEdgeY(randgen);
        const int y2 = vertEdgeY(randgen);
        centers.push_back(SDL_Point{x1, y1});
        centers.push_back(SDL_Point{x2, y2});
    }

    RandDist middleX(xInsetMin, xInsetMax);
    RandDist middleY(yInsetMin, yInsetMax);
    for (int i = 0; i < 50; ++i) {
        const int x = middleX(randgen);
        const int y = middleY(randgen);
        centers.push_back(SDL_Point{x, y});
    }

    return centers;
}
