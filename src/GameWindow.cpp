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
#include "GameWindow.h"
#include <algorithm>

GameWindow::GameWindow(int width, int height, const char *title)
    : win_{width, height, title},
    advMap_{}
{
}

SdlSurface GameWindow::getBlankMap() const
{
    return win_.createBlankSurface();
}

void GameWindow::updateMap(const SdlSurface &surf)
{
    if (!advMap_) {
        advMap_ = SdlTextureStream{surf, win_};
    }
    else {
        advMap_.update(surf);
    }
}

void GameWindow::addEntity(int id, SDL_Point pixel, const SdlSurface &surf)
{
    DrawableEntity e;
    e.id = id;
    e.pixel = pixel;
    e.img = SdlTexture{surf, win_};

    // TODO: this needs to be sorted
    entities_.push_back(std::move(e));
}

void GameWindow::moveEntity(int id, SDL_Point pixel)
{
    auto entity = findEntity(id);
    if (entity) {
        entity->pixel = pixel;
    }
}

void GameWindow::draw()
{
    win_.clear();
    advMap_.draw(0, 0);
    for (auto &e : entities_) {
        e.img.drawCentered(e.pixel);
    }
    win_.draw();
}

const DrawableEntity * GameWindow::findEntity(int id) const
{
    auto it = lower_bound(begin(entities_), end(entities_), id,
        [] (const DrawableEntity &elem, int id) { return elem.id < id; });

    if (it != end(entities_) && it->id == id) {
        return &*it;
    }

    return nullptr;
}

DrawableEntity * GameWindow::findEntity(int id)
{
    return const_cast<DrawableEntity *>(const_cast<const GameWindow &>(*this).findEntity(id));
}
