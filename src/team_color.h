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
#ifndef TEAM_COLOR_H
#define TEAM_COLOR_H

#include "SdlWindow.h"
#include "sdl_utils.h"
#include <vector>

// This is an implementation of the team color algorithm from Battle for
// Wesnoth.  We reserve a specific palette of 19 shades of magenta as a
// reference.  Those colors are replaced at runtime with the corresponding
// color for each team.
enum class Team {BLUE, RED, NONE};
extern const std::vector<SDL_Color> teamColors;

// Translate the magenta palette to the team color.
SdlSurface applyTeamColor(const SdlSurface &src, Team team);

// Flags are green and need to be translated to magenta first.
SdlSurface applyFlagColor(const SdlSurface &src);

#endif
