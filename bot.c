#include "structure.h"

void handle_enemies(State *state) {
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - state->enemySpawnTimer >= 3000) { // 30 seconds
		if (state->numEnemies < 10) {
			state->enemies[state->numEnemies].pos = (Vec2F){5, 5}; // Random position
			state->enemies[state->numEnemies].texture = load_texture(state->renderer, "./texture/enemy_1.png");
			state->numEnemies++;
		}
		state->enemySpawnTimer = currentTime;  // Reset the timer
	}
}

/**
 * render_enemies - Renders all enemies in the scene.
 * @state: Pointer to the State structure containing the SDL renderer.
 * @player: Pointer to the Player structure containing player information.
 * @MAP: The map array representing the game map.
 * Return: None
 */
void render_enemies(State *state, Player *player)
{
	for (int i = 0; i < state->numEnemies; i++) {
		Enemy *enemy = &state->enemies[i];

		// Calculate the relative position of the enemy
		Vec2F enemyRel = {
			.x = enemy->pos.x - player->pos.x,
			.y = enemy->pos.y - player->pos.y
		};

		// Calculate the angle of the enemy relative to the player's view direction
		float angle = atan2f(enemyRel.y, enemyRel.x) - atan2f(player->dir.y, player->dir.x);
		float distance = sqrtf(enemyRel.x * enemyRel.x + enemyRel.y * enemyRel.y);

		// Check if the enemy is within the player's field of view
		if (fabsf(angle) < playerFOV / 2 && distance > 0.1) {
			// Project the enemy’s position onto the screen
			float screenX = (SCREEN_W / 2) + (SCREEN_W / 2) * (enemyRel.x * player->dir.x + enemyRel.y * player->dir.y) / distance;
			float screenY = (SCREEN_H / 2) - (SCREEN_H / 2) * (enemyRel.x * player->dir.y - enemyRel.y * player->dir.x) / distance;

			// Scale the enemy's size based on distance
			float scale = 64 / distance; // Adjust the scale factor as needed

			SDL_Rect dstRect = {
				.x = (int)(screenX - (scale * 32)), // Center the texture
				.y = (int)(screenY - (scale * 32)), // Center the texture
				.w = (int)(scale * 64),
				.h = (int)(scale * 64)
			};

			// Render the enemy texture
			SDL_RenderCopy(state->renderer, enemy->texture, NULL, &dstRect);
		}
	}
}
