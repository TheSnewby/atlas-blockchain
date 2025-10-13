#include "blockchain.h"

/**
 * checks whether the given hash matches the given difficulty
 * @hash: hash to check
 * @difficulty: minimum difficulty the hash should match
 * Return: 1 if the difficulty is respected, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty)
{
	int i;

	if (!hash)
		return (0);

	for (i = 0; i < (int)difficulty && i < SHA256_DIGEST_LENGTH; i++)
	{
		if (hash[i] != 0)
			return (0);
	}
	return (1);
}
