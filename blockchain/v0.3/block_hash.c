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

	tx_size = llist_size(block->transactions); /* perhaps add -1 check */
	buffer = (unsigned char *)malloc(block->data.len + sizeof(block->info) +
	SHA256_DIGEST_LENGTH * tx_size);
	if (!buffer)
	{
		fprintf(stderr, "!buffer\n");
		return (NULL);
	}

	memcpy(buffer, (unsigned char *)block,
	sizeof(block->info) + block->data.len);
	offset = sizeof(block->info) + block->data.len;

	for (i = 0; i < tx_size; i++)
	{
		tx = (transaction_t *)llist_get_node_at(block->transactions, i);

		memcpy(buffer + offset + i * SHA256_DIGEST_LENGTH,
		tx->id, SHA256_DIGEST_LENGTH);
	}

	SHA256(buffer, block->data.len + sizeof(block->info) +
	SHA256_DIGEST_LENGTH, hash_buf);

	free(buffer);
	return (hash_buf);
}
