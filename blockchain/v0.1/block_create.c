#include "blockchain.h"

/**
 * block_create - creates a Block structure and initializes it
 * @prev: pointer to the Previous Block in the Blockchain
 * @data: memory area to duplicate in the Block's data
 * @data_len: stores the number of bytes to duplicate in data.
 *  If data_len is larger than BLOCKCHAIN_DATA_MAX then the latter must be used.
 *
 * Return: new Block or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *new_block = NULL;
	unsigned int true_data_len;

	if (!prev || !data || data_len == 0)
		return (NULL);

	true_data_len = (data_len <= BLOCKCHAIN_DATA_MAX) ?
	data_len : BLOCKCHAIN_DATA_MAX;

	new_block = (block_t *)malloc(sizeof(block_t));
	if (!new_block)
		return (NULL);

	memcpy(new_block->info.prev_hash, prev, 32);
	new_block->info.index = prev->info.index + 1;
	new_block->info.difficulty = 0;
	new_block->info.nonce = 0;
	new_block->info.timestamp = time(NULL); /* might need (uint64_t) */
	memcpy(new_block->data.buffer, data, true_data_len);
	new_block->data.len = true_data_len;

	return (new_block);
}
