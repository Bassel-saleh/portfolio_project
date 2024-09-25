#include "structure.h"

/**
 * load_map - Loads the map from a file and stores it in the MAP array.
 * @filename: The name of the file containing the map data.
 * @MAP: the map which is used during the game.
 * Return: true if the map is loaded successfully, false otherwise.
 */

bool load_map(const char *filename, uint8_t (*MAP)[MAP_SIZE * MAP_SIZE])
{
	FILE *file = fopen(filename, "r");

	if (!file)
	{
		printf("Failed to open map file: %s\n", filename);
		return (false);
	}

	for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++)
	{
		if (fscanf(file, "%hhu", &(*MAP)[i]) != 1)
		{
			printf("Failed to read map data at index %d\n", i);
			fclose(file);
			return (false);
		}
	}

	fclose(file);
	return (true);
}
