#include "structure.h"

/**
 * xy2index - Converts (x, y) coordinates to a single index for a 2D map.
 * @x: The x-coordinate in the map.
 * @y: The y-coordinate in the map.
 * @w: The width of the map.
 * Return: The computed index for the given (x, y) coordinates.
 * Description: This function calculates the index in a 1D array representation
 * of a 2D map based on the provided (x, y) coordinates
 * and the width of the map
 */
int xy2index(int x, int y, int w)
{
	return (y * w + x);
}

/**
 * initialize_sdl - Initializes the SDL library
 * and creates a window and renderer
 * @state: A pointer to the State structure to be initialized.
 * Description: This function initializes the SDL video subsystem, creates an
 * SDL window with high DPI support, and sets up an SDL renderer with vertical
 * synchronization enabled. It also sets the SDL relative mouse mode. If any
 * of these operations fail, it prints an error message
 * and terminates the program
 */
void initialize_sdl(State *state)
{
	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"SDL failed to initialize; %s\n", SDL_GetError());
	state->window = SDL_CreateWindow("Raycast",
									  SDL_WINDOWPOS_CENTERED_DISPLAY(0),
									  SDL_WINDOWPOS_CENTERED_DISPLAY(0),
									  SCREEN_WIDTH,
									  SCREEN_HEIGHT,
									  SDL_WINDOW_ALLOW_HIGHDPI);
	ASSERT(state->window, "failed to create SDL window: %s\n", SDL_GetError());
	state->renderer = SDL_CreateRenderer(state->window, -1,
		SDL_RENDERER_PRESENTVSYNC);
	ASSERT(state->renderer, "failed to create SDL renderer: %s\n",
		SDL_GetError());
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
	SDL_SetRelativeMouseMode(true);
}

/**
 * initialize_raycasting - Initializes raycasting
 * parameters based on the player's position and direction.
 * @player: A pointer to the Player structure whose position is used.
 * @rayDir: The direction of the ray being cast.
 * @mapBox: A pointer to a Vec2I structure
 * where the map box coordinates will be stored.
 * @sideDist: A pointer to a Vec2F structure where
 * the initial distances to the nearest grid lines will be stored.
 * @deltaDist: A pointer to a Vec2F structure where
 * the distances between grid lines will be stored.
 * @stepDir: A pointer to a Vec2I structure where
 * the step directions in the x and y axes will be stored.
 * Description: This function calculates and
 * initializes the raycasting parameters
 * needed for performing raycasting.
 * It determines the initial map box coordinates,
 * the distance to the nearest grid lines
 * in the x and y directions, and the step
 * directions based on the ray direction and player's position.
 */
void initialize_raycasting(Player *player, Vec2F rayDir,
	Vec2I *mapBox, Vec2F *sideDist, Vec2F *deltaDist, Vec2I *stepDir)
{
	mapBox->x = (int)player->pos.x;
	mapBox->y = (int)player->pos.y;

	deltaDist->x = (rayDir.x == 0) ? 1e30 : fabsf(1 / rayDir.x);
	deltaDist->y = (rayDir.y == 0) ? 1e30 : fabsf(1 / rayDir.y);

	if (rayDir.x < 0)
	{
		stepDir->x = -1;
		sideDist->x = (player->pos.x - mapBox->x) * deltaDist->x;
	} else
	{
		stepDir->x = 1;
		sideDist->x = (mapBox->x + 1.0f - player->pos.x) * deltaDist->x;
	}
	if (rayDir.y < 0)
	{
		stepDir->y = -1;
		sideDist->y = (player->pos.y - mapBox->y) * deltaDist->y;
	} else
	{
		stepDir->y = 1;
		sideDist->y = (mapBox->y + 1.0f - player->pos.y) * deltaDist->y;
	}
}
