#include "struc.h"

int init_instance(SDL_Instance *instance)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "unable to intialize SDL: %s\n", SDL_GetError());
		return (1);
	}
	/* Create a new Window instance */
	instance->window = SDL_CreateWindow("SDL2 \\o/", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1260, SCREEN_HEIGHT, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow ERROR %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	/* create a new Renderer instance linked to the window */
	instance->renderer = SDL_CreateRenderer(instance->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}

int main(void)
{
	SDL_Instance instance;
	Map map = create_map();
	Point player_position = {MAP_WIDTH / 2, MAP_HEIGHT / 2};
	double player_angle = 0;
	int running = 1;

	if (init_instance(&instance) != 0)
		return (1);
	while (running)
	{
		if (poll_events() == 1)
			break;
		draw_stuff(instance, map, player_position, player_angle);
		raycast(instance.renderer, player_position.x * TILE_SIZE,
			player_position.y * TILE_SIZE, player_angle, map);
		SDL_RenderPresent(instance.renderer);
	}

	/* Cleanup */
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();

	return (0);
}

int poll_events(void)
{
	SDL_Event event;
	SDL_KeyboardEvent key;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return (1);
		case SDL_KEYDOWN:
			key = event.key;
			if (key.keysym.scancode == 0x29)
				return (1);
			else if (key.keysym.scancode == 0x78)
				
			break;
		}
	}
	return (0);
}
void draw_stuff(SDL_Instance instance, Map map,
	Point player_position, double player_angle)
{
	/* Set background color */
	SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
	SDL_RenderClear(instance.renderer);

	/* Draw the map walls */
	SDL_SetRenderDrawColor(instance.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			if (map.map[i][j] == 1) /* Wall */
			{
				SDL_Rect wall_rect = {i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE};
				SDL_RenderFillRect(instance.renderer, &wall_rect);
			}
		}
	}
}
