#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

#define SCREEN_W 1280
#define SCREEN_H 720

#define MAP_SIZE 16

#define PI 3.14159265f
#define playerFOV (PI / 2.0f)
#define maxDepth 20.0f
#define MAX_TEXTURES 10

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

/**
 * struct Enemy - Represents an enemy in the game.
 * @pos: The position of the enemy.
 * @texture: The texture of the enemy.
 */
typedef struct Enemy
{
	Vec2F pos;
	SDL_Texture *texture;
} Enemy;

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
	bool mapViewEnabled;
	SDL_Texture* wallTextures[4];
	SDL_Texture* floorTextures[4];
	SDL_Texture* weaponTexture[4];
	Enemy enemies[10];  // Array of enemies
	int numEnemies;
	Uint32 enemySpawnTimer;  // Timer for spawning enemies
} State;

#define RGBA_Red ((ColorRGBA) {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF})
#define RGBA_Green ((ColorRGBA) { .r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF })
#define RGBA_Blue ((ColorRGBA) { .r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF })
#define RGBA_wall ((ColorRGBA) {.r = 0x70, .g = 0x70, .b = 0x70, .a = 0xFF})
#define RGBA_Ceiling ((ColorRGBA) {.r = 0x1E, .g = 0x90, .b = 0xFF, .a = 0xFF})
#define RGBA_Ground ((ColorRGBA) {.r = 0x22, .g = 0x8B, .b = 0x22, .a = 0xFF})

int main(void);
bool load_map(const char *filename, uint8_t (*MAP)[MAP_SIZE * MAP_SIZE]);
void render(State *state, Player *player, uint8_t MAP[MAP_SIZE * MAP_SIZE]);
float calculate_wall_distance(Side side, Vec2F *sideDist, Vec2F *deltaDist);
void render_column(State *state, int x, int lineHeight, Side side, Vec2I mapBox, uint8_t MAP[MAP_SIZE * MAP_SIZE], float perpWallDist, Vec2F rayDir, Player *player);
bool perform_dda(Vec2I *mapBox, Vec2F *sideDist,
	Vec2F *deltaDist, Vec2I *stepDir, Side *side, uint8_t [MAP_SIZE * MAP_SIZE]);
Vec2F calculate_ray_direction(Player *player, int x);
void initialize_raycasting(Player *player, Vec2F rayDir,
	Vec2I *mapBox, Vec2F *sideDist, Vec2F *deltaDist, Vec2I *stepDir);
int xy2index(int x, int y, int w);
void cleanup(State *state);
void update_player(Player *player, const uint8_t *keystate, float moveSpeed, uint8_t [MAP_SIZE * MAP_SIZE]);
void handle_input(State *state, Player *player, float rotateSpeed);
void initialize_sdl(State *state);
Player initialize_player(void);
void draw_map(State *state, uint8_t MAP[MAP_SIZE * MAP_SIZE]);
void draw_player(State *state, Player *player);
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);
void render_ground(State *state, int x, int lineHeight, Vec2I mapBox, uint8_t MAP[MAP_SIZE * MAP_SIZE], float perpWallDist, Player *player);
void render_weapon(State *state);
Enemy initialize_enemy(SDL_Renderer *renderer);
void handle_enemies(State *state);
void render_enemies(State *state, Player *player);

#endif
