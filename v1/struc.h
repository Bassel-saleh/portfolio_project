#ifndef _STRUC_H_
#define _STRUC_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define M_PI 3.14159265358979323846
#define FOV_DEG 60
#define NUM_RAYS 60
#define SCREEN_HEIGHT 720

typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

typedef struct
{
	int x, y;
} Point;

typedef struct
{
	int map[MAP_WIDTH][MAP_HEIGHT];
} Map;

int init_instance(SDL_Instance *);
int main(void);
int poll_events(void);
void draw_stuff(SDL_Instance instance, Map map,
	Point player_position, double player_angle);
void raycast(SDL_Renderer *, int, int, double, Map);
Map create_map(void);

#endif
