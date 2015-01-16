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
#include "SdlTextureStream.h"
#include <cassert>

SdlTextureStream::SdlTextureStream()
    : tex_{}
{
}

SdlTextureStream::SdlTextureStream(const SdlSurface &surf, SdlWindow &win)
    : tex_{}
{
    SDL_Texture *tmp = SDL_CreateTexture(win.getRenderer(),
                                         surf->format->format,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         surf->w,
                                         surf->h);
    if (tmp) {
        tex_ = SdlTexture{tmp, win, surf->w, surf->h};
        update(surf);
    }
}

void SdlTextureStream::update(const SdlSurface &surf)
{
    SDL_UpdateTexture(tex_.get(), nullptr, surf->pixels, surf->pitch);
}

void SdlTextureStream::draw(int px, int py)
{
    tex_.draw(px, py);
}

SdlTextureStream::operator bool() const
{
    return static_cast<bool>(tex_);
}

SDL_Texture * SdlTextureStream::get()
{
    return tex_.get();
}
