#include "structure.h"

const uint8_t MAP[MAP_SIZE * MAP_SIZE] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 2, 2, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

Player initialize_player(void)
{
	Player player = {
		.pos = {.x = 4.0f, .y = 4.0f},
		.dir = {.x = -1.0f, .y = 0.0f},
		.plane = {.x = 0.0f, .y = 0.66f},
	};
	return (player);
}

void handle_input(State *state, Player *player, float rotateSpeed)
{
	SDL_Event event;
	Vec2F oldPlane, oldDir;
	float rotSpeed;
	int mouse_xrel = 0;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			state->quit = true;
		else if (event.type == SDL_MOUSEMOTION)
			mouse_xrel = event.motion.xrel;
	}

	const uint8_t *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		state->quit = true;
	if (mouse_xrel != 0)
	{
		rotSpeed = rotateSpeed * (mouse_xrel * -0.1);
		oldDir = player->dir;
		player->dir.x = player->dir.x *
			cosf(rotSpeed) - player->dir.y * sinf(rotSpeed);
		player->dir.y = oldDir.x * sinf(rotSpeed) + player->dir.y * cosf(rotSpeed);

		oldPlane = player->plane;
		player->plane.x = player->plane.x *
			cosf(rotSpeed) - player->plane.y * sinf(rotSpeed);
		player->plane.y = oldPlane.x *
			sinf(rotSpeed) + player->plane.y * cosf(rotSpeed);
	}
}

void update_player(Player *player, const uint8_t *keystate, float moveSpeed)
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

void cleanup(State *state)
{
	SDL_DestroyRenderer(state->renderer);
	SDL_DestroyWindow(state->window);
	SDL_Quit();
}

int main(void)
{
	Player player = initialize_player();
	const float rotateSpeed = 0.025, moveSpeed = 0.05;
	State state = {.quit = false};

	initialize_sdl(&state);

	while (!state.quit)
	{
		handle_input(&state, &player, rotateSpeed);

		const uint8_t *keystate = SDL_GetKeyboardState(NULL);

		update_player(&player, keystate, moveSpeed);

		SDL_SetRenderDrawColor(state.renderer, 0x18, 0x18, 0x18, 0xFF);
		SDL_RenderClear(state.renderer);

		render(&state, &player);

		SDL_RenderPresent(state.renderer);
	}

	cleanup(&state);
	return (0);
}
