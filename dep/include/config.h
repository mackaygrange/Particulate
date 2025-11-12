#ifndef CONFIG_H
#define CONFIG_H

#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAIN_DISPLAY (2)
#define SECONDARY_DISPLAY (1)

#define DISPLAY_WIDTH (2560)
#define DISPLAY_HEIGHT (1440)

#define SCREEN_WIDTH (2560)
#define SCREEN_HEIGHT (1440)

#define WINDOW_FLAGS (SDL_WINDOW_BORDERLESS || SDL_WINDOW_ALLOW_HIGHDPI)
#define RENDERER_FLAGS (SDL_RENDERER_PRESENTVSYNC)
#define TEXTURE_FLAGS ()

#define FPS (60)
#define TARGET_FRAME_TIME (1000.0f / (f32)FPS)

#define PI (3.14159265358979323846f)
#define DEG_RAD_RATIO (PI / 180.0f)

#define F_NEAR (0.1f)
#define F_FAR (1000.0f)
#define F_FOV (60.0f)
#define F_ASPECT_RATIO ((f32)SCREEN_WIDTH / (f32)SCREEN_HEIGHT)

#define MAX_VERTICES (1000)
#define MAX_TRIANGLES (1000)

#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); SDL_Delay(3000); SDL_Quit(); exit(1);}

typedef float f32;
typedef double f64;
typedef Sint32 si32;
typedef Uint32 ui32;

typedef struct matrix matrix;
typedef struct triangle triangle;
typedef struct vertex vertex;
typedef struct mesh mesh;
typedef struct quaternion quaternion;
typedef struct state state;

#endif
