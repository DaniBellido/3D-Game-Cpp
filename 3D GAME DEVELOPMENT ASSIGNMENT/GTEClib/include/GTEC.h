/*********************************************************************
GTEC - Games Technology Game Engine
Copyright (C) 2012-2020  Andreas Hoppe

This program is free software; you can redistribute it and /or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see < http://www.gnu.org/licenses/>.
*************************************************************************/


#define __STDC_WANT_LIB_EXT1__ 1
#ifndef _WIN32
#define sprintf_s snprintf
#include <GL/gl.h>
#include <GL/glu.h>
#include <sys/time.h>
#else
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <SDL.h>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <string>
#include <fstream>

using namespace std;

#define _USE_MATH_DEFINES

#define RAD2DEG(x)	((double)(x) * 180.0 / 3.14159265359)
#define DEG2RAD(x)  ((double)(x) * 3.14159265359 / 180.0)

