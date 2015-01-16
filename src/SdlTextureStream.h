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
#ifndef SDL_TEXTURE_STREAM_H
#define SDL_TEXTURE_STREAM_H

#include "SdlTexture.h"
#include "SdlWindow.h"
#include "sdl_utils.h"

// Wrapper around a streaming texture, an image in video memory that is
// expected to change frequently.
class SdlTextureStream
{
public:
    SdlTextureStream();
    SdlTextureStream(const SdlSurface &surf, SdlWindow &win);

    void update(const SdlSurface &surf);
    void draw(int px, int py);

    explicit operator bool() const;
    SDL_Texture * get();

private:
    SdlTexture tex_;
};

#endif
