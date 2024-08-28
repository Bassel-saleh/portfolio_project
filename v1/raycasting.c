#include "struc.h"
#include <math.h>
#include <stdbool.h>

Map create_map(void)
{
	Map map = {0};
	/* Define a simple map with walls */
	/* 1 represents a wall */
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			/* Create walls around the border */
			if (i == 0 || j == 0 || i == MAP_WIDTH - 1 || j == MAP_HEIGHT - 1)
				map.map[i][j] = 1;
			/* Empty space */
			else
				map.map[i][j] = 0;
		}
	}
	return (map);
}

void raycast(SDL_Renderer *renderer, int player_x,
	int player_y, double player_angle, Map map)
{
	double fov_radians = FOV_DEG * (M_PI / 180.0);
	double angle_step = fov_radians / NUM_RAYS;

	for (int i = 0; i < NUM_RAYS; i++)
	{
		double angle = player_angle - fov_radians / 2 + i * angle_step;
		double ray_x = player_x;
		double ray_y = player_y;
		double distance_to_wall = 0;
		bool hit_wall = false;

		while (!hit_wall)
		{
			ray_x += cos(angle);
			ray_y += sin(angle);
			distance_to_wall += 1;

			/* Convert ray_x and ray_y to map coordinates */
			int map_x = (int)(ray_x / TILE_SIZE);
			int map_y = (int)(ray_y / TILE_SIZE);

			/* Check for wall collision */
			if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT || map.map[map_x][map_y] == 1)
			{
				hit_wall = true;
			}
		}

		/* Calculate the height of the wall slice to draw */
        int wall_height = (int)(SCREEN_HEIGHT / distance_to_wall);
        int wall_start = (SCREEN_HEIGHT / 2) - (wall_height / 2);
        int wall_end = (SCREEN_HEIGHT / 2) + (wall_height / 2);

        /* Draw the wall slice */
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, i, wall_start, i, wall_end);

        /* Draw the ceiling */
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, i, 0, i, wall_start);

        /* Draw the ground */
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, i, wall_end, i, SCREEN_HEIGHT);
	}
}
