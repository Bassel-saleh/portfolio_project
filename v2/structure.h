#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define ASSERT(cond, ...)												\
	do {																\
		if (!(cond))													\
		{																\
			fprintf(stderr, __VA_ARGS__);								\
			exit(1);													\
		}																\
	} while (0)

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAP_SIZE 16
extern const uint8_t MAP[MAP_SIZE * MAP_SIZE];

#define PI 3.14159265f
#define playerFOV (PI / 2.0f)
#define maxDepth 20.0f

/**
 * enum Side - Represents possible directions in a 2D plane
 * @NorthSouth: Indicates movement or direction along the north-south axis
 * @EastWest: Indicates movement or direction along the east-west axis
 */
typedef enum Side
{
	NorthSouth, EastWest
} Side;

/**
 * struct Vec2F - Represents a 2D vector with floating-point coordinates
 * @x: The x-coordinate of the vector
 * @y: The y-coordinate of the vector
 */
typedef struct Vec2F
{
	float x, y;
} Vec2F;

/**
 * struct Vec2I - Represents a 2D vector with integer coordinates
 * @x: The x-coordinate of the vector
 * @y: The y-coordinate of the vector
 */
typedef struct Vec2I
{
	int x, y;
} Vec2I;

/**
 * struct State - Holds the window, renderer, and state of the game loop
 * @window: Pointer to the SDL_Window structure
 * @renderer: Pointer to the SDL_Renderer structure for rendering
 * @quit: Boolean indicating if the game loop should quit
 */
typedef struct State
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool quit;
} State;

/**
 * struct Player - Defines the player's position and direction in the game map
 * @pos: Current position of the player as a Vec2F
 * @dir: Current direction the player is facing as a Vec2F
 * @plane: 2D plane representing the field of view for the player
 */
typedef struct Player
{
	Vec2F pos;
	Vec2F dir;
	Vec2F plane;
} Player;

/**
 * struct ColorRGBA - Represents a color with red, green, blue, and alpha
 * @r: The red channel of the color (0-255)
 * @g: The green channel of the color (0-255)
 * @b: The blue channel of the color (0-255)
 * @a: The alpha channel of the color (transparency, 0-255)
 */
typedef struct ColorRGBA
{
	uint8_t r, g, b, a;
} ColorRGBA;

#define RGBA_Red ((ColorRGBA) {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF})
#define RGBA_Green ((ColorRGBA) { .r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF })
#define RGBA_Blue ((ColorRGBA) { .r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF })

int main(void);
void render(State *, Player *);
float calculate_wall_distance(Side side, Vec2F *sideDist, Vec2F *deltaDist);
void render_column(State *state, int x, int lineHeight,
	Side side, Vec2I mapBox);
bool perform_dda(Vec2I *mapBox, Vec2F *sideDist,
	Vec2F *deltaDist, Vec2I *stepDir, Side *side);
Vec2F calculate_ray_direction(Player *player, int x);
void initialize_raycasting(Player *player, Vec2F rayDir,
	Vec2I *mapBox, Vec2F *sideDist, Vec2F *deltaDist, Vec2I *stepDir);
int xy2index(int x, int y, int w);
void cleanup(State *state);
void update_player(Player *player, const uint8_t *keystate, float moveSpeed);
void handle_input(State *state, Player *player, float rotateSpeed);
void initialize_sdl(State *state);
Player initialize_player(void);

#endif
