#include "structure.h"

void draw_rays(SDL_Instance instance, float *player_x, float *player_y, float *pa, int (*map)[64])
{
	float px = *player_x;
	float py = *player_y;
	int r, mx, my, mp, dof;
	float rx, ry, xo, yo, atan;

	for (r = 0; r < 1; r++)
	{
		printf("%f\n", *pa);
		/* checking horizontal lines */
		dof = 0;
		if (tan(*pa) != 0)
			atan = -1 / tan(*pa);
		else
			atan = 0;
		/*printf("%f\n", atan);*/
		if (*pa > M_PI)
		{
			ry = (((int)py >> 6) << 6) - 0.0001;
			rx = (py - ry) * atan + px;
			yo = -64;
			xo = -yo * atan;
		}
		if (*pa < M_PI)
		{
			ry = (((int)py >> 6) << 6) + 64;
			rx = (py - ry) * atan + px;
			yo = 64;
			xo = -yo * atan;
		}
		if (*pa == 0 || *pa == M_PI)
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mp < mapX * mapY && map[my][mx] == 1)
				dof = 8;
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
			SDL_SetRenderDrawColor(instance.renderer, 0xFF, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawLine(instance.renderer, px + 3, py + 3, rx, ry);
		}
	}
}
