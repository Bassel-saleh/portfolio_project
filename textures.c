#include "structure.h"

SDL_Texture* load_texture(SDL_Renderer *renderer, const char *path)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path);
	if (!texture)
	{
		fprintf(stderr, "Failed to load texture: %s\n", SDL_GetError());
		exit(1);
	}
	return texture;
}

/**
 * render_ground - Renders the textured ground based on the player's view.
 * @state: A pointer to the State structure containing SDL resources.
 * @player: A pointer to the Player structure representing the player's position and direction.
 * @MAP: The game map array.
 * 
 * This function renders the ground using a texture and calculates the appropriate
 * texture coordinates for each pixel, ensuring proper scaling and mapping.
 */
void render_ground(State *state, int x, int lineHeight, Vec2I mapBox, uint8_t MAP[MAP_SIZE * MAP_SIZE], float perpWallDist, Player *player)
{
	(void)perpWallDist;
	(void)player;
	int drawStart, drawEnd;
	SDL_Rect srcRect, dstRect;
	SDL_Texture *texture;

	// Define the color for the ground
	// SDL_SetRenderDrawColor(state->renderer, 0, 255, 0, 255); // Green color (R, G, B, A)

	drawStart = -lineHeight / 2 + SCREEN_H / 2;
	if (drawStart < 0)
		drawStart = 0;
	drawEnd = lineHeight / 2 + SCREEN_H / 2;
	if (drawEnd >= SCREEN_H)
		drawEnd = SCREEN_H;

	texture = state->floorTextures[MAP[xy2index(mapBox.x, mapBox.y, MAP_SIZE)]];

	// Update srcRect to cover the entire texture
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 512; // Assuming the texture width is 512
	srcRect.h = 512; // Assuming the texture height is 512

	// Set the dstRect to cover the ground area
	dstRect.x = x;
	dstRect.y = drawEnd;
	dstRect.w = 1; // Width of the column
	dstRect.h = SCREEN_H - drawEnd; // Height from the bottom of the wall to the bottom of the screen

	// Render the ground texture as a filled rectangle
	// Adjust the texture coordinates if needed to ensure proper mapping
	SDL_RenderCopy(state->renderer, texture, &srcRect, &dstRect);
}

void render_weapon(State *state)
{
	int weaponWidth = 600;  // Width of the weapon texture
	int weaponHeight = 300; // Height of the weapon texture

	SDL_Rect weaponRect;
	weaponRect.w = weaponWidth;
	weaponRect.h = weaponHeight;
	weaponRect.x = (SCREEN_W - weaponWidth) / 2 + 250;  // Center the weapon horizontally
	weaponRect.y = SCREEN_H - weaponHeight;  // 10 pixels from the bottom

	SDL_RenderCopy(state->renderer, state->weaponTexture[1], NULL, &weaponRect);
}
