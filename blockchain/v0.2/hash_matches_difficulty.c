#include "blockchain.h"

/**
 * hash_matches_difficulty - checks whether the hash matches the difficulty
 * @hash: hash to check
 * @difficulty: minimum difficulty the hash should match
 * Return: 1 if the difficulty is respected, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty)
{
	int iByte, iBit, mask, numBytes, remainingBits;

	if (!hash)
		return (0);

	numBytes = (int)difficulty / 8;
	remainingBits = ((int)difficulty % 8);

	for (iByte = 0; iByte < numBytes; iByte++)
	{
		if (hash[iByte] != 0)
			return (0);
	}

	for (iBit = 7; iBit >= (8 - remainingBits); iBit--)
	{
		mask = 1 << iBit;
		if ((mask & hash[iByte]) != 0)
			return (0);
	}

	return (1);
}
