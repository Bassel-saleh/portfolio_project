#include "structure.h"

/**
 * calculate_ray_direction - Calculates the direction
 * of a ray based on the player's position and screen coordinate.
 * @player: A pointer to the Player structure
 * containing the player's direction and plane vectors.
 * @x: The x-coordinate on the screen for which the ray direction is calculated
 * Return: The calculated direction of the ray as a Vec2F structure.
 * Description: This function calculates the direction of a ray based on the
 * player's position and the x-coordinate on the screen. It uses the player's
 * direction and plane vectors along with the cameraX value derived from the
 * screen coordinate to compute the ray direction.
 */
Vec2F calculate_ray_direction(Player *player, int x)
{
	float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
	Vec2F rayDir = {
		.x = player->dir.x + player->plane.x * cameraX,
		.y = player->dir.y + player->plane.y * cameraX,
	};
	return (rayDir);
}

/**
 * perform_dda - Performs the DDA algorithm to find where the ray hits a wall.
 * @mapBox: Pointer to the current map coordinates.
 * @sideDist: Pointer to distances to the nearest grid lines.
 * @deltaDist: Pointer to distances between grid lines.
 * @stepDir: Pointer to step directions in x and y axes.
 * @side: Pointer to store which side of the wall was hit.
 * Return: True if a wall is hit, false otherwise.
 */
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

/**
 * calculate_wall_distance - Computes the perpendicular distance to the wall.
 * @side: The side where the ray hit (EastWest or NorthSouth).
 * @sideDist: Pointer to distances to the nearest grid lines.
 * @deltaDist: Pointer to distances between grid lines.
 * Return: The distance to the wall.
 */
float calculate_wall_distance(Side side, Vec2F *sideDist, Vec2F *deltaDist)
{
	float perpWallDist;

	if (side == EastWest)
		perpWallDist = (sideDist->x - deltaDist->x);
	else
		perpWallDist = (sideDist->y - deltaDist->y);

	return (perpWallDist);
}

/**
 * render_column - Renders a vertical column of the screen.
 * @state: Pointer to the State structure containing the SDL renderer.
 * @x: The x-coordinate of the column to render.
 * @lineHeight: The height of the column to render.
 * @side: The side of the wall hit by the ray.
 * @mapBox: The map coordinates of the wall hit.
 * Return: None
 */
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

/**
 * render - Renders the entire scene.
 * @state: Pointer to the State structure containing the SDL renderer.
 * @player: Pointer to the Player structure containing player information.
 * Return: None
 */
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
