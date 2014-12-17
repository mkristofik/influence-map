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
#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include "SDL.h"
#include <memory>

using SdlSurface = std::shared_ptr<SDL_Surface>;

const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};
const SDL_Color YELLOW = {255, 255, 0, SDL_ALPHA_OPAQUE};
const SDL_Color GREY = {192, 192, 192, SDL_ALPHA_OPAQUE};
const SDL_Color BORDER_FG = {96, 100, 96, SDL_ALPHA_OPAQUE};
const SDL_Color BORDER_BG = {32, 32, 24, SDL_ALPHA_OPAQUE};
const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};

bool operator==(const SDL_Color &lhs, const SDL_Color &rhs);
bool operator<(const SDL_Color &lhs, const SDL_Color &rhs);

// Must call this before any other SDL functions will work.
void sdlInit();

// Like std::make_shared, but with SDL_Surface.
SdlSurface make_surface(SDL_Surface *surf);

SdlSurface sdlDeepCopy(const SdlSurface &src);

// Load a resource from disk.  Returns null on failure.
// note: don't try to allocate these at global scope.  They need sdlInit()
// before they will work, and the objects must be freed before SDL teardown
// happens.
SdlSurface sdlLoadImage(const char *filename);
SdlSurface sdlLoadImage(const std::string &filename);

// Return true if the given point is inside the rectangle.
bool sdlInsideRect(int px, int py, const SDL_Rect &rect);

// Translate a rectangle such that it stays inside the boundary.
SDL_Rect sdlBoundRect(SDL_Rect src, const SDL_Rect &bounds);

// Pixel-level Euclidian distance.
double sdlDistance(const SDL_Point &p1, const SDL_Point &p2);

// Accessors for pixel color. Be sure to lock the surface first.
SDL_Color sdlGetPixel(const SdlSurface &surf, const Uint8 *pixel);
void sdlSetPixel(SdlSurface &surf, Uint8 *pixel, const SDL_Color &color);

// RAII guard for setting/restoring the clipping region.
class SdlClipRect
{
public:
    SdlClipRect(SDL_Renderer *renderer, const SDL_Rect &clip);
    ~SdlClipRect();
private:
    SDL_Renderer *ren_;
    SDL_Rect orig_;
};

// RAII guard for setting/restoring the drawing color.
class SdlDrawColor
{
public:
    SdlDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b);
    ~SdlDrawColor();
private:
    SDL_Renderer *ren_;
    Uint8 origR_;
    Uint8 origG_;
    Uint8 origB_;
};

// RAII guard for pixel-level access to a surface.
class SdlLockSurface
{
public:
    SdlLockSurface(SdlSurface &surf);
    ~SdlLockSurface();
private:
    SDL_Surface *surf_;
    bool locked_;
};

#endif
