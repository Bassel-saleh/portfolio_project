#include "structure.h"

Vec2F calculate_ray_direction(Player *player, int x)
{
	float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
	Vec2F rayDir = {
		.x = player->dir.x + player->plane.x * cameraX,
		.y = player->dir.y + player->plane.y * cameraX,
	};
	return (rayDir);
}

bool perform_dda(Vec2I *mapBox, Vec2F *sideDist,
	Vec2F *deltaDist, Vec2I *stepDir, Side *side)
{
	bool hit = false;

	while (!hit)
	{
		if (sideDist->x < sideDist->y)
		{
			sideDist->x += deltaDist->x;
			mapBox->x += stepDir->x;
			*side = EastWest;
		}
		else
		{
			sideDist->y += deltaDist->y;
			mapBox->y += stepDir->y;
			*side = NorthSouth;
		}
		if (MAP[xy2index(mapBox->x, mapBox->y, MAP_SIZE)] > 0)
			hit = true;
	}
	return (hit);
}

float calculate_wall_distance(Side side, Vec2F *sideDist, Vec2F *deltaDist)
{
	float perpWallDist;

	if (side == EastWest)
		perpWallDist = (sideDist->x - deltaDist->x);
	else
		perpWallDist = (sideDist->y - deltaDist->y);

	return (perpWallDist);
}

void render_column(State *state, int x,
	int lineHeight, Side side, Vec2I mapBox)
{
	ColorRGBA color;
	int drawStart, drawEnd;

	drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;
	drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT;

	switch (MAP[xy2index(mapBox.x, mapBox.y, MAP_SIZE)])
	{
		case 1:
		color = RGBA_Red;
		break;
		case 2:
		color = RGBA_Green;
		break;
		case 3:
		color = RGBA_Blue;
		break;
	}

	if (side == NorthSouth)
		color.r /= 2, color.g /= 2, color.b /= 2;

	SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(state->renderer, x, drawStart, x, drawEnd);
}

void render(State *state, Player *player)
{
	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		Vec2F rayDir = calculate_ray_direction(player, x);
		Vec2I mapBox;
		Vec2F sideDist = {0.0f, 0.0f};
		Vec2F deltaDist;
		Vec2I stepDir;
		Side side;

			initialize_raycasting(player, rayDir, &mapBox,
				&sideDist, &deltaDist, &stepDir);
			perform_dda(&mapBox, &sideDist, &deltaDist, &stepDir, &side);

			float perpWallDist = calculate_wall_distance(side,
				&sideDist, &deltaDist);

			int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

			render_column(state, x, lineHeight, side, mapBox);
		}
}
