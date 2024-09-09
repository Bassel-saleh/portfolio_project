#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535f
#define SCREEN_H 720
#define mapS 64
#define mapX 8
#define mapY 8
#define DEG_TO_RAD(deg) ((deg) * (M_PI / 180.0))
#define RAD_TO_DEG(deg) ((deg) / (M_PI / 180.0))
#define P2 M_PI / 2
#define P3 3 * M_PI / 2
#define DIST(ax, ay, bx, by, ang) ( sqrt(((bx) - (ax)) + ((by) - (ay)) * ((by) - (ay))))

typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int main(void);
int init_instance(SDL_Instance *);
int poll_events(float *, float *, float *, float *, float *);
void draw_player(SDL_Instance, float *, float *,
	float *, float *);
void draw_map(SDL_Instance instance, int *);
void draw_rays(SDL_Instance, float *, float *, float *, int [64]);
void calculate_rx_ry(float *, float *, float *, float *, float , int [64]);

#endif
