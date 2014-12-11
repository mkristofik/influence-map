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
#ifndef SDL_TEXTURE_H
#define SDL_TEXTURE_H

#include "SdlWindow.h"
#include "sdl_utils.h"
#include <memory>

// Wrapper around SDL_Texture, representing a static image in video memory.
class SdlTexture
{
public:
    SdlTexture();
    SdlTexture(const SdlSurface &surf, SDL_Renderer *renderer);
    SdlTexture(const SdlSurface &surf, SdlWindow &win);

    // Take control of an existing texture.
    SdlTexture(SDL_Texture *tex, SdlWindow &win, int width, int height);

    int width() const;
    int height() const;

    // Draw the texture to its window using (px,py) as the upper-left corner.
    // Optionally provide 'srcRect' to draw only a portion of the image.
    void draw(int px, int py, const SDL_Rect *srcRect = nullptr);

    // Rotate the texture clockwise before drawing.
    void drawRotated(int px, int py, double angle_rad,
                     const SDL_Rect *srcRect = nullptr);

    // Flip the texture before drawing.
    void drawFlippedH(int px, int py, const SDL_Rect *srcRect = nullptr);

    // Scale a texture up (> 1.0) or down (< 1.0) before drawing.
    void drawZoomed(int px, int py, double zoom,
                    const SDL_Rect *srcRect = nullptr);

    // Draw a texture scaled to fit the given destination rectangle.
    void drawZoomed(const SDL_Rect &destRect, const SDL_Rect *srcRect = nullptr);

    explicit operator bool() const;
    SDL_Texture * get();

private:
    // Return the bounding box for drawing the (un-zoomed) texture at (px,py).
    SDL_Rect getDestRect(int px, int py,
                         const SDL_Rect *srcRect = nullptr) const;

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture_;
    SDL_Renderer *renderer_;
    int width_;
    int height_;
};

#endif
