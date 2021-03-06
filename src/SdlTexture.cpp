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
#include "SdlTexture.h"
#include <cassert>
#include <cmath>
#include <iostream>

SdlTexture::SdlTexture()
    : texture_{nullptr, SDL_DestroyTexture},
    renderer_{nullptr},
    width_{0},
    height_{0}
{
}

SdlTexture::SdlTexture(const SdlSurface &surf, SDL_Renderer *renderer)
    : texture_{nullptr, SDL_DestroyTexture},
    renderer_{renderer},
    width_{0},
    height_{0}
{
    assert(surf && renderer_);

    auto img = SDL_CreateTextureFromSurface(renderer_, surf.get());
    if (img == nullptr) {
        std::cerr << "Error creating texture: " << SDL_GetError();
        return;
    }

    width_ = surf.get()->w;
    height_ = surf.get()->h;
    texture_.reset(img);
}

SdlTexture::SdlTexture(const SdlSurface &surf, SdlWindow &win)
    : SdlTexture{surf, win.getRenderer()}
{
}

SdlTexture::SdlTexture(SDL_Texture *tex, SdlWindow &win, int width, int height)
    : texture_{tex, SDL_DestroyTexture},
    renderer_{win.getRenderer()},
    width_{width},
    height_{height}
{
}

int SdlTexture::width() const
{
    return width_;
}

int SdlTexture::height() const
{
    return height_;
}

void SdlTexture::draw(int px, int py, const SDL_Rect *srcRect)
{
    assert(*this);
    auto dest = getDestRect(px, py, srcRect);
    if (SDL_RenderCopy(renderer_, texture_.get(), srcRect, &dest) < 0) {
        std::cerr << "Error rendering texture: " << SDL_GetError();
    }
}

void SdlTexture::draw(const SDL_Point &p, const SDL_Rect *srcRect)
{
    draw(p.x, p.y, srcRect);
}

void SdlTexture::drawCentered(int px, int py, const SDL_Rect *srcRect)
{
    draw(px - width_ / 2, py - height_ / 2, srcRect);
}

void SdlTexture::drawCentered(const SDL_Point &p, const SDL_Rect *srcRect)
{
    drawCentered(p.x, p.y, srcRect);
}

void SdlTexture::drawRotated(int px, int py, double angle_rad,
                             const SDL_Rect *srcRect)
{
    assert(*this);
    auto angle_deg = static_cast<int>(angle_rad * 180.0 / M_PI);
    auto dest = getDestRect(px, py, srcRect);
    if (SDL_RenderCopyEx(renderer_,
                         texture_.get(),
                         srcRect,
                         &dest,
                         angle_deg,
                         nullptr,  // rotate about the center of draw target
                         SDL_FLIP_NONE) < 0)
    {
        std::cerr << "Error drawing texture rotated: " << SDL_GetError();
    }
}

void SdlTexture::drawRotated(const SDL_Point &p, double angle_rad,
                             const SDL_Rect *srcRect)
{
    drawRotated(p.x, p.y, angle_rad, srcRect);
}

void SdlTexture::drawFlippedH(int px, int py, const SDL_Rect *srcRect)
{
    assert(*this);
    auto dest = getDestRect(px, py, srcRect);
    if (SDL_RenderCopyEx(renderer_,
                         texture_.get(),
                         srcRect,
                         &dest,
                         0,
                         nullptr,
                         SDL_FLIP_HORIZONTAL) < 0)
    {
        std::cerr << "Error drawing texture rotated: " << SDL_GetError();
    }
}

void SdlTexture::drawFlippedH(const SDL_Point &p, const SDL_Rect *srcRect)
{
    drawFlippedH(p.x, p.y, srcRect);
}

void SdlTexture::drawZoomed(int px, int py, double zoom,
                            const SDL_Rect *srcRect)
{
    assert(*this);
    auto w = static_cast<int>(width_ * zoom);
    auto h = static_cast<int>(height_ * zoom);
    auto dest = SDL_Rect{px, py, w, h};
    if (SDL_RenderCopy(renderer_, texture_.get(), srcRect, &dest) < 0)
    {
        std::cerr << "Error drawing texture zoomed by int: " << SDL_GetError();
    }
}

void SdlTexture::drawZoomed(const SDL_Point &p, double zoom,
                            const SDL_Rect *srcRect)
{
    drawZoomed(p.x, p.y, zoom, srcRect);
}

void SdlTexture::drawZoomed(const SDL_Rect &destRect, const SDL_Rect *srcRect)
{
    assert(*this);
    if (SDL_RenderCopy(renderer_, texture_.get(), srcRect, &destRect) < 0)
    {
        std::cerr << "Error drawing texture zoomed to fit: " << SDL_GetError();
    }
}

SdlTexture::operator bool() const
{
    return texture_.get() != nullptr;
}

SDL_Texture * SdlTexture::get()
{
    return texture_.get();
}

SDL_Rect SdlTexture::getDestRect(int px, int py, const SDL_Rect *srcRect) const
{
    if (srcRect == nullptr) {
        return {px, py, width_, height_};  // by default draw the entire image
    }

    return {px, py, srcRect->w, srcRect->h};
}
