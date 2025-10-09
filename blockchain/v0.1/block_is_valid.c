#include "blockchain.h"

/**
 * block_is_valid - verifies that a Block is valid
 * @block: the Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if first Block
 * Return: 0 on success, 1 on failure
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	int i;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block || (block->info.index != 0 && !prev_block))
		return (1);
	if (prev_block && (block->info.index != prev_block->info.index + 1))
		return (1);
	if (block->info.index == 0) /* Genesis Block Check */
	{
		if (prev_block || block->info.difficulty || block->info.nonce)
			return (1);
		for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		{
			if (block->hash[i] != 0)
			{
				fprintf(stderr, "block->hash[%d] = %d\n", i, block->hash[i]);
				return (1);
			}
		}
	}
	if (prev_block)
		SHA256((unsigned char *)prev_block,
		prev_block->data.len + sizeof(prev_block->info), prev_hash);
	SHA256((unsigned char *)block,
	block->data.len + sizeof(block->info), current_hash);
	if (prev_block &&
		memcmp(prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (memcmp(prev_block->hash, block->info.prev_hash,
		SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (memcmp(current_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (sizeof(block->data.buffer) > BLOCKCHAIN_DATA_MAX)
		return (1);

	return (0);
}
