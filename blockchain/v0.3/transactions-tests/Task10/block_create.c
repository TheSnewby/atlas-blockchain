#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "blockchain.h"

/**
 * block_create - Allocates and initializes a new Block structure
 *
 * @prev:     Pointer to the previous block in the Blockchain
 * @data:     Data to be duplicated and stored in the Block
 * @data_len: Length of @data
 *
 * Return: A pointer to the created Block structure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t *block = malloc(sizeof(*block));

	if (!block || !prev)
		return (NULL);
	memset(block, 0, sizeof(*block));

	block->info.index = prev->info.index + 1;
	block->info.difficulty = 0;
	block->info.timestamp = (unsigned long)time(NULL);
	block->info.nonce = 0;
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	block->data.len = data_len;
	if (data_len > BLOCKCHAIN_DATA_MAX)
		block->data.len = BLOCKCHAIN_DATA_MAX;
	memcpy(block->data.buffer, data, block->data.len);

	block->transactions = llist_create(MT_SUPPORT_FALSE);

	memset(block->hash, 0, SHA256_DIGEST_LENGTH);

	return (block);
}
