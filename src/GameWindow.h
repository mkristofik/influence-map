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
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "SdlTextureStream.h"
#include "SdlWindow.h"
#include <vector>

struct DrawableEntity
{
    int id;
    SDL_Point pixel;
    SdlTexture img;
};


class GameWindow
{
public:
    GameWindow(int width, int height, const char *title);

    SdlSurface getBlankMap() const;
    void updateMap(const SdlSurface &surf);

    void addEntity(int id, SDL_Point pixel, const SdlSurface &surf);
    void moveEntity(int id, SDL_Point pixel);

    void draw();

private:
    const DrawableEntity * findEntity(int id) const;
    DrawableEntity * findEntity(int id);

    SdlWindow win_;
    SdlTextureStream advMap_;
    std::vector<DrawableEntity> entities_;
};

#endif
