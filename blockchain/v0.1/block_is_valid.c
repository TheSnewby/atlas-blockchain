#include "blockchain.h"

/**
 * block_is_valid - verifies that a Block is valid
 * @block: the Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if first Block
 * Return: 1 on success, 0 on failure ???
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	int i;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block || (block->info.index != 0 && !prev_block))
		return (0);

	if (block->info.index != prev_block->info.index + 1)
		return (0);

	if (block->info.index == 0) /* Genesis Block Check */
	{
		if (prev_block || block->info.difficulty || block->info.nonce)
			return (0);
		for (i = 0; i < 32; i++)
			if (block->hash[i] != 0)
				return (0);
	}
	sha256(prev_block->data.buffer, prev_block->data.len, prev_hash);
	sha256(block->data.buffer, SHA256_DIGEST_LENGTH, current_hash);
	if (memcmp(prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(prev_block->hash, block->info.prev_hash,
		SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(current_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (0);
	if (sizeof(block->data.buffer) > BLOCKCHAIN_DATA_MAX)
		return (0);

	return (1);
}
