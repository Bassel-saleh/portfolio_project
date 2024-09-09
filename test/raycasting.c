#include "structure.h"

float dist(float ax, float ay, float bx, float by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void draw_rays(SDL_Instance instance, float *px,
	float *py, float *pa, int map[64])
{
	int r, mx, my, mp, dof, disT;
	float rx, ry, xo, yo, atan, ntan, hx = *px, hy, vx = *px, vy = *py;
	float disH, disV, ca, lineH, lineO;
	double ra;
	SDL_Rect rect;

	ra = DEG_TO_RAD(*pa - 30);
	if (ra<0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;
	for (r = 0; r < 60; r++)
	{
		/* HORIZONTAL RAYCASTING */
		dof = 0, atan = -1 / tan(ra), hx = *px, hy = *py, disH = 1000000;
		if (ra == 0 || ra == M_PI)
			calculate_rx_ry(px, py, &rx, &ry, *pa, map), dof = 8;
		else if (ra > M_PI)
			ry = (((int)*py >> 6) << 6) - 0.0001,
				rx = (*py - ry) * atan + *px, yo = -64, xo = -yo * atan;
		else if (ra < M_PI)
			ry = (((int)*py >> 6) << 6) + 64, rx = (*py - ry) * atan + *px,
				yo = 64, xo = -yo * atan;
		while (dof < 8)
		{
			mx = (int)(rx) >> 6, my = (int)(ry) >> 6, mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				hx = rx, hy = ry, disH = dist(*px, *py, hx, hy), dof = 8;
			else
				rx += xo, ry += yo, dof++;
		}
		/*SDL_SetRenderDrawColor(instance.renderer, 0x00, 0xFF, 0xFF, 0xFF);
		printf("rx: %f, ry: %f, pa: %f\n", rx, ry, *pa);
		SDL_RenderDrawLine(instance.renderer, *px, *py, rx, ry);*/
		/* VERTICAL RAYCASTING */
		dof = 0, ntan = -tan(ra), vx = *px, vy = *py, disV = 1000000;;
		if (ra == 0 || ra == M_PI)
			calculate_rx_ry(px, py, &rx, &ry, *pa, map), dof = 8;
		else if (ra > P2 && ra < P3)
			rx = (((int)*px >> 6) << 6) - 0.0001,
				ry = (*px - rx) * ntan + *py, xo = -64, yo = -xo * ntan;
		else if (ra < P2 || ra > P3)
			rx = (((int)*px >> 6) << 6) + 64, ry = (*px - rx) * ntan + *py,
				xo = 64, yo = -xo * ntan;
		while (dof < 8)
		{
			mx = (int)(rx) >> 6, my = (int)(ry) >> 6, mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				vx = rx, vy = ry, disV = dist(*px, *py, vx, vy), dof = 8;
			else
				rx += xo, ry += yo, dof++;
		}
		if (disV < disH)
			rx = vx, ry = vy, disT = disV;
		else if (disH < disV)
			rx = hx, ry = hy, disT = disH;
		/*printf("disV = %f, disH = %f\n", disV, disH);*/
		SDL_SetRenderDrawColor(instance.renderer, 0x00, 0xFF, 0xFF, 0xFF);
		/*printf("rx: %f, ry: %f, pa: %f\n", rx, ry, *pa);*/
		SDL_RenderDrawLine(instance.renderer, *px, *py, rx, ry);
		/* 3D */
		ca = DEG_TO_RAD(*pa) - ra;
		if (ca < 0)
			ca += 2 * M_PI;
		if (ca > 2 * M_PI)
			ca -= 2 * M_PI;
		disT = disT * cos(ca);
		lineH = (mapS * 320) / disT;
		lineO = 160 - lineH / 2;
		rect.x = r * 8 + 530,rect.y = lineO,rect.w = 8,rect.h = lineH;
		SDL_SetRenderDrawColor(instance.renderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(instance.renderer, &rect);
		ra += M_PI / 180;
		if (ra<0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;	
	}
}

void calculate_rx_ry(float *px, float *py, float *rx, float *ry, float pa, int map[64])
{
    int mx, my, mp;
    
    if (pa < 90 || pa > 270) /* Facing right */
    {
        *ry = *py; /* y-coordinate remains the same */
        *rx = *px;

        /* Increment rx to find the nearest wall or boundary */
        while (1)
        {
            *rx += 1; /* Move right */
            mx = (int)(*rx) >> 6; /* Convert rx to map grid */
            my = (int)(*ry) >> 6; /* Convert ry to map grid */
            mp = my * mapX + mx; /* Calculate map position */

            /* Check if the ray hits a wall */
            if (mp < mapX * mapY && map[mp] == 1)
				break; /* Ray hit a wall */
        }
    }
    else if (pa > 90 || pa < 270) /* Facing left */
    {
        *ry = *py; /* y-coordinate remains the same */
        *rx = *px;

        /* Decrement rx to find the nearest wall or boundary */
        while (1)
        {
            *rx -= 1; // Move left
            mx = (int)(*rx) >> 6; /* Convert rx to map grid */
            my = (int)(*ry) >> 6; /* Convert ry to map grid */
            mp = my * mapX + mx; /* Calculate map position */

            /* Check if the ray hits a wall */
            if (mp < mapX * mapY && map[mp] == 1)

                break; /* Ray hit a wall */
        }
    }
}
