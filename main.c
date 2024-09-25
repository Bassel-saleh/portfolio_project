#include "structure.h"

/**
 * initialize_player - Initializes and returns
 * a Player structure with default values.
 * Return: The initialized Player structure.
 */
Player initialize_player(void)
{
	Player player = {
		.pos = {.x = 4.0f, .y = 4.0f},
		.dir = {.x = -1.0f, .y = 0.0f},
		.plane = {.x = 0.0f, .y = 0.66f},
	};
	return (player);
}

/**
 * handle_input - Handles user input and updates the Player and
 * State based on keyboard and mouse events.
 * @state: A pointer to the State structure to be updated.
 * @player: A pointer to the Player structure to be updated.
 * @rotateSpeed: The speed at which the player
 * should rotate based on mouse movement.
 * Description: This function processes events
 * from the SDL event queue, including
 * keyboard inputs for quitting the application
 * and mouse movements for rotating the
 * player's view. It updates the direction
 * and plane vectors of the Player structure
 * based on the mouse's relative movement.
 */
void handle_input(State *state, Player *player, float rotateSpeed)
{
	SDL_Event event;
	Vec2F oldPlane, oldDir;
	float rotSpeed = 0.0f;
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			state->quit = true;
	}
	if (keystate[SDL_SCANCODE_ESCAPE])
		state->quit = true;

	if (keystate[SDL_SCANCODE_LEFT])
		rotSpeed = rotateSpeed;
	else if (keystate[SDL_SCANCODE_RIGHT])
		rotSpeed = -rotateSpeed;

	if (rotSpeed != 0.0f)
	{
		oldDir = player->dir;
		player->dir.x = player->dir.x *
			cosf(rotSpeed) - player->dir.y * sinf(rotSpeed);
		player->dir.y = oldDir.x * sinf(rotSpeed) + player->dir.y * cosf(rotSpeed);

		oldPlane = player->plane;
		player->plane.x = player->plane.x * cosf(rotSpeed) -
			player->plane.y * sinf(rotSpeed);
		player->plane.y = oldPlane.x *
			sinf(rotSpeed) + player->plane.y * cosf(rotSpeed);
	}
	if (keystate[SDL_SCANCODE_M])
	{
		state->mapViewEnabled = !state->mapViewEnabled;
		/* Add a small delay to prevent rapid toggling */
		SDL_Delay(200);
	}
}

/**
 * update_player - Updates the Player's position based on keyboard input.
 * @player: A pointer to the Player structure to be updated.
 * @keystate: A pointer to the current state of the keyboard.
 * @moveSpeed: The speed at which the player should move.
 * @MAP: the map which is used during the game.
 */
void update_player(Player *player, const uint8_t *keystate,
	float moveSpeed, uint8_t MAP[MAP_SIZE * MAP_SIZE])
{
	Vec2F deltaPos = {.x = player->dir.x *
		moveSpeed, .y = player->dir.y * moveSpeed};

	if (keystate[SDL_SCANCODE_W])
	{
		if (MAP[xy2index(player->pos.x + deltaPos.x, player->pos.y, MAP_SIZE)] == 0)
			player->pos.x += deltaPos.x;
		if (MAP[xy2index(player->pos.x, player->pos.y + deltaPos.y, MAP_SIZE)] == 0)
			player->pos.y += deltaPos.y;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		if (MAP[xy2index(player->pos.x - deltaPos.x, player->pos.y, MAP_SIZE)] == 0)
			player->pos.x -= deltaPos.x;
		if (MAP[xy2index(player->pos.x, player->pos.y - deltaPos.y, MAP_SIZE)] == 0)
			player->pos.y -= deltaPos.y;
	}
	if (keystate[SDL_SCANCODE_A])
	{
		if (MAP[xy2index(player->pos.x - deltaPos.y, player->pos.y, MAP_SIZE)] == 0)
			player->pos.x -= deltaPos.y;
		if (MAP[xy2index(player->pos.x, player->pos.y + deltaPos.x, MAP_SIZE)] == 0)
			player->pos.y += deltaPos.x;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		if (MAP[xy2index(player->pos.x + deltaPos.y, player->pos.y, MAP_SIZE)] == 0)
			player->pos.x += deltaPos.y;
		if (MAP[xy2index(player->pos.x, player->pos.y - deltaPos.x, MAP_SIZE)] == 0)
			player->pos.y -= deltaPos.x;
	}
}


/**
 * cleanup - Cleans up SDL resources and quits SDL.
 * @state: A pointer to the State structure containing SDL resources.
 * Description: This function destroys the SDL renderer and window, and then
 * quits the SDL library. It should be called before the application exits to
 * release resources properly.
 */
void cleanup(State *state)
{
	for (int i = 0; i < 4; ++i)
	{
		if (state->wallTextures[i])
			SDL_DestroyTexture(state->wallTextures[i]);
		if (state->floorTextures[i])
			SDL_DestroyTexture(state->wallTextures[i]);
		if (state->weaponTexture[i])
			SDL_DestroyTexture(state->wallTextures[i]);
	}
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	SDL_Quit();
}

/**
 * main - The entry point of the application.
 * Return: 0 on successful completion.
 * Description: This function initializes SDL library and creates the Player
 * and State structures. It runs the main loop where it handles input, updates
 * the player, clears and renders the screen, and presents the rendered image.
 * The loop continues until the quit flag in the State structure is set to true
 * After exiting the loop, it cleans up SDL resources and exits.
 */
int main(void)
{
	uint8_t MAP[MAP_SIZE * MAP_SIZE];
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);

	if (!load_map("map_1", &MAP))
		return (1);  /* Exit if map loading fails */
	Player player = initialize_player();
	const float rotateSpeed = 0.025, moveSpeed = 0.05;
	State state = {.quit = false, .mapViewEnabled = false,
		.numEnemies = 0, .enemySpawnTimer = SDL_GetTicks()};

	initialize_sdl(&state);
	while (!state.quit)
	{
		handle_input(&state, &player, rotateSpeed);
		update_player(&player, keystate, moveSpeed, MAP);
		handle_enemies(&state);
		SDL_SetRenderDrawColor(state.renderer, 0x18, 0x18, 0x18, 0xFF);
		SDL_RenderClear(state.renderer);
		render(&state, &player, MAP);
		render_weapon(&state);
		SDL_RenderPresent(state.renderer);
	}
	cleanup(&state);
	return (0);
}
