#include "structure.h"

int xy2index(int x, int y, int w)
{
	return (y * w + x);
}

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
