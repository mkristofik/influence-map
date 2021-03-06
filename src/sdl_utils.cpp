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

    Uint32 uintFromColor(const SDL_Color &src)
    {
        return (src.r << 24) | (src.g << 16) | (src.b << 8) | src.a;
    }
}


bool operator==(const SDL_Color &lhs, const SDL_Color &rhs)
{
    return uintFromColor(lhs) == uintFromColor(rhs);
}

bool operator<(const SDL_Color &lhs, const SDL_Color &rhs)
{
    return uintFromColor(lhs) < uintFromColor(rhs);
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

SdlSurface sdlDeepCopy(const SdlSurface &src)
{
    auto dest = SDL_CreateRGBSurface(0,
                                     src->w,
                                     src->h,
                                     src->format->BitsPerPixel,
                                     src->format->Rmask,
                                     src->format->Gmask,
                                     src->format->Bmask,
                                     src->format->Amask);
    if (!dest) {
        std::cerr << "Error copying surface: " << SDL_GetError();
        return nullptr;
    }

    memcpy(dest->pixels, src->pixels,
           src->w * src->h * src->format->BytesPerPixel);
    return make_surface(dest);
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
    // TODO: SDL 2.0 has an API for this?
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

SDL_Color sdlGetPixel(const SdlSurface &surf, const Uint8 *pixel)
{
    SDL_Color c{0};
    const auto format = surf->format;
    if (format->BytesPerPixel == 3) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        c.r = pixel[2];
        c.g = pixel[1];
        c.b = pixel[0];
#else
        c.r = pixel[0];
        c.g = pixel[1];
        c.b = pixel[2];
#endif
    }
    else if (format->BytesPerPixel == 4) {
        SDL_GetRGBA(*reinterpret_cast<const Uint32 *>(pixel), format,
                    &c.r, &c.g, &c.b, &c.a);
    }
    else {
        assert(false);
    }

    return c;
}

void sdlSetPixel(SdlSurface &surf, Uint8 *pixel, const SDL_Color &color)
{
    const auto format = surf->format;
    if (format->BytesPerPixel == 3) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        pixel[2] = color.r;
        pixel[1] = color.g;
        pixel[0] = color.b;
#else
        pixel[0] = color.r;
        pixel[1] = color.g;
        pixel[2] = color.b;
#endif
    }
    else if (format->BytesPerPixel == 4) {
        auto target = reinterpret_cast<Uint32 *>(pixel);
        *target = SDL_MapRGBA(surf->format, color.r, color.g, color.b,
                              color.a);
    }
    else {
        assert(false);
    }
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
    : surf_{surf.get()},
    locked_{false}
{
    if (SDL_MUSTLOCK(surf_)) {
        if (SDL_LockSurface(surf_) == 0) {
            locked_ = true;
        }
        else {
            std::cerr << "Error locking surface: " << SDL_GetError();
        }
    }
}

SdlLockSurface::~SdlLockSurface()
{
    if (locked_ && SDL_MUSTLOCK(surf_)) {
        SDL_UnlockSurface(surf_);
    }
}
