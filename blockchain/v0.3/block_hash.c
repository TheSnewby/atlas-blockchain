#include "blockchain.h"

/**
 * block_hash - computes the hash of a Block
 * @block: block to be hashed
 * @hash_buf: location to store hash
 *
 * Return: pointer to hash_buf, NULL on failure
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	unsigned char *buffer = NULL;
	transaction_t *tx = NULL;
	int tx_size, i, offset;

	if (!block || !hash_buf)
	{
		fprintf(stderr, "!param\n");
		return (NULL);
	}

	offset = sizeof(block->info) + block->data.len;

	tx_size = llist_size(block->transactions); /* perhaps add -1 check */
	buffer = (unsigned char *)malloc(offset + SHA256_DIGEST_LENGTH * tx_size);
	if (!buffer)
	{
		fprintf(stderr, "!buffer\n");
		return (NULL);
	}

	fprintf(stderr, "{DEBUG} block->data[%d]: %.*s\n",
		block->info.index, block->data.len, block->data.buffer);

	memcpy(buffer, (unsigned char *)block, offset);

	for (i = 0; i < tx_size; i++)
	{
		tx = (transaction_t *)llist_get_node_at(block->transactions, i);

		memcpy(buffer + offset + i * SHA256_DIGEST_LENGTH,
		tx->id, SHA256_DIGEST_LENGTH);
	}

	SHA256(buffer, offset +	SHA256_DIGEST_LENGTH, hash_buf);

	free(buffer);
	return (hash_buf);
}
