#include "structure.h"

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
		SDL_WINDOWPOS_CENTERED, 1260, SCREEN_H, 0);
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
int map[] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
	float px = 200, py = 200, pdx = 5, pdy = 0, pa = 0;
	SDL_Instance instance;

	if (init_instance(&instance) != 0)
		return (1);
	while ("C is awesome")
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance.renderer);
		if (poll_events(&py, &px, &pdx, &pdy, &pa) == 1)
			break;
		draw_map(instance, map);
		draw_player(instance, &py, &px, &pdx, &pdy);
		draw_rays(instance, &px, &py, &pa, &map);
		SDL_RenderPresent(instance.renderer);
	}

	/* Cleanup */
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();

	return (0);
}

int poll_events(float *py, float *px, float *pdx, float *pdy, float *pa)
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
			if (event.key.keysym.scancode == SDL_SCANCODE_W)
			{
				(*px) += *pdx;
				(*py) += *pdy;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_S)
			{
				(*px) -= *pdx;
				(*py) -= *pdy;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_A)
			{
				(*pa) -= DEG_TO_RAD(1);
				if (*pa < 0)
					(*pa) += (2 * M_PI);
				(*pdx) = cos(*pa) * 5;
				(*pdy) = sin(*pa) * 5;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_D)
			{
				(*pa) += DEG_TO_RAD(1);
				if (*pa > 2 * M_PI)
					(*pa) -= (2 * M_PI);
				(*pdx) = cos(*pa)* 5;
				(*pdy) = sin(*pa)* 5;
			}
			break;
		}
	}
	return (0);
}

void draw_player(SDL_Instance instance, float *player_y, float *player_x,
	float *dir_x, float *dir_y)
{
	float px = *player_x;
    float py = *player_y;
    float pdx = *dir_x;
    float pdy = *dir_y;
	float a = (px + 3 + pdx * 5);
	float b = (py + 3 + pdy * 5);
	SDL_Rect square = {px, py, 6, 6};

	SDL_SetRenderDrawColor(instance.renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(instance.renderer, &square);
	SDL_RenderDrawLine(instance.renderer, px + 3, py + 3, (int)a, (int)b);
}

void draw_map(SDL_Instance instance, int *map)
{
	int x, y;
	int tile;

	for (y = 0; y < mapY; y++)
	{
		for(x = 0; x < mapX; x++)
		{
			if (map[y * mapX + x] == 1)
			{
				tile = map[y * mapX + x];
            	if (tile == 1)
                SDL_SetRenderDrawColor(instance.renderer, 128, 128, 128, 255);

				else
                SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);

            SDL_Rect rect = {x * mapS, y * mapS, mapS, mapS};
            SDL_RenderFillRect(instance.renderer, &rect);
			}
		}
	}
}
