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
#include "team_color.h"
#include <algorithm>
#include <cassert>

// Reference color for each team.  The other 18 shades are offset from
// this, 14 darker and 4 lighter.
const std::vector<SDL_Color> teamColors = {
    {0x2E, 0x41, 0x9B, 0}, // blue
    {0xFF, 0, 0, 0},       // red
};

namespace
{
    using TeamColorMatrix = std::vector<std::vector<SDL_Color>>;

    // Magenta color palette to be replaced by team colors.
    const std::vector<SDL_Color> baseColors = {
        {0x3F, 0, 0x16, 0},
        {0x55, 0, 0x2A, 0},
        {0x69, 0, 0x39, 0},
        {0x7B, 0, 0x45, 0},
        {0x8C, 0, 0x51, 0},
        {0x9E, 0, 0x5D, 0},
        {0xB1, 0, 0x69, 0},
        {0xC3, 0, 0x74, 0},
        {0xD6, 0, 0x7F, 0},
        {0xEC, 0, 0x8C, 0},
        {0xEE, 0x3D, 0x96, 0},
        {0xEF, 0x5B, 0xA1, 0},
        {0xF1, 0x72, 0xAC, 0},
        {0xF2, 0x87, 0xB6, 0},
        {0xF4, 0x9A, 0xC1, 0},
        {0xF6, 0xAD, 0xCD, 0},
        {0xF8, 0xC1, 0xD9, 0},
        {0xFA, 0xD5, 0xE5, 0},
        {0xFD, 0xE9, 0xF1, 0},
    };

    std::vector<SDL_Color> makeTeamColors(const SDL_Color &refColor)
    {
        std::vector<SDL_Color> colors(19);

        // Reference color.
        colors[14] = refColor;

        // Fade toward black in 1/16th increments.
        auto rStep = refColor.r / 16.0;
        auto gStep = refColor.g / 16.0;
        auto bStep = refColor.b / 16.0;
        double curR = refColor.r;
        double curG = refColor.g;
        double curB = refColor.b;
        for (int i = 13; i >= 0; --i) {
            curR -= rStep;
            curG -= gStep;
            curB -= bStep;
            colors[i] = {static_cast<Uint8>(curR),
                         static_cast<Uint8>(curG),
                         static_cast<Uint8>(curB),
                         0};
        }

        // Fade toward white in 1/5th increments.
        rStep = (255 - refColor.r) / 5.0;
        gStep = (255 - refColor.g) / 5.0;
        bStep = (255 - refColor.b) / 5.0;
        curR = refColor.r;
        curG = refColor.g;
        curB = refColor.b;
        for (int i = 15; i < 19; ++i) {
            curR += rStep;
            curG += gStep;
            curB += bStep;
            colors[i] = {static_cast<Uint8>(curR),
                         static_cast<Uint8>(curG),
                         static_cast<Uint8>(curB),
                         0};
        }

        return colors;
    }

    TeamColorMatrix initTeamColors()
    {
        TeamColorMatrix ret;
        for (const auto &color : teamColors) {
            ret.emplace_back(makeTeamColors(color));
        }
        return ret;
    }

    const TeamColorMatrix allShades = initTeamColors();

    SDL_Color translateColor(const SDL_Color &orig, Team t)
    {
        auto color = orig;
        color.a = 0;
        auto iter = lower_bound(std::begin(baseColors), std::end(baseColors),
                                color);
        if (iter != std::end(baseColors) && *iter == color) {
            const auto index = distance(std::begin(baseColors), iter);
            color = allShades[static_cast<int>(t)][index];
        }

        color.a = orig.a;
        return color;
    }

    SDL_Color getPixelColor(const SdlSurface &surf, const Uint8 *ptr)
    {
        SDL_Color c{0};
        const auto format = surf->format;
        if (format->BytesPerPixel == 3) {
            c.r = ptr[0];
            c.g = ptr[1];
            c.b = ptr[2];
        }
        else if (format->BytesPerPixel == 4) {
            SDL_GetRGBA(*reinterpret_cast<const Uint32 *>(ptr), format,
                        &c.r, &c.g, &c.b, &c.a);
        }
        else {
            assert(false);
        }

        return c;
    }

    void setPixelColor(const SdlSurface &surf, Uint8 *ptr,
                       const SDL_Color &color)
    {
        const auto format = surf->format;
        if (format->BytesPerPixel == 3) {
            ptr[0] = color.r;
            ptr[1] = color.g;
            ptr[2] = color.b;
        }
        else if (format->BytesPerPixel == 4) {
            auto pixel = reinterpret_cast<Uint32 *>(ptr);
            *pixel = SDL_MapRGBA(surf->format, color.r, color.g, color.b,
                                 color.a);
        }
        else {
            assert(false);
        }
    }
}


SdlSurface applyTeamColor(const SdlSurface &src, Team team)
{
    auto img = sdlDeepCopy(src);
    SdlLockSurface guard{img};

    auto pixel = static_cast<Uint8 *>(img->pixels);
    const auto bpp = img->format->BytesPerPixel;
    const auto end = pixel + img->w * img->h * bpp;
    for (; pixel != end; pixel += bpp) {
        const auto color = getPixelColor(img, pixel);
        setPixelColor(img, pixel, translateColor(color, team));
    }

    return img;
}
