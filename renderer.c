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
	float cameraX = 2 * x / (float)SCREEN_W - 1;
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
bool perform_dda(Vec2I *mapBox, Vec2F *sideDist,Vec2F *deltaDist,
	Vec2I *stepDir, Side *side, uint8_t MAP[MAP_SIZE * MAP_SIZE])
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
void render_column(State *state, int x, int lineHeight, Side side, Vec2I mapBox, uint8_t MAP[MAP_SIZE * MAP_SIZE], float perpWallDist, Vec2F rayDir, Player *player)
{
	int drawStart, drawEnd;
	SDL_Rect srcRect, dstRect;
	SDL_Texture *texture;

	drawStart = -lineHeight / 2 + SCREEN_H / 2;
	if (drawStart < 0)
		drawStart = 0;
	drawEnd = lineHeight / 2 + SCREEN_H / 2;
	if (drawEnd >= SCREEN_H)
		drawEnd = SCREEN_H;

	// Select the texture based on the map value
	texture = state->wallTextures[MAP[xy2index(mapBox.x, mapBox.y, MAP_SIZE)]];

	// Calculate texture coordinates
	float wallX = (side == EastWest) ? player->pos.y + perpWallDist * rayDir.y : player->pos.x + perpWallDist * rayDir.x;
	wallX -= floor(wallX);
	srcRect.x = (int)(wallX * 512); // Width of your texture
	srcRect.y = 0;
	srcRect.w = 1;
	srcRect.h = 512; /* Height of your texture*/

	dstRect.x = x;
	dstRect.y = drawStart;
	dstRect.w = 1;
	dstRect.h = drawEnd - drawStart;

	// Set the texture color based on the side
	if (side == NorthSouth)
		SDL_SetTextureColorMod(texture, 128, 128, 128);
	else
		SDL_SetTextureColorMod(texture, 255, 255, 255);

	SDL_RenderCopy(state->renderer, texture, &srcRect, &dstRect);
}

/**
 * render - Renders the entire scene.
 * @state: Pointer to the State structure containing the SDL renderer.
 * @player: Pointer to the Player structure containing player information.
 * Return: None
 */
void render(State *state, Player *player, uint8_t MAP[MAP_SIZE * MAP_SIZE])
{
	SDL_Rect ceilingRect = {0, 0, SCREEN_W, SCREEN_H / 2};
	SDL_Rect groundRect = {0, SCREEN_H / 2, SCREEN_W, SCREEN_H / 2};

	/* Set color for the ceiling and fill the ceiling area */
	SDL_SetRenderDrawColor(state->renderer, RGBA_Ceiling.r, RGBA_Ceiling.g, RGBA_Ceiling.b, RGBA_Ceiling.a);
	SDL_RenderFillRect(state->renderer, &ceilingRect);

	// Set color for the ground and fill the ground area
	SDL_SetRenderDrawColor(state->renderer, RGBA_Ground.r, RGBA_Ground.g, RGBA_Ground.b, RGBA_Ground.a);
	SDL_RenderFillRect(state->renderer, &groundRect);
	for (int x = 0; x < SCREEN_W; ++x)
	{
		Vec2F rayDir = calculate_ray_direction(player, x);
		Vec2I mapBox;
		Vec2F sideDist = {0.0f, 0.0f};
		Vec2F deltaDist;
		Vec2I stepDir;
		Side side;

		initialize_raycasting(player, rayDir, &mapBox, &sideDist, &deltaDist, &stepDir);
		perform_dda(&mapBox, &sideDist, &deltaDist, &stepDir, &side, MAP);

		float perpWallDist = calculate_wall_distance(side, &sideDist, &deltaDist);
		int lineHeight = (int)(SCREEN_H / perpWallDist);

		render_column(state, x, lineHeight, side, mapBox, MAP, perpWallDist, rayDir, player);
		/*render_ground(state, x, lineHeight, mapBox, MAP, perpWallDist, player);*/
	}
	/*render_enemies(state, player);*/
	if (state->mapViewEnabled)
	{
		draw_map(state, MAP);
		draw_player(state, player);
	}
}
