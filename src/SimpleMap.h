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

#include "sdl_utils.h"

class SimpleMap
{
public:
    SimpleMap(int width, int height);
    void draw(SdlSurface &surf) const;

private:
    SDL_Point pixelFromAry(int a) const;
    int regionFromPixel(const SDL_Point &p) const;
    int regionFromAry(int a) const;

    // Are any of the 8 neighboring pixels in a different region?
    bool neighborsDiff(int a) const;

    int width_;
    int height_;
};

#endif
